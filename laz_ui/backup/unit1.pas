unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Forms, Buttons, StdCtrls, ExtCtrls, Classes, SysUtils, Controls,
  Dialogs, BGRAFlashProgressBar;

type

  { TRoundedForm }

  TRoundedForm = class(TForm)
      BCRadialProgressBar1: TBGRAFlashProgressBar;
      pnlMain: TPanel;
      pnlNav: TPanel;
      pnlNavButtons: TPanel;
      pnlUser: TPanel;

      nbkPages: TNotebook;
      dashboard: TPage;
      video: TPage;
      music: TPage;
      book: TPage;
      settings: TPage;

      Panel1: TPanel;
      Panel2: TPanel;
      Panel3: TPanel;
      Panel4: TPanel;
      Panel5: TPanel;
      Panel6: TPanel;

      Image1: TImage;
      Image2: TImage;
      Image3: TImage;
      Image4: TImage;

      Label1: TLabel;
      Label2: TLabel;
      Label3: TLabel;
      Label4: TLabel;
      Label5: TLabel;
      Label6: TLabel;
      Label7: TLabel;

      btnBackward: TSpeedButton;
      btnStop: TSpeedButton;
      btnPlay: TSpeedButton;
      btnPause: TSpeedButton;
      btnForward: TSpeedButton;
      SpeedButton1: TSpeedButton;
      SpeedButton2: TSpeedButton;
      SpeedButton3: TSpeedButton;

      lblDashboard: TLabel;
      lblVideo: TLabel;
      lblMusic: TLabel;
      lblBook: TLabel;
      lblSettings: TLabel;
      lblUserName: TLabel;
      lblAppInfo: TLabel;

      btnDashboard: TSpeedButton;
      btnSettings: TSpeedButton;
      btnBook: TSpeedButton;
      btnMusic: TSpeedButton;
      btnVideo: TSpeedButton;
      btnExit: TSpeedButton;

      edtSearch: TEdit;
      progressBar: TBGRAFlashProgressBar;
      radialProgress: TBGRAFlashProgressBar;
      shapeSearchField: TShape;

      procedure btnExitClick(Sender: TObject);
      procedure FormCreate(Sender: TObject);
      procedure FormShow(Sender: TObject);
      procedure pnlMouseDown(Sender: TObject; Button: TMouseButton;
        Shift: TShiftState; X, Y: Integer);
      procedure pnlMouseMove(Sender: TObject; Shift: TShiftState; X,
            Y: Integer);
      procedure btnNavClick(Sender: TObject);


    private
        FRgnCorner: Integer;
        FMousePoint: TPoint;
        FFormPoint: TPoint;
        progressTimer: TTimer;
        procedure DeleteRegion;
        procedure CreateRegion;
        procedure SetMouseEvents;
        procedure progressTimerEvent(Sender: TObject);

    end;

var
  RoundedForm: TRoundedForm;

implementation

{$R *.lfm}

{ TRoundedForm }

procedure TRoundedForm.FormCreate(Sender: TObject);
begin
  Position:= poScreenCenter;
  FRgnCorner := 16;
  SetMouseEvents;
  CreateRegion;
  progressTimer:= TTimer.Create(self);
  progressTimer.OnTimer:= @progressTimerEvent;
end;

procedure TRoundedForm.FormShow(Sender: TObject);
begin
  Position:= poDesigned;
end;

procedure TRoundedForm.SetMouseEvents;
var i: integer;
begin
  for i:=0 to nbkPages.Pages.Count-1 do
  begin
    nbkPages.Page[i].OnMouseDown:= @pnlMouseDown;
    nbkPages.Page[i].OnMouseMove:= @pnlMouseMove;
  end;
end;

procedure TRoundedForm.btnExitClick(Sender: TObject);
begin
  close;
end;

procedure TRoundedForm.btnNavClick(Sender: TObject);
begin
  nbkPages.PageIndex:= (Sender as TSpeedButton).Tag;
end;

procedure TRoundedForm.progressTimerEvent(Sender: TObject);
begin
  if radialProgress.Value >= radialProgress.MaxValue then
    radialProgress.Value:= 0;
  if progressBar.Value >= progressBar.MaxValue then
    progressBar.Value:= 0;
  progressBar.Value:= progressBar.Value + 2;
  radialProgress.Value:= radialProgress.Value + 2;
end;

procedure TRoundedForm.pnlMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if (GetKeyState(VK_LBUTTON) < 0) then
  begin
    FMousePoint := Mouse.CursorPos;
    FFormPoint  := Classes.Point(Left, Top);
  end;
end;

procedure TRoundedForm.pnlMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if (GetKeyState(VK_LBUTTON) < 0) and (FRgnHandle<>0) then
  begin
    RoundedForm.left := Mouse.CursorPos.X - (FMousePoint.X - FFormPoint.X);
    RoundedForm.top := Mouse.CursorPos.Y - (FMousePoint.Y - FFormPoint.Y);
  end else
  begin
    if (Sender<>pnlMain) and (Y > 10) then
    begin
      if (FRgnHandle=0) then CreateRegion
    end else
      DeleteRegion;
  end;
end;

procedure TRoundedForm.CreateRegion;
begin
  BorderStyle := bsNone;
  if FRgnHandle <> 0 then DeleteObject(FRgnHandle);
  with RoundedForm.BoundsRect do
  begin
    FRgnHandle := CreateRoundRectRgn(0, 0, Right - Left + 1, Bottom - Top + 1,
                  FRgnCorner, FRgnCorner);
  end;

  if SetWindowRGN(Handle, FRgnHandle, True)=0 then DeleteObject(FRgnHandle);
end;


procedure TRoundedForm.DeleteRegion;
begin
  if FRgnHandle <> 0 then
  begin
    BorderStyle := bsToolWindow;
    SetWindowRGN(Handle, 0, True);
    DeleteObject(FRgnHandle);
    FRgnHandle := 0;
  end;
end;

end.
