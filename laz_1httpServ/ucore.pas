unit uCore;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
   process, fpjson,
   jsonparser, jsonscanner, uTypes, ExtCtrls, StdCtrls, Controls, Graphics,
   fphttpserver, httpdefs, httproute;

type
  { TServerProcess }
  TServerProcess  = class(TThread)
  private
    _Error: string;
    FServer: TFPHttpServer;
  // route
    procedure OnRequest(Sender: TObject; Var ARequest: TFPHTTPConnectionRequest; Var AResponse : TFPHTTPConnectionResponse);
  protected
    procedure Execute; override;
  public
    procedure StopResource;
    constructor Create(CreateSuspended: Boolean);
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

procedure TServerProcess.OnRequest(Sender: TObject;
  var ARequest: TFPHTTPConnectionRequest;
  var AResponse: TFPHTTPConnectionResponse);
begin
 if ARequest.QueryFields.Values['turn'] > '' then
 begin
   // change state ARequest.RemoteAddress device
   AResponse.Contents.Add(ARequest.RemoteAddress + ' state: ' + ARequest.QueryFields.Values['turn'] + '\n');
   AResponse.Contents.Add(':)');
 end;
 if ARequest.QueryFields.Values['state'] > '' then
 begin
   if ARequest.QueryFields.Values['state'] = 'all' then
   begin
        //todo states of all devices
     AResponse.Contents.Add('State of all devices :)');
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

procedure TServerProcess.StopResource;
begin
 FServer.Active:= False;
end;

constructor TServerProcess.Create(CreateSuspended: Boolean);
begin
 FServer := TFPHttpServer.Create(nil);
 FServer.port := 8080;
 FServer.threaded := true;
 FServer.OnRequest := @OnRequest;
 FreeOnTerminate := true;
 inherited Create(False);
 // FServer.Active:= true;
//  FServer.OnAllowConnect:= ;
end;

destructor TServerProcess.Destroy;
begin
 // FServer.Free;
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
  MainForm.m_state.Lines.Assign(FPingedList);
  MainForm.m_state.Lines.Add(DateTimeToStr(Now));
  MainForm.m_state.Lines.Add('=================');
  MainForm.img_state.Picture.LoadFromFile('img/wifi_black.png');
end;

procedure TPingProcess.ShowStateDevices;
begin
  MainForm.m_device.Clear;
  MainForm.m_device.Lines.Add(FDeviceList.ToString);
  MainForm.m_device.Lines.Add(DateTimeToStr(Now));
  MainForm.m_device.Lines.Add('=================');
  MainForm.img_state.Picture.LoadFromFile('img/wifi_green.png');
end;

procedure TPingProcess.ShowStatePingOk;
begin
  MainForm.m_all_proc.Clear;
  MainForm.m_all_proc.Lines.Add('PingOk');
  MainForm.m_all_proc.Lines.Add(DateTimeToStr(Now));
  MainForm.m_all_proc.Lines.Add('=================');
end;

procedure TPingProcess.ShowStateStateOk;
begin
  MainForm.m_all_proc.Clear;
  MainForm.m_all_proc.Lines.Add('StateOk');
  MainForm.m_all_proc.Lines.Add(DateTimeToStr(Now));
  MainForm.m_all_proc.Lines.Add('=================');
end;

procedure TPingProcess.log;
begin
  MainForm.m_device.Lines.Add('log ok: ' + fdebuginfo)
end;

procedure TPingProcess.ChangeMainFon;
begin
{  MainForm.lb_num_sett.Visible := (FNewDeviceList.Count > 0);
  MainForm.lb_num_sett.Caption := FNewDeviceList.Count.ToString;
  MainForm.img_main.Picture.LoadFromFile(FChangeMainFon);
  MainForm.lb_num_sett.BringToFront; }
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

