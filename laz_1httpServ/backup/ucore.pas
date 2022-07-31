unit uCore;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
   process, fpjson,
   jsonparser, jsonscanner, uTypes, ExtCtrls, StdCtrls, Controls, Graphics,
   fphttpserver, httpdefs, httproute, sqlite3conn, sqldb, Dialogs;

type
  { TServerProcess }
  TServerProcess  = class(TThread)
  private
    _Error: string;
    FServer: TFPHttpServer;
    FdbConn: TSQLite3Connection;
    FdbTransact: TSQLTransaction;
    fdebuginfoAdd, fdebuginfoText: string;
    procedure OnRequest(Sender: TObject; Var ARequest: TFPHTTPConnectionRequest; Var AResponse : TFPHTTPConnectionResponse);
    function ContentToMap(AContent: string): Boolean;
  private
    FDevList: TDeviceList;
    function IsDevice(ARequest: TFPHTTPConnectionRequest; var ADevice: TDevice): Boolean;
  protected
    procedure logAdd(const AValue: string);
    procedure _logAdd;
    procedure logText(const AValue: string);
    procedure _logText;
    procedure Execute; override;
  public
    constructor Create;
    destructor Destroy; override;
  end;

  TPingProcess = class(TThread)
  private
    FChangeMainFon: string;
    FDeviceList, FNewDeviceList, FOldDeviceList: TDeviceList;
    FPingedList: TStringList;
  private
    FAppPath: string;
    fdebuginfo: string;
    FHomeCotrollButton: THomeCotrollButton;
// core info
    procedure DoPing_nmap;
    procedure DoPing;
    procedure DoState;
    procedure DoGUIControll;
    function CheckDevice(AValue: string): Boolean;
   // procedure _log;
  protected
    procedure Execute; override;
// debug info
    procedure ShowStatePinged;
    procedure ShowStateDevices;
    procedure ShowStatePingOk;
    procedure ShowStateStateOk;
    procedure log;
// GUI interface info
    procedure ChangeMainFon;
    procedure BuildLVOld;
    procedure BuildLVNew;
    procedure LabelCountProc;
    procedure BuildHomeButton;
    procedure ChangeState;
  public
    procedure DoProcess;
    property AppPath: string read FAppPath write FAppPath;
    property DeviceList: TDeviceList read FDeviceList;
    property NewDeviceList: TDeviceList read FNewDeviceList;
    property OldDeviceList: TDeviceList read FOldDeviceList;
    property HomeCotrollButton: THomeCotrollButton read FHomeCotrollButton write FHomeCotrollButton;
    constructor Create(CreateSuspended: Boolean);
    destructor Destroy; override;
  end;


implementation

uses
  uMain;

{ TServerProcess }

procedure TServerProcess.OnRequest(Sender: TObject; var ARequest: TFPHTTPConnectionRequest;
    var AResponse: TFPHTTPConnectionResponse);
var
  ADevice: TDevice;
begin
 try
{  logAdd('ARequest.RemoteAddr: [' + ARequest.RemoteAddr +'] ');
  logAdd('ARequest.URI: [' + ARequest.URI +'] ');
  logAdd('----------------------------------------');
  logAdd('ARequest.Content: [' + ARequest.Content +'] ');
  logAdd('----------------------------------------');
}
  if (SameText(ARequest.URI, '/dev.json')) then // post req from job
  begin
    if not ContentToMap(ARequest.Content) then
      Exit;
    logText(FDevList.ToString);
  end;
 {  if IsDevice(ARequest, ADevice) then
   begin
     logAdd('is dev');
   end
   else
   begin
      logAdd('not dev');
   end;   }

 except
   On E:Exception do
   begin
    //  ShowMessage('Ошибка [OnRequest]: ' + E.Message);
      logAdd('Ошибка [OnRequest]: ' + E.Message);
   end;
 end;
end;

function TServerProcess.ContentToMap(AContent: string): Boolean;
var
  JsonParser: TJSONParser;
  JsonObject: TJSONObject;
  jdev: TJSONData;
  JsonEnum: TBaseJSONEnumerator;
  i: integer;
  ADevice: TDevice;
