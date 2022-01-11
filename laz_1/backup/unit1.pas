unit uMain;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  ComCtrls, Arrow,
  fgl, fphttpclient, process, fpjson, fpjsonrtti, fpjsontopas,
  jsonconf, jsonparser, jsonscanner, uTypes, Types, uCore;


  { TForm1 }
type
  TForm1 = class(TForm)
    Arrow1: TArrow;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    ListBox1: TListBox;
    m_state: TMemo;
    m_device: TMemo;
    m_all_proc: TMemo;
    PageControl1: TPageControl;
    Shape1: TShape;
    Shape2: TShape;
    Shape3: TShape;
    Shape4: TShape;
    tsMain: TTabSheet;
    tsSett: TTabSheet;
    tsControll: TTabSheet;
    tsToDo: TTabSheet;
    tsDebug: TTabSheet;
    procedure Shape1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure AfterConstruction; override;
    procedure BeforeDestruction; override;
  private
    FPingProcess: TPingProcess;
  protected
    procedure DoShow; override;
    procedure RunProcessPing;
    procedure KillProcessPing;
  end;

var
  MainForm: TForm1;

implementation

{$R *.lfm}


{ TForm1 }

procedure TForm1.Shape1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  showmessage(Shape1.Name);
end;

procedure TForm1.AfterConstruction;
begin
  inherited AfterConstruction;
    Color := RGBToColor(239, 239, 244);
end;

procedure TForm1.BeforeDestruction;
begin
  KillProcessPing;
  inherited BeforeDestruction;
end;

procedure TForm1.DoShow;
begin
  inherited DoShow;
  RunProcessPing;
end;

procedure TForm1.RunProcessPing;
begin
  if Assigned(FPingProcess) then
     FPingProcess.Terminate;
   FPingProcess := TPingProcess.Create(True);
   FPingProcess.Priority := tpLower;
end;

procedure TForm1.KillProcessPing;
begin
  FPingProcess.Terminate;
  FPingProcess.WaitFor;
  FreeAndNil(FPingProcess);
end;


end.

