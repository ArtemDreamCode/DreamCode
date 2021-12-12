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
  System.Actions, Vcl.ActnList, dxGDIPlusClasses, cxLabel, Generics.Collections;

type
  TIPList = class(TList<string>);
  TDeviceList = class(TList<string>);

type TPingProcess = class(TThread)
  const
    c_base_mask = '192.168.0.';
    c_min_range = 100;
    c_max_range = 107;
    c_proc_delay = 5000;
    c_is_ok: array[Boolean] of string = ('Bad', 'Ok');
  strict private
    fHt: TIdHTTP;
    fSsl: TIdSSLIOHandlerSocketOpenSSL;
  private
    FIPList: TIPList;
    FDeviceList: TDeviceList;
  private
    procedure DoPing;
  protected
    procedure Execute; override;
  public
    constructor Create(CreateSuspended: Boolean);
    destructor  Destroy; override;
  end;


type
  TfrMain = class(TForm)
    m: TMemo;
    m_ip: TMemo;
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
//  FPingProcess.WaitFor;
  FreeAndNil(FPingProcess);
end;

{ TPingProcess }

constructor TPingProcess.Create(CreateSuspended: Boolean);
begin
  inherited Create(False);
  fHt := TIdHTTP.Create;
  fSsl := TIdSSLIOHandlerSocketOpenSSL.Create;
  FIPList := TIPList.Create;
  FDeviceList := TDeviceList.Create;
  fSsl.IPVersion := Id_IPv6;
end;

destructor TPingProcess.Destroy;
begin
  fHt.Free;
  fSsl.Free;
  FIPList.Free;
  FDeviceList.Free;
  inherited Destroy;
end;

procedure TPingProcess.DoPing;
var
  f: Boolean;
  i: Integer;
  s_ip: string;
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
      FIPList.Add(s_ip);
      Synchronize(procedure
               var
                elem: string;
               begin
                 frMain.m_ip.Clear;
                  for elem in FIPList do
                    frMain.m_ip.Lines.Add(elem);
             end);
    end;
  end;

end;

procedure TPingProcess.Execute;
begin
 while not Terminated do
    begin
      DoPing;
      sleep(c_proc_delay);
    end
end;

end.