begin
  FDevList.Clear;
  Result := True;
  try
    JsonParser := TJSONParser.Create(AContent, DefaultOptions);
    try
      JsonObject := JsonParser.Parse as TJSONObject;
      try
        JsonEnum := JsonObject.GetEnumerator;
        try
          while JsonEnum.MoveNext do
            if JsonObject.Types[JsonEnum.Current.Key] = jtArray then
               for i:=0 to Pred(TJSONArray(JsonEnum.Current.Value).Count) do
               begin
                 jdev := GetJSON(TJSONArray(JsonEnum.Current.Value).Items[i].AsJSON);
                 ADevice.Ip :=          jdev.FindPath('ip').AsString;
                 ADevice.DeviceClass := jdev.FindPath('class').AsString;
                 ADevice.State :=       jdev.FindPath('state').AsString;
                 ADevice.Name :=        jdev.FindPath('name').AsString;
                 ADevice.GUID :=        jdev.FindPath('device_guid').AsString;
                 ADevice.DeviceIndex := jdev.FindPath('index').AsString;
                 ADevice.IsNewDevice := jdev.FindPath('isnewdevice').AsString;
                 FDevList.Add(ADevice);
               end;
        finally
          FreeAndNil(JsonEnum)
        end;
      finally
        FreeAndNil(JsonObject);
      end;
    finally
      FreeAndNil(JsonParser);
    end;

  except
  On E:Exception do
   begin
      Result := False;
      logAdd('Ошибка [ContentToMap]: ' + E.Message);
   end;
  end;
end;

procedure TServerProcess.logAdd(const AValue: string);
begin
  fdebuginfoAdd := AValue;
  Synchronize(@_logAdd);
end;

procedure TServerProcess._logAdd;
begin
  MainForm.m_device.Clear;
  MainForm.m_device.Lines.Add('[' + TimeToStr(Now) + '] '  + fdebuginfoAdd)
end;

procedure TServerProcess.logText(const AValue: string);
begin
  fdebuginfoText := AValue;
  Synchronize(@_logText);
end;

procedure TServerProcess._logText;
begin
  MainForm.m_device.Lines.Add('[' + TimeToStr(Now) + '] '  + fdebuginfoText)
end;

function TServerProcess.IsDevice(ARequest: TFPHTTPConnectionRequest; var ADevice: TDevice): Boolean;
var
  js: TJSONData;
  AJSONText: string;
begin
  Result := False;

  AJSONText := ARequest.Content;

  Result := pos(c_Device_GUID, AJSONText) > 0; //NewTechDev

  if Result then
    begin
      js := GetJSON(AJSONText);
      try
       ADevice.Ip :=          js.FindPath('ip').AsString;
       ADevice.DeviceClass := js.FindPath('class').AsString;
       ADevice.State :=       js.FindPath('state').AsString;
       ADevice.Name :=        js.FindPath('name').AsString;
       ADevice.GUID :=        js.FindPath('device_guid').AsString;
       ADevice.DeviceIndex := js.FindPath('index').AsString;
       ADevice.IsNewDevice := js.FindPath('isnewdevice').AsString;
       ADevice.srcContent:=   AJSONText;
       { if (SameText(device.IsNewDevice, 'old')) then
         FOldDeviceList.Add(device)
       else
         FNewDeviceList.Add(device);
       FDeviceList.Add(device);    }
      finally
       js.Free;
      end;
    end;
end;

procedure TServerProcess.Execute;
begin
 try
    FServer.Active := True;
  except
    on E: Exception do
    begin
      _Error := E.Message;
    end;
  end;
end;

constructor TServerProcess.Create;
begin
 FServer := TFPHttpServer.Create(nil);
 FServer.port := 8080;
 FServer.threaded := true;
 FServer.OnRequest := @OnRequest;
 FreeOnTerminate := true;
 FDevList := TDeviceList.Create;
 inherited Create(False);
end;

destructor TServerProcess.Destroy;
begin
  FServer.Active:= False;
  FServer.Free;
  FDevList.Free;
  inherited Destroy;
end;

{ TPingProcess }
procedure TPingProcess.Execute;
begin
  while not Terminated do
  begin
     DoProcess;
     sleep(c_execute_timeout);
  end;
end;

procedure TPingProcess.DoProcess;
begin
  DoPing;
  Synchronize(@ShowStatePingOk);
  DoState;
  Synchronize(@ShowStateStateOk);
  DoGUIControll;
end;

procedure TPingProcess.DoPing_nmap;
var
  i, k, l: Integer;
  s_ip, s, d, m: string;
  f: Boolean;
  ip_map, ip_dest_map: TStringList;
  outArp: AnsiString;

begin
  FPingedList.Clear;
 // RunCommand('/arp-scan --localnet', outArp);
    RunCommand('/nmap -sn 192.168.1.1/24', outArp);
    // arp -a
   ip_map := TStringList.Create;
   ip_dest_map := TStringList.Create;
   ip_dest_map.Duplicates:= dupIgnore;
   try

    ip_map.Text := outArp;

      for i := 0 to ip_map.Count - 1 do
      begin
        s := ip_map.Strings[i];
        k := Pos('Nmap scan report for', s);
        if (k > 0) then
        begin
          d := Copy(s, k + Length('Nmap scan report for '), Length(s));
          if Length(d) <= 13 then
          begin
            if ((i + 2) <= ip_map.Count - 1) then
            begin
               m := ip_map.Strings[i + 2];
               if Pos('Unknown', m) > 0 then
               begin
             //    m := Copy(m, Pos('(', m), Length(m) - 1);
                 ip_dest_map.Add(d);
               end;
            end;


        end;

       end;
      end;

      FPingedList.Text:= ip_dest_map.Text ;
   finally
     ip_map.Free;
     ip_dest_map.Free;
   end;

  synchronize(@ShowStatePinged);
