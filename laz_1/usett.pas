unit uSett;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ExtCtrls, StdCtrls,
  uTypes, process, ComCtrls;

type

  { TfrSett }

  TfrSett = class(TForm)
    bt_turn: TButton;
    Panel1: TPanel;
    pShNew: TShape;
    pTabNew: TPanel;
    procedure bt_turnClick(Sender: TObject);
    procedure bt_turnMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure bt_turnMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure pShNewMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    flv: TListView;
  public

  end;

  function Execute(ALv: TListView): Boolean;

implementation
 uses
   uMain;

function Execute(ALv: TListView): Boolean;
var
  indx: Integer;
  ip: string;
begin
    MainForm.SettingsForm := TfrSett.Create(nil);
    With TfrSett(MainForm.SettingsForm) Do
    Try
      flv := ALv;
 //     indx := lv.Selected.Index;
  //    ip := lv.Selected.SubItems[2];
      bt_turn.Caption := ALv.Selected.SubItems[2];

      Result := (ShowModal = mrOk);
      If Result Then
      begin

      end;
    Finally
      Free;
    End;
end;

{$R *.lfm}

{ TfrSett }

procedure TfrSett.FormCreate(Sender: TObject);
begin

end;

procedure TfrSett.bt_turnMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
end;

procedure TfrSett.bt_turnMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   fip, pref, addr, outGet: string;
begin
  if not Assigned(flv.Selected) then
  begin
    ModalResult:= mrCancel;
    Exit;
  end;

  flv.BeginUpdate;
  try
    if flv.Selected.SubItems[3] = 'on' then
    begin
      flv.Selected.SubItems[0] := '⚫' ;
      flv.Selected.SubItems[3] := 'off';
      pref := c_turn_off;
    end
    else
    begin
      flv.Selected.SubItems[0] := '🔥';
      flv.Selected.SubItems[3] := 'on';
      pref := c_turn_on;
    end;
  finally
    flv.EndUpdate;
  end;

  fip := flv.Selected.SubItems[2];
  addr := 'http://' + fip + pref;
  RunCommand('/curl -m 2 ' + addr, outGet);
end;

procedure TfrSett.bt_turnClick(Sender: TObject);
begin

end;

procedure TfrSett.pShNewMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  ModalResult:= mrOK;
end;

end.

