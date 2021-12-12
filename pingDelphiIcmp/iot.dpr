program iot;

uses
  Vcl.Forms,
  Main in 'Main.pas' {frMain},
  PingUnits in 'PingUnits.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrMain, frMain);
  Application.Run;
end.
