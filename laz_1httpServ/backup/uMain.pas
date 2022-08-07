unit uMain;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ExtCtrls,
  ComCtrls, EditBtn, uCore, uTypes, Types, uSett, sqlite3conn, sqldb, fpjson,
  process, fphttpserver;

  { TForm1 }
type
  TForm1 = class(TForm)
    img_state: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    lb_cnt_sett: TLabel;
    lb_num_sett: TLabel;
    MainHomePanel: TPanel;
    PageControl2: TPageControl;
    Panel1: TPanel;
    Panel2: TPanel;
    Panel23: TPanel;
    Panel24: TPanel;
    Panel25: TPanel;
    Panel26: TPanel;
    Panel27: TPanel;
    Panel3: TPanel;
    Panel30: TPanel;
    Panel31: TPanel;
    Panel33: TPanel;
    Panel34: TPanel;
    Panel35: TPanel;
    Panel36: TPanel;
    Panel38: TPanel;
    Panel39: TPanel;
    Panel4: TPanel;
    Panel40: TPanel;
    Panel43: TPanel;
    Panel44: TPanel;
    Panel45: TPanel;
    Panel46: TPanel;
    Panel47: TPanel;
    Panel48: TPanel;
    Panel5: TPanel;
    Panel50: TPanel;
    Panel51: TPanel;
    Panel52: TPanel;
    Panel53: TPanel;
    Panel6: TPanel;
    pBtn: TPanel;
    pEdit: TPanel;
    pEdit1: TPanel;
    Shape3: TShape;
    Shape4: TShape;
    sh_home: TShape;
    sh_sett: TShape;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    TabSheet3: TTabSheet;
    tm_time: TTimer;
    pgc: TPageControl;
    m_device: TMemo;
    tsControll: TTabSheet;
    tsSett: TTabSheet;
    tsDebug: TTabSheet;
    lb_date: TLabel;
   lb_time: TLabel;
    tsMain:TTabSheet;
    tsToDo: TTabSheet;
    procedure Button1Click(Sender: TObject);
    procedure FServerRequest(Sender: TObject;
      var ARequest: TFPHTTPConnectionRequest;
      var AResponse: TFPHTTPConnectionResponse);
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
    procedure Panel34Click(Sender: TObject);
    procedure Panel37Click(Sender: TObject);
    procedure Panel3MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pBtnMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pEditMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pgcChange(Sender: TObject);
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
    FServerProcess: TServerProcess;
    FSettingsForm: TForm;
    F_lv_old_Down, F_lv_new_Down: Boolean;
    dy: Integer;
  protected
    procedure ShowTime;
    procedure DoShow; override;
    procedure RunProcessServer;
    procedure KillProcessServer;
  public
 //   property PingProcessThread: TPingProcess read FPingProcess write FPingProcess;
    property SettingsForm: TForm read FSettingsForm write FSettingsForm;
  end;

var
  MainForm: TForm1;

implementation

{$R *.lfm}

 // size op = 10,6 mbit on start !
{ TForm1 }

procedure TForm1.AfterConstruction;
begin
  inherited AfterConstruction;
end;

procedure TForm1.Arrow1Click(Sender: TObject);
begin
  pgc.ActivePage := tsMain;
end;

procedure TForm1.BeforeDestruction;
begin
//  KillProcessPing;
  KillProcessServer;
  inherited BeforeDestruction;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  ShowTime;
  pgc.ActivePage := tsMain;
  pgc.ShowTabs:= False;
  BorderStyle:= bsNone;
  Position:= poDesktopCenter;
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
begin


end;

procedure TForm1.Panel30MouseWheelDown(Sender: TObject; Shift: TShiftState;
  MousePos: TPoint; var Handled: Boolean);
begin
end;

procedure TForm1.Panel31MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
end;

procedure TForm1.Panel34Click(Sender: TObject);
begin

end;

procedure TForm1.Panel37Click(Sender: TObject);
begin

end;

procedure TForm1.OnCustomTabLinkClickNew;
begin
end;

procedure TForm1.lv_oldMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin

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

procedure TForm1.FServerRequest(Sender: TObject;
  var ARequest: TFPHTTPConnectionRequest;
  var AResponse: TFPHTTPConnectionResponse);
  var LState: String;
begin
  LState := ARequest.QueryFields.Values['turn'];
  AResponse.Content := 'led1 state ' + LState + '!';
  //Handled := true;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin

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
end;

procedure TForm1.Panel3MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pgc.ActivePage := tsMain;
end;

procedure TForm1.pBtnMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TForm1.pEditMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TForm1.pgcChange(Sender: TObject);
begin

end;

procedure TForm1.pgc_devChange(Sender: TObject);
begin

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
  pgc.ActivePage := tsSett;
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
//  RunProcessPing;
  RunProcessServer;
end;

procedure TForm1.RunProcessServer;
begin
  if Assigned(FServerProcess) then
     FServerProcess.Terminate;
   FServerProcess := TServerProcess.Create;
   FServerProcess.Priority := tpLower;
end;

procedure TForm1.KillProcessServer;
begin
//  FServerProcess.Terminate;
//  FServerProcess.WaitFor;
//  FreeAndNil(FServerProcess);
end;

end.

