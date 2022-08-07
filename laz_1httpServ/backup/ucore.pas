unit uCore;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,
   process, fpjson,
   jsonparser, jsonscanner, uTypes, ExtCtrls, StdCtrls, Controls, Graphics,
   fphttpserver, httpdefs, httproute, sqlite3conn, sqldb, Dialogs;

type
  { TServerProcess }
  TServerProcess  = class(TThread)
  private
    _Error: string;
    FServer: TFPHttpServer;
    FdbConn: TSQLite3Connection;
    FdbTransact: TSQLTransaction;
    fdebuginfoAdd, fdebuginfoText: string;
    procedure OnRequest(Sender: TObject; Var ARequest: TFPHTTPConnectionRequest; Var AResponse : TFPHTTPConnectionResponse);
    function ContentToMap(AContent: string): Boolean;
    procedure GUIControll; // for sync
  private
    FDevList: TDeviceList;
    FBtnLIst: TList;
  protected
    procedure logAdd(const AValue: string);
    procedure _logAdd;
    procedure logText(const AValue: string);
    procedure _logText;
    procedure Execute; override;
    procedure DoGUIControll;
  public
    constructor Create;
    destructor Destroy; override;
  end;

implementation

uses
  uMain;

{ TServerProcess }

procedure TServerProcess.OnRequest(Sender: TObject; var ARequest: TFPHTTPConnectionRequest;
    var AResponse: TFPHTTPConnectionResponse);
var
  ADevice: TDevice;
 begin
 try
{  logAdd('ARequest.RemoteAddr: [' + ARequest.RemoteAddr +'] ');
  logAdd('ARequest.URI: [' + ARequest.URI +'] ');
  logAdd('----------------------------------------');
  logAdd('ARequest.Content: [' + ARequest.Content +'] ');
  logAdd('----------------------------------------');
}

  if (SameText(ARequest.URI, '/dev.json')) then // post req from job
  begin
    if not ContentToMap(ARequest.Content) then
      Exit;
    logText(FDevList.ToShortString);
  end;

  if ((SameText(ARequest.URI, '/on')) or (SameText(ARequest.URI, '/off'))) then // get req on/off by device
  begin
    if ((FDevList.Count > 0) and (FDevList.IsContainsByIp(ARequest.RemoteAddr))) then
      FDevList.SetNewState(ARequest.RemoteAddr, ARequest.URI);  // -< gui dependence
    logText(FDevList.ToShortString);
  end;

 DoGUIControll;
 except
   On E:Exception do
   begin
    //  ShowMessage('Ошибка [OnRequest]: ' + E.Message);
      logAdd('Ошибка [OnRequest]: ' + E.Message);
   end;
 end;
end;

function TServerProcess.ContentToMap(AContent: string): Boolean;
var
  JsonParser: TJSONParser;
  JsonObject: TJSONObject;
  jdev: TJSONData;
  JsonEnum: TBaseJSONEnumerator;
  i: integer;
  v_ip: string;
  ADevice: TDevice;
begin
//  FDevList.Clear;
  FDevList.MarkLife(0);
  Result := True;
  try
    JsonParser := TJSONParser.Create(AContent, DefaultOptions);
    try
      JsonObject := JsonParser.Parse as TJSONObject;
      try
        JsonEnum := JsonObject.GetEnumerator;
        try
          while JsonEnum.MoveNext do
            if JsonObject.Types[JsonEnum.Current.Key] = jtArray then
               for i:=0 to Pred(TJSONArray(JsonEnum.Current.Value).Count) do
               begin
                 jdev := GetJSON(TJSONArray(JsonEnum.Current.Value).Items[i].AsJSON);
                 v_ip := jdev.FindPath('ip').AsString;

                 if not FDevList.IsContainsByIp(v_ip) then
                 begin
                   ADevice.Ip :=          jdev.FindPath('ip').AsString;
                   ADevice.DeviceClass := jdev.FindPath('class').AsString;
                   ADevice.State :=       jdev.FindPath('state').AsString;
                   ADevice.Name :=        jdev.FindPath('name').AsString;
                   ADevice.GUID :=        jdev.FindPath('device_guid').AsString;
                   ADevice.DeviceIndex := jdev.FindPath('index').AsString;
                   ADevice.IsNewDevice := jdev.FindPath('isnewdevice').AsString;
                   ADevice.IsLife:= 1;
                   FDevList.Add(ADevice);  // -< gui dependence
                 end else  // make poverka
                   FDevList.MarkLife(v_ip, 1); //  // -< gui dependence
               end;
        finally
          FreeAndNil(JsonEnum)
        end;
      finally
        FreeAndNil(JsonObject);
      end;
    finally
      FreeAndNil(JsonParser);
    end;

  except
  On E:Exception do
   begin
      Result := False;
      logAdd('Ошибка [ContentToMap]: ' + E.Message);
   end;
  end;
end;

procedure TServerProcess.GUIControll;
var
  dev_elem: TDevice;
  FDevButtonController: TDevButtonController;
begin
  FDevButtonController := TDevButtonController.Create;
  try
   for dev_elem in FDevList.List do
     if not FDevButtonController.FindByIp(dev_elem.Ip) then
       FDevButtonController.Add(dev_elem)
     else
       FDevButtonController.Refresh(dev_elem);

  finally
    FDevButtonController.Free;
  end;
  finally
  end;

end;

procedure TServerProcess.logAdd(const AValue: string);
begin
  fdebuginfoAdd := AValue;
  Synchronize(@_logAdd);
end;

procedure TServerProcess._logAdd;
begin
  MainForm.m_device.Lines.Add('[' + TimeToStr(Now) + '] '  + fdebuginfoAdd)
end;

procedure TServerProcess.logText(const AValue: string);
begin
  fdebuginfoText := AValue;
  Synchronize(@_logText);
end;

procedure TServerProcess._logText;
begin
  MainForm.m_device.Clear;
  MainForm.m_device.Lines.Add('[' + TimeToStr(Now) + '] '  + fdebuginfoText)
end;

procedure TServerProcess.Execute;
begin
 try
    FServer.Active := True;
  except
    on E: Exception do
    begin
      _Error := E.Message;
    end;
  end;
end;

procedure TServerProcess.DoGUIControll;
begin
 Synchronize(@GUIControll);
end;

constructor TServerProcess.Create;
begin
 FServer := TFPHttpServer.Create(nil);
 FServer.port := 8080;
 FServer.threaded := true;
 FServer.OnRequest := @OnRequest;
 FreeOnTerminate := true;
 FDevList := TDeviceList.Create;
 FBtnLIst := TList.Create;
 inherited Create(False);
end;

destructor TServerProcess.Destroy;
begin
  FServer.Active:= False;
  FServer.Free;
  FDevList.Free;
  FBtnLIst.Free;
  inherited Destroy;
end;



{controll by FOldDeviceList FNewDeviceList}
{
 device api
http://192.168.4.100/state
http://192.168.4.100/relay?turn=on
http://192.168.4.100/set?name=value
http://172.20.10.14/reset
http://172.20.10.14/fullreset

http://192.168.4.100/scan
}
end.

