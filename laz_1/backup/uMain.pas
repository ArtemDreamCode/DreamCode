unit uMain;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  ComCtrls, Arrow, EditBtn, Calendar, Grids, Buttons, ListViewFilterEdit,
  DividerBevel, CheckBoxThemed, uCore, uTypes, Types, uSett;


  { TForm1 }
type
  TForm1 = class(TForm)
    img_state: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    lb_cnt_sett: TLabel;
    lb_date: TLabel;
    lb_num_sett: TLabel;
    lb_time: TLabel;
    lv_old: TListView;
    lv_new: TListView;
    m: TMemo;
    m_state: TMemo;
    m_device: TMemo;
    m_all_proc: TMemo;
    Panel27: TPanel;
    Panel28: TPanel;
    Panel29: TPanel;
    Panel30: TPanel;
    Panel31: TPanel;
    Panel32: TPanel;
    Panel33: TPanel;
    Panel34: TPanel;
    Panel35: TPanel;
    Panel36: TPanel;
    Panel37: TPanel;
    Panel38: TPanel;
    Panel39: TPanel;
    Panel40: TPanel;
    Panel41: TPanel;
    Panel42: TPanel;
    Panel43: TPanel;
    Panel44: TPanel;
    Panel45: TPanel;
    Panel46: TPanel;
    Panel47: TPanel;
    Panel48: TPanel;
    pTabNew: TPanel;
    pTabOld: TPanel;
    pgc_dev: TPageControl;
    Panel10: TPanel;
    Panel11: TPanel;
    Panel12: TPanel;
    Panel13: TPanel;
    Panel14: TPanel;
    Panel15: TPanel;
    Panel16: TPanel;
    Panel17: TPanel;
    Panel18: TPanel;
    Panel19: TPanel;
    Panel2: TPanel;
    Panel20: TPanel;
    Panel21: TPanel;
    Panel22: TPanel;
    Panel23: TPanel;
    Panel24: TPanel;
    Panel25: TPanel;
    Panel26: TPanel;
    Panel3: TPanel;
    Panel4: TPanel;
    Panel5: TPanel;
    Panel6: TPanel;
    Panel7: TPanel;
    Panel8: TPanel;
    Panel9: TPanel;
    pgc: TPageControl;
    Panel1: TPanel;
    ScrollBox1: TScrollBox;
    pShOld: TShape;
    pShNew: TShape;
    sh_sett: TShape;
    sh_home: TShape;
    Shape3: TShape;
    Shape4: TShape;
    ts_old: TTabSheet;
    ts_new: TTabSheet;
    tm_time: TTimer;
    tsMain: TTabSheet;
    tsSett: TTabSheet;
    tsControll: TTabSheet;
    tsToDo: TTabSheet;
    tsDebug: TTabSheet;
    procedure lv_oldMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure lv_oldMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer
      );
    procedure lv_oldMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure OnCustomTabLinkClickNew;
    procedure OnCustomTabLinkClickOld;
    procedure AfterConstruction; override;
    procedure Arrow1Click(Sender: TObject);
    procedure BeforeDestruction; override;
    procedure FormCreate(Sender: TObject);
    procedure Panel1Click(Sender: TObject);
    procedure Panel27Click(Sender: TObject);
    procedure Panel30Click(Sender: TObject);
    procedure Panel30MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Panel30MouseWheelDown(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure Panel31MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Panel37Click(Sender: TObject);
    procedure Panel3MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pgc_devChange(Sender: TObject);
    procedure pTabNewMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pTabOldClick(Sender: TObject);
    procedure pTabOldMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Shape3MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure sh_settMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure sh_homeMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure tm_timeTimer(Sender: TObject);
    procedure tsSettShow(Sender: TObject);
  private
    FPingProcess: TPingProcess;
    F_lv_old_Down, F_lv_new_Down: Boolean;
    dy: Integer;
  protected
    procedure ShowTime;
    procedure DoShow; override;
    procedure RunProcessPing;
    procedure KillProcessPing;
  public
    property PingProcessThread: TPingProcess read FPingProcess write FPingProcess;
  end;

var
  MainForm: TForm1;

implementation

{$R *.lfm}


{ TForm1 }

procedure TForm1.AfterConstruction;
begin
  inherited AfterConstruction;
//  Color := RGBToColor(239, 239, 244);
end;

procedure TForm1.Arrow1Click(Sender: TObject);
begin
  pgc.ActivePage := tsMain;
end;

procedure TForm1.BeforeDestruction;
begin
  KillProcessPing;
  inherited BeforeDestruction;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  ShowTime;
  pgc.ActivePage := tsMain;
  pgc_dev.ShowTabs:= False;
  pgc.ShowTabs:= False;
  MainForm.BorderStyle:= bsNone;
  MainForm.Position:= poDesktopCenter;
end;

procedure TForm1.Panel1Click(Sender: TObject);
begin

end;

procedure TForm1.Panel27Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.Panel30Click(Sender: TObject);
begin

end;

procedure TForm1.Panel30MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   dev_mode: TDeviceMode;
   indx: Integer;
   lv: TListView;
   ip: string;
   glist: TDeviceList;
begin

  if pgc_dev.ActivePage = ts_old then
  begin
    dev_mode := dmOld;
    lv := lv_old;
    glist := MainForm.PingProcessThread.NewDeviceList;
  end
  else begin
    dev_mode := dmNew;
    lv := lv_New;
    glist := MainForm.PingProcessThread.OldDeviceList;
  end;

  if not Assigned(lv) then
     Exit;

  indx := lv.Selected.Index;
  ip := lv.Selected.SubItems[2];
  if uSett.Execute(glist, ip) then;
end;

procedure TForm1.Panel30MouseWheelDown(Sender: TObject; Shift: TShiftState;
  MousePos: TPoint; var Handled: Boolean);
begin
end;

procedure TForm1.Panel31MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
end;

procedure TForm1.Panel37Click(Sender: TObject);
begin

end;

procedure TForm1.OnCustomTabLinkClickNew;
begin
  pgc_dev.ActivePage := ts_new;
  pShNew.Pen.Width:= 3;
  //------------------
  pShOld.Pen.Width:= 1;
end;

procedure TForm1.lv_oldMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if F_lv_old_Down then
    lv_old.ScrollBy_WS(0, (y - dy));
  dy := y;
end;

procedure TForm1.lv_oldMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  F_lv_old_Down := True;
end;

procedure TForm1.lv_oldMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
   F_lv_old_Down := False;
end;

procedure TForm1.OnCustomTabLinkClickOld;
begin
  pgc_dev.ActivePage := ts_old;
  pShOLd.Pen.Width:= 3;
  //------------------
  pShNew.Pen.Width:= 1;
end;

procedure TForm1.Panel3MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pgc.ActivePage := tsMain;
end;

procedure TForm1.pgc_devChange(Sender: TObject);
begin
  if pgc_dev.ActivePage = ts_old then
     OnCustomTabLinkClickOld
  else
     OnCustomTabLinkClickNew;
end;

procedure TForm1.pTabNewMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  OnCustomTabLinkClickNew;
end;

procedure TForm1.pTabOldClick(Sender: TObject);
begin

end;

procedure TForm1.pTabOldMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  OnCustomTabLinkClickOld;
end;

procedure TForm1.Shape3MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pgc.ActivePage := tsDebug;
end;

procedure TForm1.sh_settMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pgc.ActivePage := tsSett;
end;

procedure TForm1.ShowTime;
begin
  lb_date.Caption:= DateToStr(Now);
  lb_time.Caption:= TimeToStr(Now);
end;

procedure TForm1.sh_homeMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pgc.ActivePage := tsControll;
end;

procedure TForm1.tm_timeTimer(Sender: TObject);
begin
  ShowTime;
end;

procedure TForm1.tsSettShow(Sender: TObject);
begin
  // event on show tabs !

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
   FPingProcess.AppPath := ExtractFilePath(Application.ExeName);
end;

procedure TForm1.KillProcessPing;
begin
  FPingProcess.Terminate;
  FPingProcess.WaitFor;
  FreeAndNil(FPingProcess);
end;

end.