end;

procedure TPingProcess.DoPing;
var
  i, k, l: Integer;
  s_ip, s, d, m: string;
  f: Boolean;
  ip_map, ip_dest_map: TStringList;
  outArp: AnsiString;

begin
  FPingedList.Clear;
  RunCommand('./arp.sh', outArp);
  ip_map := TStringList.Create;
  ip_dest_map := TStringList.Create;
  ip_dest_map.Duplicates:= dupIgnore;
  try
    ip_map.Text := outArp;

      for i := 0 to ip_map.Count - 1 do
      begin
        k := Pos(c_Device_GUID, ip_map.Strings[i]);
        if (k > 0) and (i > 0) then
        begin
          s := ip_map.Strings[i - 1];
          d :=  Copy(s, Pos('ip: ',  s) + Length('ip: '), Length(s));
          ip_dest_map.Add(d);
        end;
      end;
      FPingedList.Text:= ip_dest_map.Text ;
   finally
     ip_map.Free;
     ip_dest_map.Free;
     synchronize(@ShowStatePinged);
   end;
end;

procedure TPingProcess.DoState;
var
  ip: string;
begin
  FDeviceList.Clear;
  FOldDeviceList.Clear;
  FNewDeviceList.Clear;
  try
    if FPingedList.Count = 0 then
      Exit;

    for ip in FPingedList do
     CheckDevice(ip);
  finally
    Synchronize(@ShowStateDevices);
  end;
end;

{controll by FOldDeviceList FNewDeviceList}
{
 device api
http://192.168.4.100/state
http://192.168.4.100/relay?turn=on
http://192.168.4.100/set?name=value
http://172.20.10.14/reset
http://172.20.10.14/fullreset

http://192.168.4.100/scan
}
procedure TPingProcess.DoGUIControll;
begin
  Synchronize(@BuildLVOld);
  Synchronize(@BuildLVNew);
  Synchronize(@LabelCountProc);
  Synchronize(@BuildHomeButton);
  Synchronize(@ChangeState);
end;

function TPingProcess.CheckDevice(AValue: string): Boolean;
var
  js: TJSONData;
  addr: string;
  device: TDevice;
  outGet: AnsiString;
begin
  Result := False;
  addr := 'http://' + AValue + c_state;
 // RunCommand('/curl --connect-timeout 10 ' + addr, outGet);
  RunCommand('/curl -m 5 ' + addr, outGet);
  Result := pos(c_Device_GUID, outGet) > 0; //NewTechDev

    if Result then
    begin
      js := GetJSON(outGet);
      try
       fdebuginfo := 'end parse';
       Synchronize(@log);

       device.Ip :=          js.FindPath('ip').AsString;
       device.DeviceClass := js.FindPath('class').AsString;
       device.State :=       js.FindPath('state').AsString;
       device.Name :=        js.FindPath('name').AsString;
       device.GUID :=        js.FindPath('device_guid').AsString;
       device.DeviceIndex := js.FindPath('index').AsString;
       device.IsNewDevice := js.FindPath('isnewdevice').AsString;
       if (SameText(device.IsNewDevice, 'old')) then
         FOldDeviceList.Add(device)
       else
         FNewDeviceList.Add(device);
       FDeviceList.Add(device);
      finally
       js.Free;
      end;
    end;
end;

procedure TPingProcess.ShowStatePinged;
begin
  MainForm.img_state.Picture.LoadFromFile('img/wifi_black.png');
end;

procedure TPingProcess.ShowStateDevices;
begin
  MainForm.img_state.Picture.LoadFromFile('img/wifi_green.png');
end;

procedure TPingProcess.ShowStatePingOk;
begin

end;

procedure TPingProcess.ShowStateStateOk;
begin

end;

procedure TPingProcess.log;
begin

end;

procedure TPingProcess.ChangeMainFon;
begin

end;

procedure TPingProcess.BuildLVOld;
var
  p: TDevice;
  num, iSelIndex, i: Integer;
begin
  iSelIndex := -1;
  if MainForm.lv_Old.Selected <> nil then
    iSelIndex := MainForm.lv_Old.Selected.Index;

    MainForm.lv_Old.BeginUpdate;
    MainForm.lv_Old.Clear;
    num := 0;
    try
