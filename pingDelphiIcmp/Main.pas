unit Main;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls,
  cxGraphics, cxControls,
  cxLookAndFeels, cxLookAndFeelPainters, dxSkinsCore, dxSkinBlack, dxSkinBlue,
  dxSkinBlueprint, dxSkinsDefaultPainters, cxContainer, cxEdit, Vcl.ExtCtrls,
  cxTextEdit, dxTileControl, dxCustomTileControl, dxTileBar, cxClasses,
  IdServerIOHandler, IdSSL, IdSSLOpenSSL,
  IdBaseComponent, IdComponent, IdTCPConnection, IdTCPClient, IdHTTP, idGlobal, DBXJSON, System.JSON, REST.Json,
  System.Actions, Vcl.ActnList, dxGDIPlusClasses, cxLabel, Generics.Collections, IdAntiFreezeBase, Vcl.IdAntiFreeze;

type

  TDevice = packed record
    Ip: string;
    DeviceClass: string;
    State: string;
    Name: string;
    GUID: string;
    DeviceIndex: Integer;
    IsNewDevice: Boolean;
  end;


  TIPList = class(TList<TDevice>);
  TDeviceList = class(TList<TDevice>);

type TPingProcess = class(TThread)
  const
    c_base_mask = '192.168.0.';
    c_min_range = 100;
    c_max_range = 107;
    c_execute_timeout = 2500;
    c_id_connect_timeout = 1500;
    c_is_ok: array[Boolean] of string = ('Bad', 'Ok');
    c_state = '/state';
    c_Device_GUID = 'dDf5FFShellysde';
  strict private
    fHt: TIdHTTP;
    fAntiFreeze: TIdAntiFreeze;
    fSsl: TIdSSLIOHandlerSocketOpenSSL;
  private
    FIPList: TIPList;
    FDeviceList: TDeviceList;
  private
    procedure DoPing;
    procedure DoState;
    function IsDevice(AValue: TDevice): Boolean;
  protected
    procedure Execute; override;
  public
    constructor Create(CreateSuspended: Boolean);
    destructor Destroy; override;
  end;


type
  TfrMain = class(TForm)
    m: TMemo;
    m_ip: TMemo;
    m_dev: TMemo;
    procedure AfterConstruction; override;
    procedure BeforeDestruction; override;
  private
    FPingProcess: TPingProcess;
  protected
    procedure DoShow; override;
    procedure RunProcessPing;
    procedure KillProcessPing;
  public
    { Public declarations }
  end;

var
  frMain: TfrMain;

implementation

{$R *.dfm}

uses PingUnits;

procedure TfrMain.AfterConstruction;
begin
  inherited;
{$IFDEF DEBUG}
  ReportMemoryLeaksOnShutdown := true;
{$ENDIF}
end;

procedure TfrMain.BeforeDestruction;
begin
  inherited;
  KillProcessPing;
end;

procedure TfrMain.DoShow;
begin
  inherited;
  RunProcessPing;
end;

procedure TfrMain.RunProcessPing;
begin
 if Assigned(FPingProcess) then
    FPingProcess.Terminate;
  FPingProcess := TPingProcess.Create(True);
  FPingProcess.Priority := tpLower;
end;

procedure TfrMain.KillProcessPing;
begin
  FPingProcess.Terminate;
  FPingProcess.WaitFor;
  FreeAndNil(FPingProcess);
end;

{ TPingProcess }

constructor TPingProcess.Create(CreateSuspended: Boolean);
begin
  inherited Create(False);
  fHt := TIdHTTP.Create;
  fHt.ConnectTimeout := c_id_connect_timeout;
  fSsl := TIdSSLIOHandlerSocketOpenSSL.Create;
  fAntiFreeze := TIdAntiFreeze.Create;
  FIPList := TIPList.Create;
  FDeviceList := TDeviceList.Create;
  fSsl.IPVersion := Id_IPv6;
end;

destructor TPingProcess.Destroy;
begin
  fHt.Free;
  fSsl.Free;
  fAntiFreeze.Free;
  FIPList.Free;
  FDeviceList.Free;
  inherited Destroy;
end;

procedure TPingProcess.DoPing;
var
  f: Boolean;
  i: Integer;
  s_ip: string;
  Device: TDevice;
begin
  FIPList.Clear;
  for i := c_min_range to c_max_range do
  begin
    if Terminated then
      Break;
    s_ip := c_base_mask + i.ToString;
    f := Ping(s_ip);
    Synchronize(procedure begin
             frMain.m.Lines.Add(s_ip + ':' + c_is_ok[f]);
             end);
    if f then
    begin
      Device.Ip := s_ip;
      FIPList.Add(Device);

      Synchronize(procedure
               var
                el: TDevice;
                begin
                 frMain.m_ip.Clear;
                  for el in FIPList do
                    frMain.m_ip.Lines.Add(el.Ip);
                end);
    end;
  end;
end;

procedure TPingProcess.DoState;
var
  elem: TDevice;
begin
  if FIPList.Count = 0 then
    Exit;

 // no override collect
 { for elem in FIPList do
    if (IsDevice(elem)) and (FDeviceList.IndexOf(elem) = - 1) then
      FDeviceList.Add(elem);

  for elem in FDeviceList do
    if FIPList.IndexOf(elem) = - 1 then
      FDeviceList.Remove(elem);
  }

// override collect
  FDeviceList.Clear;
  for elem in FIPList do
    if IsDevice(elem) then
      FDeviceList.Add(elem);

   Synchronize(procedure
               var
                el: TDevice;
                begin
                 frMain.m_dev.Clear;
                  for el in FDeviceList do
                    frMain.m_dev.Lines.Add(el.Ip);
                end);
//      FDeviceList.Parse(elem);
end;

procedure TPingProcess.Execute;
begin
 while not Terminated do
    begin
      DoPing;
      DoState;
      sleep(c_execute_timeout);
    end
end;

function TPingProcess.IsDevice(AValue: TDevice): Boolean;
var
  Json: string;
  sResponse: string;
  mj: TJSONObject;
  s, r: string;
begin
  Result := False;
  s := 'http://' + AValue.Ip + c_state;
  try
    r := fHt.Get(s);
    Result := pos(c_Device_GUID, r) > 0;
  except
    on E: Exception do
  end;
end;

end.
