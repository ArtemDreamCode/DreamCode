unit uSett;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ExtCtrls, StdCtrls,
  uTypes, process;

type

  { TfrSett }

  TfrSett = class(TForm)
    bt_turn: TButton;
    Panel1: TPanel;
    pShNew: TShape;
    pTabNew: TPanel;
    procedure bt_turnMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure pShNewMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    fip: string;
    flist: TDeviceList;
  public

  end;

  function Execute(const AList: TDeviceList; const AIP: string): Boolean;

implementation
 uses
   uMain;

function Execute(const AList: TDeviceList; const AIP: string): Boolean;
begin
    With TfrSett.Create(nil) Do
    Try
      fip := AIP;
      flist := AList;
      bt_turn.Caption := AIP;

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
var
   pref, addr, outGet: string;
   dev: TDevice;
begin

  dev := flist.SearchByIp(fip);

  if SameText('on', dev.State) then
  begin
     pref := c_turn_off;
     dev.State := 'off';
  end
  else begin
     pref := c_turn_on;
     dev.State := 'on';
  end;

  addr := 'http://' + fip + pref;
  RunCommand('/curl -m 2 ' + addr, outGet);
  showmessage(outGet);
end;

procedure TfrSett.pShNewMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  ModalResult:= mrOK;
end;

end.

