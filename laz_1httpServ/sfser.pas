unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, fphttpserver;

type

  { TTMainForm }

  TTMainForm = class(TForm)
    Bstart: TButton;
    Bstop: TButton;
    MLog: TMemo;
    procedure BstartMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure BstopMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
  private

  public

  end;

var
  TMainForm: TTMainForm;

implementation

{$R *.lfm}

{ TTMainForm }

procedure TTMainForm.BstartMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TTMainForm.BstopMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TTMainForm.FormCreate(Sender: TObject);
begin
end;

THTTPServerThread = Class(TThread)
Private
  FServer : TFPHTTPServer;
Public
  Constructor Create(APort : Word;
    Const OnRequest : THTTPServerRequestHandler);
  Procedure Execute; override;
  Procedure DoTerminate; override;
  Property Server : TFPHTTPServer Read FServer;
end;

constructor THTTPServerThread.Create(APort: Word;
  const OnRequest: THTTPServerRequestHandler);
begin
  FServer:=TFPHTTPServer.Create(Nil);
  FServer.Port:=APort;
  FServer.OnRequest:=OnRequest;
  Inherited Create(False);
end;

procedure THTTPServerThread.Execute;
begin
  try
    FServer.Active:=True;
  Finally
    FreeAndNil(FServer);
  end;
end;

end.
