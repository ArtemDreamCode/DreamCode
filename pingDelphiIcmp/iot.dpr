program iot;

uses
  Vcl.Forms,
  Main in 'Main.pas' {frMain},
  PingUnits in 'PingUnits.pas',
  cxClasses in 'ModifiedSource\cxClasses.pas',
  cxDrawTextUtils in 'ModifiedSource\cxDrawTextUtils.pas',
  cxGrid in 'ModifiedSource\cxGrid.pas',
  cxGridCustomTableView in 'ModifiedSource\cxGridCustomTableView.pas',
  cxGridCustomView in 'ModifiedSource\cxGridCustomView.pas',
  cxGridTableView in 'ModifiedSource\cxGridTableView.pas',
  dxCustomTileControl in 'ModifiedSource\dxCustomTileControl.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrMain, frMain);
  Application.Run;
end.
