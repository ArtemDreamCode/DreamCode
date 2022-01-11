
{cc from https://wiki.lazarus.freepascal.org/TList}
unit uTypes;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, process;

type
 //PDevice = ^TDevice;
 TDevice = packed record
    Ip: string;
    DeviceClass: string;
    State: string;
    Name: string;
    GUID: string;
    DeviceIndex: string;
    IsNewDevice: string;
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
        procedure Clear;
        property List: TDeviceArrayList read FDeviceArrayList;
        property Count: Integer read GetCount;
        constructor Create;
    end;


function show_kb: string;
function hide_kb: string;


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

procedure TDeviceList.Clear;
begin
  SetLength(FDeviceArrayList, 0);
end;

constructor TDeviceList.Create;
begin

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