//     for i:= 0 to 30 do
      for p in FOldDeviceList.List do
      begin
        Inc(num);
        with MainForm.lv_Old.Items.Add do
        begin
           Caption:= num.ToString + '.';
           if SameText(p.State, 'off') then
             SubItems.Add('⚫')
           else
             SubItems.Add('🔥');

           SubItems.Add(p.Name);
           SubItems.Add(p.Ip);
           SubItems.Add(p.State);
           SubItems.Add(p.IsNewDevice);
        end;
      end;
    finally
       if (iSelIndex < MainForm.lv_Old.Items.Count) and (iSelIndex > -1) then
         MainForm.lv_Old.Selected := MainForm.lv_Old.Items[iSelIndex];
      MainForm.lv_Old.EndUpdate;
    end;
end;

procedure TPingProcess.BuildLVNew;
var
  p: TDevice;
  num, i, iSelIndex: Integer;
begin
  iSelIndex := -1;
  if MainForm.lv_New.Selected <> nil then
    iSelIndex := MainForm.lv_New.Selected.Index;
  MainForm.lv_New.BeginUpdate;
  MainForm.lv_New.Clear;
  num := 0;
  try
//   for i:= 0 to 30 do
    for p in FNewDeviceList.List do
    begin
      Inc(num);
      with MainForm.lv_New.Items.Add do
      begin
         Caption:= num.ToString + '.';
         if SameText(p.State, 'off') then
           SubItems.Add('⚫')
         else
           SubItems.Add('🔥');
         SubItems.Add(p.Name);
         SubItems.Add(p.Ip);
         SubItems.Add(p.State);
         SubItems.Add(p.IsNewDevice);
      end;
    end;
  finally
    if (iSelIndex < MainForm.lv_New.Items.Count) and (iSelIndex > -1) then
       MainForm.lv_New.Selected := MainForm.lv_New.Items[iSelIndex];
    MainForm.lv_New.EndUpdate;
  end;
end;

procedure TPingProcess.LabelCountProc;
begin
  MainForm.lb_cnt_sett.Visible:= FNewDeviceList.Count > 0;
  MainForm.lb_cnt_sett.Caption:= FNewDeviceList.Count.ToString;
end;

procedure TPingProcess.BuildHomeButton;
var
  d: TDevice;
  p: TPanel;
  k, i, iSelIndex: Integer;
begin
  k := 0;

  for p in FHomeCotrollButton do
      p.Free;

  SetLength(FHomeCotrollButton, k);

  for d in FOldDeviceList.List do
  begin
    Inc(k);
    SetLength(FHomeCotrollButton, k);
    FHomeCotrollButton[k - 1] := TPanel.Create(MainForm.MainHomePanel);
    FHomeCotrollButton[k - 1].Align := alTop;
    FHomeCotrollButton[k - 1].Height:= 70;
    FHomeCotrollButton[k - 1].BevelInner:= bvLowered;
    FHomeCotrollButton[k - 1].BevelInner:= bvRaised;
    FHomeCotrollButton[k - 1].Caption:= d.Name;
    FHomeCotrollButton[k - 1].ShowHint:= False;
    FHomeCotrollButton[k - 1].Hint:= '{ip:'+ chr(39) + d.Ip + chr(39) +',state:'
                                             + chr(39) + d.State + chr(39) +'}';
    if SameText('on', d.State) then
       FHomeCotrollButton[k - 1].Color:= clGreen
    else
       FHomeCotrollButton[k - 1].Color:= clWhite;

    FHomeCotrollButton[k - 1].Parent := MainForm.MainHomePanel;
    FHomeCotrollButton[k - 1].OnMouseUp:= @MainForm.OnHomeControllButtonMouseUp;
  end;
end;

procedure TPingProcess.ChangeState;
begin
 with MainForm do
 begin
  sh_sett.Enabled := DeviceList.Count > 0;

  pTabOld.Enabled:= OldDeviceList.Count > 0;
  pTabNew.Enabled:= NewDeviceList.Count > 0;

  if (DeviceList.Count = 0) and (pgc.ActivePage = tsSett) then
  begin
     pgc.ActivePage := tsMain;
     if Assigned(MainForm.SettingsForm) then
        MainForm.SettingsForm.Close;
  end;
  if (OldDeviceList.Count = 0) then
     OnCustomTabLinkClickNew;
  if (NewDeviceList.Count = 0) then
     OnCustomTabLinkClickOLd;
 end;
end;

constructor TPingProcess.Create(CreateSuspended: Boolean);
begin
  inherited Create(False);
   FPingedList := TStringList.Create;
   FDeviceList := TDeviceList.Create;
   FNewDeviceList := TDeviceList.Create;
   FOldDeviceList := TDeviceList.Create;
end;

destructor TPingProcess.Destroy;
begin
  FPingedList.Free;
  FDeviceList.Free;
  FNewDeviceList.Free;
  FOldDeviceList.Free;
  inherited Destroy;
end;

end.

