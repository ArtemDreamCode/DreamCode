unit uCore;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
   process, fpjson,
   jsonparser, jsonscanner, uTypes, ExtCtrls;

{ TPingProcess }
type
  TPingProcess = class(TThread)
  const
    c_ping_count   = '1';
    c_ping_timeout = '1';
    c_base_mask = '192.168.0.';
    c_min_range = 100;
    c_max_range = 125;
    c_execute_timeout = 5000;
    c_is_ok: array[Boolean] of string = ('Bad', 'Ok');
    c_state = '/state';
    c_Device_GUID = 'dDf5FFShellysde';
  private
    FChangeMainFon: string;
    FDeviceList, FNewDeviceList, FOldDeviceList: TDeviceList;
    FPingedList: TStringList;
  private
    FAppPath: string;
    fdebuginfo: string;
// core info
    procedure DoPing;
    procedure DoState;
    procedure DoControll;
    function CheckDevice(AValue: string): Boolean;
  protected
    procedure Execute; override;
// debug info
    procedure ShowStatePinged;
    procedure ShowStateDevices;
    procedure ShowStatePingOk;
    procedure ShowStateStateOk;
    procedure log;
// interface info
    procedure ChangeMainFon;
    procedure BuildLVOld;
    procedure BuildLVNew;
    procedure LabelCountProc;
    procedure ChangeState;
  public
    property AppPath: string read FAppPath write FAppPath;
    property DeviceList: TDeviceList read FDeviceList;
    property NewDeviceList: TDeviceList read FNewDeviceList;
    property OldDeviceList: TDeviceList read FOldDeviceList;
    constructor Create(CreateSuspended: Boolean);
    destructor Destroy; override;
  end;


implementation

uses
  uMain;

{ TPingProcess }
procedure TPingProcess.Execute;
begin
  while not Terminated do
  begin
     DoPing;
     Synchronize(@ShowStatePingOk);
     DoState;
     Synchronize(@ShowStateStateOk);
     DoControll;
     sleep(c_execute_timeout);
  end;
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
 // RunCommand('/arp-scan --localnet', outArp);
    RunCommand('/nmap -sn 172.18.44.1/24', outArp);
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

procedure TPingProcess.DoState;
var
  ip: string;
begin
  if FPingedList.Count = 0 then
    Exit;
 FDeviceList.Clear;
 FOldDeviceList.Clear;
 FNewDeviceList.Clear;

 for ip in FPingedList do
  CheckDevice(ip);

 Synchronize(@ShowStateDevices);
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
procedure TPingProcess.DoControll;
begin
  Synchronize(@BuildLVOld);
  Synchronize(@BuildLVNew);
  Synchronize(@LabelCountProc);
  Synchronize(@ChangeState);

 {  if (FDeviceList.Count = 0) then
     FChangeMainFon := FAppPath + 'img/fon_iot.jpg'
   else
   if (FOldDeviceList.Count > 0) and (FNewDeviceList.Count > 0) then
     FChangeMainFon := FAppPath + 'img/fon_iot_all_balls.jpg'
   else
   if (FOldDeviceList.Count > 0) and (FNewDeviceList.Count = 0) then
     FChangeMainFon := FAppPath + 'img/fon_iot_dev.jpg'
   else
   if (FOldDeviceList.Count = 0) and (FNewDeviceList.Count > 0) then
     FChangeMainFon := FAppPath + 'img/fon_iot_sett.jpg'
   else
     FChangeMainFon := FAppPath + 'img/fon_iot.jpg';

   Synchronize(@ChangeMainFon); }

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
  RunCommand('/curl ' + addr, outGet);
  Result := pos(c_Device_GUID, outGet) > 0;

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
     for i:= 0 to 30 do
      for p in FOldDeviceList.List do
      begin
        Inc(num);
        with MainForm.lv_OLd.Items.Add do
        begin
           Caption:= num.ToString + '.';
           if SameText(p.State, 'off') then
             SubItems.Add('⚫')
           else
             if (num mod 2 = 0) then
                SubItems.Add('🔥')
             else
                SubItems.Add('⚫');
           SubItems.Add(p.Name);
           SubItems.Add(p.Ip);
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
  if MainForm.lv_Old.Selected <> nil then
    iSelIndex := MainForm.lv_Old.Selected.Index;
  MainForm.lv_Old.BeginUpdate;
  MainForm.lv_New.Clear;
  num := 0;
  try
   for i:= 0 to 30 do
    for p in FNewDeviceList.List do
    begin
      Inc(num);
      with MainForm.lv_New.Items.Add do
      begin
         Caption:= num.ToString + '.';
         if SameText(p.State, 'off') then
           SubItems.Add('⚫')
         else
           if (num mod 2 = 0) then
              SubItems.Add('🔥')
           else
              SubItems.Add('⚫');
         SubItems.Add(p.Name);
         SubItems.Add(p.Ip);
      end;
    end;
  finally
    if (iSelIndex < MainForm.lv_Old.Items.Count) and (iSelIndex > -1) then
       MainForm.lv_Old.Selected := MainForm.lv_Old.Items[iSelIndex];
    MainForm.lv_Old.EndUpdate;
  end;
end;

procedure TPingProcess.LabelCountProc;
begin
  MainForm.lb_cnt_sett.Visible:= FNewDeviceList.Count > 0;
  MainForm.lb_cnt_sett.Caption:= FNewDeviceList.Count.ToString;
end;

procedure TPingProcess.ChangeState;
begin
 with MainForm do
 begin
  sh_sett.Enabled := DeviceList.Count > 0;

  pTabOld.Enabled:= OldDeviceList.Count > 0;
  pTabNew.Enabled:= NewDeviceList.Count > 0;

  if (DeviceList.Count = 0) and (pgc.ActivePage = tsSett) then
     pgc.ActivePage := tsMain;
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

