using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Text.Json;
using Avalonia.Collections;
using System.Collections.Specialized;
using System.Reactive;
using ReactiveUI;
using System.Threading;
using System.Threading.Tasks;
using dev.Models;

namespace dev.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        CancellationTokenSource? cancellationToken;
        
        public ReactiveCommand<Device, Unit>? ExecuteOperationCommand { get; }

        public MainWindowViewModel()
        {
            ExecuteOperationCommand = ReactiveCommand.Create<Device>(ChangeDeviceState);
            //Devices = new AvaloniaList<Device>();

            /*  Devices.Add(new Device { name = "NewDev555", state = "on", ip = "192.168.1.5", guid = "5f5f5f", isnew = "old" });

            Devices.Add(new Device { name = "NewDev666", state = "off", ip = "192.168.1.6", guid = "5f5f25f", isnew = "new" });
            Devices.Add(new Device { name = "NewDev777", state = "on", ip = "192.168.1.7", guid = "5f5f35f", isnew = "old" });
            Devices.Add(new Device { name = "NewDev888", state = "off", ip = "192.168.1.8", guid = "5f54f5f", isnew = "new" });
            */    
            
            //Log("MainWindowViewModel Constructor\n");
            ServerStart();
        }

        async public Task ServerStart()
        {
            //Log("Task ServerStart\n");
            if (ServerIsStart)
            { }
            else
            {
                cancellationToken = new CancellationTokenSource();
                ServerIsStart = true;
                //await 
                    Task.Run(() => DoRunServer(cancellationToken.Token), cancellationToken.Token);

            }
        }

        void DoRunServer(CancellationToken cts)
        {
            Log("void DoServerStart\n");

            int port = 8080;

            TcpListener Server = new(IPAddress.Any, port);
            //Log("Server on: " + IPAddress.Any + ":" + port + "\n");
            Server.Start();
            Log("Server started!\n");
            Byte[] bytes = new Byte[1024]; //буфер для хранения входящей строки
            String data = "";
            while (true)
            {
                TcpClient client = Server.AcceptTcpClient();
                string? MYIpClient = Convert.ToString(((IPEndPoint)client.Client.RemoteEndPoint).Address);
                Log("AccepClient from ip: " + MYIpClient + "\n");
                NetworkStream stream = client.GetStream();

                int i;
                while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    data = Encoding.ASCII.GetString(bytes, 0, i);
                    Log("Received: " + data + "\n");
                    if ((data.Contains("POST")))
                    {
                        GetDevice getDevice = new();
                        try
                        {
                            //если пришел пост запрос от скрипта со списком всех устройств
                            if (getDevice.devices != null)
                                getDevice.devices.Clear();

                            string newData = data.Remove(0, data.IndexOf('{'));
                            Log("New data: \n" + newData); // -<
                            //getDevice.devices = JsonSerializer.Deserialize<AvaloniaList<Device>>(newData);
                            getDevice = JsonSerializer.Deserialize<GetDevice>(newData);

                            string str = getDevice.job_date + "\n";
                            if (getDevice.devices != null)
                            {
                                foreach (Device device in getDevice.devices)
                                {
                                    str += "\tName: " + device.name + "\n";
                                    str += "\t\tIP: " + device.ip + "\n";
                                    str += "\t\tIsnew: " + device.isnewdevice + "\n";
                                    str += "\t\tState: " + device.state + "\n" + "\n";
                                }
                                ShownDevice(str);
                            }

                            if (getDevice.devices == null) { } //Log("JSON from bash is't valid! ");
                            else
                            {
                                foreach (Device device in Devices)
                                {
                                    bool devInDevs = false;
                                    foreach (Device device1 in getDevice.devices)
                                    {
                                        if (device.ip == device1.ip)
                                            devInDevs = true;
                                        else continue;
                                    }
                                    if (!devInDevs)
                                    {
                                        Devices.Remove(device);
                                    }
                                }
                                foreach (Device device in getDevice.devices)
                                {
                                    bool devInDevs = false;
                                    foreach (Device device1 in Devices)
                                    {
                                        if (device1.ip == device.ip)
                                            devInDevs = true;
                                        else continue;
                                    }
                                    if (!devInDevs)
                                    {
                                        Devices.Add(device);
                                    }
                                }
                            }
                        }
                        catch { }
                    }
                    else if (data.Contains("GET"))
                    {
                        string newData = data.Substring(data.IndexOf('/') + 1, 3);
                        string state = "";
                        if (newData.Contains("off")) { state = "off"; }
                        else if (newData.Contains("on")) { state = "on"; }
                        if (state != "")
                        {

                            Device dev = new();
                            foreach (Device device in Devices)
                            {
                                if (device.ip == MYIpClient)
                                {
                                    dev = device;
                                    //device.state = state;
                                    //device.BindCommand();
                                }
                            }
                            if (dev != null)
                            {
                                int index = Devices.IndexOf(dev);
                                Devices.RemoveAt(index);
                                dev.state = state;
                                Devices.Insert(index, dev);
                            }
                        }
                    }
                    else
                    {
                        Log("Received_end: " + data + "\n");
                    }
 //                   byte[] getdata = Encoding.UTF8.GetBytes("ok");
 //                   stream.Write(getdata, 0, getdata.Length);
                } // while
                stream.Close();
                client.Close();
                client.Dispose();

                cts.ThrowIfCancellationRequested();
            }
            Server.Stop();
        }

        private void ChangeDeviceState(Device device)
        {
            if (device.state == "on")
            {
                GetToDevice(device.ip, "relay?turn=off");
            }
            //отправить гет-запрос о выючении
            else if (device.state == "off")
            {
                GetToDevice(device.ip, "relay?turn=on");
            }
         
        }

        async public Task GetToDevice(string ip, string command)
        {
            await Task.Run(() => {
                string response = "";
                MyWebClient webClient = new();
                try
                {
                    response = webClient.DownloadString("http://" + ip + "/" + command);
                    //Log("dev " + ip + "/" + command + " ::: ");
                    //Log(response + "\n");
                }
                catch { }//Log("PING dont work!!!" + "\n"); }
            });
        }

        private class MyWebClient : WebClient
        {
            protected override WebRequest GetWebRequest(Uri uri)
            {
                WebRequest w = base.GetWebRequest(uri);
                w.Timeout = 500;
                return w;
            }
        }
    }
}
