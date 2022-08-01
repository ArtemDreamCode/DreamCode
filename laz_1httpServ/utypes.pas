
{cc from https://wiki.lazarus.freepascal.org/TList}
unit uTypes;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, process, StdCtrls, ExtCtrls, fgl;

const
  c_ping_count   = '1';
  c_ping_timeout = '1';
  c_base_mask = '192.168.0.';
  c_min_range = 100;
  c_max_range = 125;
  c_execute_timeout = 7000;
  c_is_ok: array[Boolean] of string = ('Bad', 'Ok');
  c_state = '/state';
  c_turn_on = '/relay?turn=on';
  c_turn_off = '/relay?turn=off';
  c_change_name = '/set?name=';
  c_reset = '/reset';
  c_fullreset = '/fullreset';
  c_Device_GUID = 'NewDev';
  c_MaxDevice = 20;
  c_DbName = 'db_dev';
type

 THomeCotrollButton = array of TPanel;

 TDevice = packed record
    Ip: string;
    DeviceClass: string;
    State: string;
    Name: string;
    GUID: string;
    DeviceIndex: string;
    IsNewDevice: string;
    srcContent: string;
  end;

type
   TDeviceArrayList = Array of TDevice;

  { TDeviceList }
   TDeviceList = class(TObject)
   private
      FDeviceArrayList: TDeviceArrayList;
      function GetCount: Integer;
    public
        function Add(ADevice : TDevice) : integer;
        function ToString: string; override;
        function SearchByIp(AValue: string): TDevice;
        function IsContainsByIp(AValue: string): Boolean;
        procedure SetNewState(AIp, AState: string);
        procedure Clear;
        property List: TDeviceArrayList read FDeviceArrayList;
        property Count: Integer read GetCount;
        constructor Create;
    end;

function DeviceToStr(ADevice: TDevice): string;
function show_kb: string;
function hide_kb: string;
var
  FDeviceList, FNewDeviceList, FOldDeviceList: TDeviceList;

implementation

{ TDeviceList }

function TDeviceList.GetCount: Integer;
begin
  result := Length(FDeviceArrayList);
end;

function TDeviceList.Add(ADevice: TDevice): integer;
var
  c: integer;
begin
  c := Length(FDeviceArrayList);
  SetLength(FDeviceArrayList, c + 1);
  FDeviceArrayList[c] := ADevice;
  Result := c;
end;

function TDeviceList.ToString: string;
var
  s: TStringList;
  P : TDevice;
begin
  s := TStringList.Create;
  try
    s.Add('Devices count: ' + self.Count.ToString);
    for P in FDeviceArrayList do
    begin
      s.Add('{ ip: ' + P.Ip);
      s.Add('DeviceClass: ' + P.DeviceClass);
      s.Add('State: ' + P.State);
      s.Add('Name: ' + P.Name);
      s.Add('GUID: ' + P.GUID);
      s.Add('DeviceIndex: ' + P.DeviceIndex);
      s.Add('isnewdevice: ' + P.isnewdevice + '}');
      s.Add('------------------------------------');
    end;
    Result := s.Text;
  finally
    s.Free;
  end;
end;

function TDeviceList.SearchByIp(AValue: string): TDevice;
var
  elem: TDevice;
begin
  for elem in FDeviceArrayList do
      if SameText(elem.Ip, AValue) then
         Exit(elem);
end;

function TDeviceList.IsContainsByIp(AValue: string): Boolean;
var
  elem: TDevice;
begin
  Result := False;
  for elem in FDeviceArrayList do
      if SameText(elem.Ip, AValue) then
         Exit(True);
end;

procedure TDeviceList.SetNewState(AIp, AState: string);
var
  i: Integer;
  NewState: string;
begin
  NewState := Copy(AState, 2, Length(AState));
  for i := 0 to Count - 1 do
      if SameText(FDeviceArrayList[i].Ip, AIp) then
         FDeviceArrayList[i].State:= NewState;
end;

procedure TDeviceList.Clear;
begin
  SetLength(FDeviceArrayList, 0);
end;

constructor TDeviceList.Create;
begin

end;

function DeviceToStr(ADevice: TDevice): string;
var
  s: TStringList;
begin
  s := TStringList.Create;
  try
    s.Add('{ ip: ' + ADevice.Ip);
    s.Add('DeviceClass: ' + ADevice.DeviceClass);
    s.Add('State: ' + ADevice.State);
    s.Add('Name: ' + ADevice.Name);
    s.Add('GUID: ' + ADevice.GUID);
    s.Add('DeviceIndex: ' + ADevice.DeviceIndex);
    s.Add('isnewdevice: ' + ADevice.isnewdevice + '}');
    s.Add('------------------------------------');
    Result := s.Text;
  finally
    s.Free;
  end;
end;

function show_kb: string;
begin
  with tprocess.Create(nil) do
  begin
    try
     CommandLine:= 'gsettings set org.gnome.desktop.a11y.applications screen-keyboard-enabled true';
     Execute;
     beep;
    finally
      Free;
    end;
  end;
//  RunCommand('/onboard',result);
//  if RunCommand('/gsettings set org.gnome.desktop.a11y.applications screen-keyboard-enabled true ',result) then
//     beep;
end;

function hide_kb: string;
begin
  RunCommand('/gsettings set org.gnome.desktop.a11y.applications screen-keyboard-enabled false ',result);
end;


end.

