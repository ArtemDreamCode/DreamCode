program project1;

{$mode objfpc}{$H+}

uses
  {$IFNDEF WIN32}
    {$DEFINE UseCThreads}
    {$IFDEF UseCThreads}
    cthreads,
     cmem,
    {$ENDIF}{$ENDIF}
  Interfaces,// this includes the LCL widgetset
  Forms, lazcontrols, runtimetypeinfocontrols, uMain, uTypes, uCore, uSett
  { you can add units after this };

{$R *.res}

begin
  SetHeapTraceOutput('heaptrace.trc');
  RequireDerivedFormResource:=True;
  Application.Scaled:=True;
  Application.Initialize;
  Application.CreateForm(TForm1, MainForm);
  Application.Run;
end.

