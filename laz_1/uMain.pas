unit uMain;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  ComCtrls, Arrow, EditBtn, Calendar, Grids, Buttons, ListViewFilterEdit,
  DividerBevel, CheckBoxThemed, uCore, uTypes, Types, uSett, fpjson, process;


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
    m_state: TMemo;
    m_device: TMemo;
    m_all_proc: TMemo;
    PageControl2: TPageControl;
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
    Panel4: TPanel;
    Panel40: TPanel;
    Panel41: TPanel;
    Panel42: TPanel;
    Panel43: TPanel;
    Panel44: TPanel;
    Panel45: TPanel;
    Panel46: TPanel;
    Panel47: TPanel;
    Panel48: TPanel;
    MainHomePanel: TPanel;
    pEdit: TPanel;
    Panel5: TPanel;
    Panel50: TPanel;
    Panel51: TPanel;
    Panel52: TPanel;
    Panel53: TPanel;
    pEdit1: TPanel;
    pTabNew: TPanel;
    pTabOld: TPanel;
    pgc_dev: TPageControl;
    Panel2: TPanel;
    Panel23: TPanel;
    Panel24: TPanel;
    Panel25: TPanel;
    Panel26: TPanel;
    Panel3: TPanel;
    pgc: TPageControl;
    Panel1: TPanel;
    pShOld: TShape;
    pShNew: TShape;
    sh_sett: TShape;
    sh_home: TShape;
    Shape3: TShape;
    Shape4: TShape;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    TabSheet3: TTabSheet;
    ts_old: TTabSheet;
    ts_new: TTabSheet;
    tm_time: TTimer;
    tsMain: TTabSheet;
    tsSett: TTabSheet;
    tsControll: TTabSheet;
    tsToDo: TTabSheet;
    tsDebug: TTabSheet;
    procedure OnHomeControllButtonMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
    procedure Image1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Image1MouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer
      );
    procedure lv_oldCustomDrawItem(Sender: TCustomListView; Item: TListItem;
      State: TCustomDrawState; var DefaultDraw: Boolean);
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
    procedure Panel30MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Panel30MouseWheelDown(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure Panel31MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Panel37Click(Sender: TObject);
    procedure Panel3MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pEditMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pgc_devChange(Sender: TObject);
    procedure pTabNewMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pTabOldClick(Sender: TObject);
    procedure pTabOldMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Shape3MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure shApplyChangeBounds(Sender: TObject);
    procedure sh_settMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure sh_homeMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure tm_timeTimer(Sender: TObject);
    procedure tsSettShow(Sender: TObject);
  private
    FPingProcess: TPingProcess;
    FSettingsForm: TForm;
    F_lv_old_Down, F_lv_new_Down: Boolean;
    dy: Integer;
  protected
    procedure ShowTime;
    procedure DoShow; override;
    procedure RunProcessPing;
    procedure KillProcessPing;
  public
    property PingProcessThread: TPingProcess read FPingProcess write FPingProcess;
    property SettingsForm: TForm read FSettingsForm write FSettingsForm;
  end;

var
  MainForm: TForm1;

implementation

{$R *.lfm}


{ TForm1 }

procedure TForm1.AfterConstruction;
begin
  inherited AfterConstruction;
  pgc_dev.OnChange(nil);
//  Image1.Canvas.Pen.Width:=4;
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
begin
end;

procedure TForm1.Panel30MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   lv: TListView;
begin

  if pgc_dev.ActivePage = ts_old then
    lv := lv_old
  else
    lv := lv_New;

  if not Assigned(lv) then
    Exit;

  if not Assigned(lv.Selected) then
    if (lv.Items.Count > 0) then
      lv.Items[0].Selected:= True;

  uSett.Execute(lv);

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
  pTabNew.Enabled := True;
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

procedure TForm1.lv_oldCustomDrawItem(Sender: TCustomListView; Item: TListItem;
  State: TCustomDrawState; var DefaultDraw: Boolean);
begin

end;

procedure TForm1.OnHomeControllButtonMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  p: TPanel;
  js, js_answ: TJSONData;
  fip, state, addr,outGet: string;
begin
  if not Assigned(Sender) then // осоьенности моментов перестроение (отсутствие begin/end update)
    Exit;
  if not (Sender is TPanel) then
    Exit;

  try
    p := (Sender as TPanel);
    js := GetJSON(p.Hint);
    try
      fip := js.FindPath('ip').AsString;
      state := js.FindPath('state').AsString;

      if sametext('on', state) then
      begin
        addr := 'http://' + fip + c_turn_off;
        RunCommand('/curl -m 2 ' + addr, outGet);
        js_answ := GetJSON(outGet);
        try
          if sametext('off', js_answ.FindPath('state').AsString) then
          begin
            p.Color:= clWhite;
            p.Hint:= '{ip:'+ chr(39) + fIp + chr(39) +',state:'
                   + chr(39) + 'off' + chr(39) +'}';
          end;
        finally
          js_answ.free;
        end;
        Exit;
      end
      else
      begin
        addr := 'http://' + fip + c_turn_on;
        RunCommand('/curl -m 2 ' + addr, outGet);
        js_answ := GetJSON(outGet);
        try
          if sametext('on', js_answ.FindPath('state').AsString) then
          begin
            p.Color:= clGreen;
            p.Hint:= '{ip:'+ chr(39) + fIp + chr(39) +',state:'
                   + chr(39) + 'on' + chr(39) +'}';
          end;
        finally
          js_answ.free;
        end;
        Exit;
      end;
    finally
      js.Free;
    end;

  except
    // хз но иногда крашится
  end;

end;

procedure TForm1.Image1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TForm1.Image1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin

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
  pTabOld.Enabled := True;
end;

procedure TForm1.Panel3MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pgc.ActivePage := tsMain;
end;

procedure TForm1.pEditMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if pEdit.tag = 0 then
  begin
    pEdit.BevelInner:= bvLowered;
    pEdit.tag := 1;
  end
  else
  begin
    pEdit.BevelInner:= bvSpace;
    pEdit.tag := 0;
  end;
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

procedure TForm1.shApplyChangeBounds(Sender: TObject);
begin

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

