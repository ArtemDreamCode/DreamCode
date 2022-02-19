unit uSett;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ExtCtrls, StdCtrls,
  uTypes, process, ComCtrls;

type

  { TfrSett }

  TfrSett = class(TForm)
    focused_button: TButton;
    edName: TEdit;
    Label1: TLabel;
    Panel1: TPanel;
    Panel2: TPanel;
    pTabNew2: TPanel;
    pReset: TPanel;
    pSwitch: TPanel;
    shApply: TShape;
    pShNew1: TShape;
    pTabNew: TPanel;
    pTabNew1: TPanel;
    Shape1: TShape;
    Shape2: TShape;
    shReset: TShape;
    shFullReset: TShape;
    shSwitch: TShape;
    procedure edNameChange(Sender: TObject);
    procedure FormClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure s(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure shApplyMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure shFullResetMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure shResetMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure shSwitchMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    flv: TListView;
    procedure ChangeName;
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
      if not Assigned(ALv.Selected) then
         Exit;

      flv := ALv;
      pReset.Enabled := flv.Selected.SubItems[4] = 'old';
      if flv.Selected.SubItems[3] = 'on' then
      begin
        pSwitch.Caption:= 'Выключить';
        shSwitch.Pen.Color := $FEFF14;
      end else
      begin
        pSwitch.Caption:= 'Включить';
        shSwitch.Pen.Color := $151F22;
      end;

      edName.TextHint:= ALv.Selected.SubItems[1];
      ShowModal;
    //  Result := (ShowModal = mrOk);
     // If Result Then
     //   if edName.Text > '' then
     //     ChangeName;
    Finally
      Free;
    End;
end;

{$R *.lfm}

{ TfrSett }

procedure TfrSett.FormCreate(Sender: TObject);
begin

end;

procedure TfrSett.FormShow(Sender: TObject);
begin
  focused_button.SetFocus;
end;

procedure TfrSett.edNameChange(Sender: TObject);
begin
  if Length(edName.Text) > 20 then
    Exit;
end;

procedure TfrSett.FormClick(Sender: TObject);
begin
  focused_button.SetFocus;
end;

procedure TfrSett.s(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TfrSett.shApplyMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  ChangeName;
end;

procedure TfrSett.shFullResetMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin

end;

procedure TfrSett.shResetMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   fip, addr, outGet: string;
   p: TDevice;
begin
  if not Assigned(flv.Selected) then
  begin
    ModalResult:= mrCancel;
    Exit;
  end;

  p.Ip:= flv.Selected.SubItems[2];
  p.State:=flv.Selected.SubItems[3];
  p.Name:=flv.Selected.SubItems[1];
  p.IsNewDevice:= flv.Selected.SubItems[4];
  if Assigned(MainForm.lv_New) then
  begin
    MainForm.lv_New.BeginUpdate;
    try
      with MainForm.lv_New.Items.Add do
        begin
           Caption:= MainForm.lv_New.Items.Count.ToString + '.';
           if SameText(p.State, 'off') then
             SubItems.Add('⚫')
           else
             SubItems.Add('🔥');
           SubItems.Add(p.Name);
           SubItems.Add(p.Ip);
           SubItems.Add(p.State);
           SubItems.Add('new');
        end;
    finally
      MainForm.lv_New.EndUpdate;
    end;
  end;

   try
    fip := flv.Selected.SubItems[2];
    flv.BeginUpdate;
    try
       flv.Selected.Delete;
    finally
      flv.EndUpdate;
    end;

    MainForm.pTabOld.Enabled:= flv.Items.Count  > 0;
    MainForm.pTabNew.Enabled:= MainForm.lv_New.Items.Count  > 0;

     if not MainForm.pTabOld.Enabled then
       MainForm.OnCustomTabLinkClickNew;

    addr := 'http://' + fip + c_reset;
    RunCommand('/curl -m 2 ' + addr, outGet);
  finally
    ModalResult:= mrOk;
  end;
end;

procedure TfrSett.shSwitchMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   fip, pref, addr, outGet: string;
begin
  focused_button.SetFocus;
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
      pSwitch.Caption := 'Включить';
      shSwitch.Pen.Color := $151F22;
    end
    else
    begin
      flv.Selected.SubItems[0] := '🔥';
      flv.Selected.SubItems[3] := 'on';
      pref := c_turn_on;              
      pSwitch.Caption := 'Выключить';
      shSwitch.Pen.Color := $FEFF14;
    end;
  finally
    flv.EndUpdate;
  end;

  fip := flv.Selected.SubItems[2];
  addr := 'http://' + fip + pref;
  RunCommand('/curl -m 2 ' + addr, outGet);
end;

procedure TfrSett.ChangeName;
var
   fip, addr, outGet: string;
   p: TDevice;
begin
  if not Assigned(flv.Selected) then
  begin
    ModalResult:= mrCancel;
    Exit;
  end;

  if Length(edName.Text) = 0 then
  begin
    ModalResult:= mrCancel;
    Exit;
  end;

  p.Ip:= flv.Selected.SubItems[2];
  p.State:=flv.Selected.SubItems[3];
  p.Name:=flv.Selected.SubItems[1];
  p.IsNewDevice:= flv.Selected.SubItems[4];

  if p.IsNewDevice = 'old' then
  begin
      fip := flv.Selected.SubItems[2];
      flv.BeginUpdate;
      try
        flv.Selected.SubItems[1] := edName.Text;
      finally
        flv.EndUpdate;
      end;
      addr := 'http://' + fip + c_change_name + edName.Text;
      RunCommand('/curl -m 2 ' + addr, outGet);
      ModalResult:= mrOk;
      Exit;
  end
  else
  begin
     if Assigned(MainForm.lv_Old) then
       begin
         MainForm.lv_Old.BeginUpdate;
         try
           with MainForm.lv_Old.Items.Add do
             begin
                Caption:= MainForm.lv_Old.Items.Count.ToString + '.';
                if SameText(p.State, 'off') then
                  SubItems.Add('⚫')
                else
                  SubItems.Add('🔥');
                SubItems.Add(edName.Text);
                SubItems.Add(p.Ip);
                SubItems.Add(p.State);
                SubItems.Add('old');
             end;
         finally
           MainForm.lv_Old.EndUpdate;
         end;
       end;

      try
        fip := flv.Selected.SubItems[2];
        flv.BeginUpdate;
        try
           flv.Selected.Delete;
        finally
          flv.EndUpdate;
        end;

        MainForm.pTabNew.Enabled:= flv.Items.Count  > 0;
        MainForm.pTabOld.Enabled:= MainForm.lv_Old.Items.Count  > 0;
        if not MainForm.pTabNew.Enabled then
          MainForm.OnCustomTabLinkClickOLd;

        addr := 'http://' + fip + c_change_name + edName.Text;
        RunCommand('/curl -m 2 ' + addr, outGet);

      finally
        ModalResult:= mrOk;
      end;

  end;
end;

end.

