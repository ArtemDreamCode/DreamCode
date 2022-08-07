
{cc from https://wiki.lazarus.freepascal.org/TList}
unit uTypes;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, process, StdCtrls, ExtCtrls, fgl, Graphics, Controls, fphttpclient, Forms;

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
    IsLife: Byte; // syntetic - if device not ping = false
  end;

type
   TDeviceArrayList = Array of TDevice;

   { TDevButton }

   { TDevButtonController }
   TDevButtonController = class
   private
     FlastTop: Integer;
     procedure t_Get(p_bt: PtrInt); // for BeginThread for AntiFreeze Click Button
   public
      function FindByIp(const ATextTag: string): Boolean;
      procedure Add(const ADevice: TDevice);
      procedure Delete(const ATextTag: string);
      procedure Refresh(const ADevice: TDevice);
      procedure OnBaseDevMouseUp(Sender: TObject; Button: TMouseButton;
                  Shift: TShiftState; X, Y: Integer);
      constructor Create;
   end;

   TDevButton = class(TPanel)
   private
     FState: string;
     FIsLife: Byte;
     function GetIsLife: Byte;
     function GetState: string;
     procedure SetIsLife(AValue: Byte);
     procedure SetState(AValue: string);
   public
     ip: string;
     AllowedToClick: Boolean;
     procedure CustomDrawState(AValue: string);
     property State: string read GetState write SetState;
     property IsLife: Byte read GetIsLife write SetIsLife;
     constructor Create(AOwner: TComponent);
   end;

  { TDeviceList }
   TDeviceList = class(TObject)
   private
      FDeviceArrayList: TDeviceArrayList;
      function GetCount: Integer;
    public
        function Add(ADevice : TDevice) : integer;
        function ToString: string; override;
        function ToShortString: string;
        function GetByIp(AValue: string): TDevice;
        procedure MarkLife(AValue: Byte); overload;
        procedure MarkLife(AIp: string; AValue: Byte); overload;
        function IsContainsByIp(AValue: string): Boolean;
        procedure SetNewState(AIp, AState: string);
        procedure Clear;
        property List: TDeviceArrayList read FDeviceArrayList;
        property Count: Integer read GetCount;
    end;

function show_kb: string;
function hide_kb: string;
var
  FDeviceList, FNewDeviceList, FOldDeviceList: TDeviceList;

implementation

uses
  uMain;

{ TDevButtonController }

function TDevButtonController.FindByIp(const ATextTag: string): Boolean;
var
  i: Integer;
  bt: TDevButton;
begin
  Result := False;
  for i := 0 to MainForm.pBtn.ControlCount - 1 do
  begin
    if not (MainForm.pBtn.Controls[i] is TDevButton) then
       Continue;
    bt := (MainForm.pBtn.Controls[i] as TDevButton);

    if (SameText(bt.ip, ATextTag)) then
      Result := True;
  end;
end;

procedure TDevButtonController.Add(const ADevice: TDevice);
var
  bt: TDevButton;
begin
  bt := TDevButton.Create(MainForm.pBtn);
  bt.OnMouseUp:= @OnBaseDevMouseUp;
  bt.Height:= 50;
  bt.Width:= 200;
  bt.Top:= FlastTop + 10;
  FlastTop := bt.Top + bt.Height;
  bt.Left:= 10;
  bt.BevelInner:= bvLowered;
  bt.BevelOuter:= bvRaised;
  bt.Parent := MainForm.pBtn;
  bt.IsLife:= ADevice.IsLife;
  bt.State:= ADevice.State;
  bt.ip := ADevice.Ip;
  bt.Caption:= ADevice.Ip;
end;

procedure TDevButtonController.Delete(const ATextTag: string);
begin
//
end;

procedure TDevButtonController.Refresh(const ADevice: TDevice);
var
  i: Integer;
  bt: TDevButton;
begin
  for i := 0 to MainForm.pBtn.ControlCount - 1 do
  begin
    if not (MainForm.pBtn.Controls[i] is TDevButton) then
       Continue;
    bt := (MainForm.pBtn.Controls[i] as TDevButton);

    if (SameText(bt.ip, ADevice.Ip)) then
    begin
      bt.IsLife:= ADevice.IsLife;
      bt.State:= ADevice.State;
      bt.ip := ADevice.Ip;
      bt.Caption:= ADevice.Ip;
    end;
  end;
end;

procedure TDevButtonController.t_Get(p_bt: PtrInt);
var
  bt: TDevButton;
  outGet: string;
begin
  bt := TDevButton(p_bt);
  bt.AllowedToClick := False;
  try
    with TFPHttpClient.Create(nil) do
      try
        if (bt.State = 'on') then
          RunCommand('/curl -m 2 http://' + bt.ip + '/relay?turn=off', outGet)
         // Get('http://' + bt.ip + '/relay?turn=off')
        else
          RunCommand('/curl -m 2 http://' + bt.ip + '/relay?turn=on', outGet)
         // Get('http://' + bt.ip + '/relay?turn=on');
      finally
        Free;
      end;

  finally
    bt.AllowedToClick := True;
  end;
end;

procedure TDevButtonController.OnBaseDevMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
  bt: TDevButton;
  outGet: string;
begin
  if not (Sender is TDevButton) then
     Exit;
  bt := (Sender as TDevButton);
  if not (bt.AllowedToClick) then
    Exit;
  with TFPHttpClient.Create(nil) do
    try
      if (bt.State = 'on') then
        bt.CustomDrawState('off')
      else
        bt.CustomDrawState('on');
//    t_Get(PtrInt(Pointer(bt)));
      Forms.Application.QueueAsyncCall(@t_get, PtrInt(Pointer(bt)));
    finally
      Free;
    end;
    bt.Enabled:= TRue;
 end;

constructor TDevButtonController.Create;
begin
  FlastTop := 0;
end;

{ TDevButton }

function TDevButton.GetState: string;
begin
  result := FState;
end;

function TDevButton.GetIsLife: Byte;
begin
  result := FIsLife;
end;

procedure TDevButton.SetIsLife(AValue: Byte);
begin
  if (AValue = 0) then
    self.Enabled:= False
  else
    self.Enabled:= True;
  FIsLife := AValue;
end;

procedure TDevButton.CustomDrawState(AValue: string);
begin
  if (AValue = 'on') then
  begin
    self.Color:= clGreen;
    self.Font.Color := clWhite;
  end
  else
  begin
    self.Color:= clWhite;
    self.Font.Color := clBlack;
  end;
end;

procedure TDevButton.SetState(AValue: string);
begin
  if (AValue = 'on') then
      CustomDrawState('on')
  else
    CustomDrawState('off');
  FState := AValue;
end;

constructor TDevButton.Create(AOwner: TComponent);
begin
  Inherited Create(AOwner);
  self.Color:= clWhite;
  AllowedToClick := True;
end;

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
      s.Add('IsLife: ' + P.IsLife.ToString + '}');
      s.Add('------------------------------------');
    end;
    Result := s.Text;
  finally
    s.Free;
  end;
end;

function TDeviceList.ToShortString: string;
var
  s: TStringList;
  P : TDevice;
begin
  s := TStringList.Create;
  try
    s.Add('Devices count: ' + self.Count.ToString);
    for P in FDeviceArrayList do
    begin
      s.Add('ip: ' + P.Ip);
      s.Add('State: ' + P.State);
      s.Add('Name: ' + P.Name);
      s.Add('isnewdevice: ' + P.isnewdevice + '}');
      s.Add('IsLife: ' + P.IsLife.ToString + '}');
      s.Add('------------------------------------');
    end;
    Result := s.Text;
  finally
    s.Free;
  end;
end;

function TDeviceList.GetByIp(AValue: string): TDevice;
var
  elem: TDevice;
begin
  for elem in FDeviceArrayList do
      if SameText(elem.Ip, AValue) then
         Exit(elem);
end;

procedure TDeviceList.MarkLife(AValue: Byte);
var
  i: Integer;
begin
  for i := 0 to Count - 1 do
      FDeviceArrayList[i].IsLife := AValue;
end;

procedure TDeviceList.MarkLife(AIp: string; AValue: Byte);
var
  i: Integer;
begin
  for i := 0 to Count - 1 do
      if SameText(FDeviceArrayList[i].Ip, AIp) then
         FDeviceArrayList[i].IsLife:= AValue;
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

