using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Text.Json;
using System.Threading.Tasks;
using System.Threading;
using System;
using System.Reactive;
using ReactiveUI;
using System.Diagnostics;
using System.IO;
using testServ.Models;
using Avalonia.Collections;
using System.ComponentModel;

using System.Collections.ObjectModel;

namespace testServ.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        private AvaloniaList<Device> fakeDevices;
        public AvaloniaList<Device> FakeDevices
        {
            get { return fakeDevices; }
            set
            {
                fakeDevices = value;
                this.RaiseAndSetIfChanged(ref fakeDevices, value);
            }
        }


        private AvaloniaList<Device> devices;
        public AvaloniaList<Device> Devices
        {
            get { return devices; }
            set 
            {
                devices = value;
                this.RaiseAndSetIfChanged(ref devices, value);
            }
        }

        public MainWindowViewModel()
        {
            FakeDevices = new AvaloniaList<Device>
            {
                new Device {name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "off", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
                new Device { name = "111", state = "on", ip = "192.168.1.3" },
            };
           
            devices = new();
        }


       
        public string Greeting => "Welcome to Avalonia!   ";
        public string Text => "Nu-ka poshel nahoi!   ";
        public string Hello => "Tvoya dom truba shatal!   ";

        private string textLog1 = "";

       

        public string ShownValue
        {
            get => textLog1;
            set => this.RaiseAndSetIfChanged(ref textLog1, value);
        }


        public bool ServerIsStart = false;

        private static void Collection_Changed(object sender,
            System.ComponentModel.CollectionChangeEventArgs e)
        {
            Console.WriteLine("Collection_Changed Event: '{0}'\table element={1}",
                e.Action.ToString(), e.Element.ToString());
        }

        public void AddDevice()
        {
            FakeDevices.Add(new Device { name = "111", state = "on", ip = "192.168.1.3" });
        }
        
        async public Task ServerStart()
        {
            if (ServerIsStart)
            { }
            else
            {
//                devices.CollectionChanged();
                ServerIsStart = true;

                //Task.Run(DoPingStart);// => DoPingStart());
                //Task.Run(DoServerStart);// => DoServerStart());

                Thread ServerThread = new(DoServerStart);
                ServerThread.Start();

                Thread PingThread = new(DoPingStart);
                PingThread.Start();
            
            }
        }
        
        void Log(string str)
        {
            ShownValue = ShownValue + str;
        }
        void DoServerStart()
        {
            
            int port = 7890;
            //IPAddress localAddr = IPAddress.Parse("192.168.1.16");
            //TcpListener Server = new(localAddr, port);

            TcpListener Server = new(IPAddress.Any, port);
            Server.Start();
      

            Byte[] bytes = new Byte[1024]; //буфер для хранения входящей строки
            String data = "";

            
            while (true)
            {
                TcpClient client = Server.AcceptTcpClient();
                Log("AcceptTcpClient is done"+"\n");
                NetworkStream stream = client.GetStream();

                int i;
                while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    data = Encoding.ASCII.GetString(bytes, 0, i);
                    if ((data.Contains("GET")) || (data.Contains("POST"))) { }
                    else if (data.Contains("{"))
                    {
                        Log("Received: "+data + "\n");
                        try
                        {
                            Device? device = JsonSerializer.Deserialize<Device>(data);
                            if (device == null) Log("JSON is not valid" + "\n");
                            bool devInDevs = false;
                            foreach (Device device1 in devices)
                            {
                                if (device1.ip == device.ip)
                                    devInDevs = true;
                                else continue;
                            }
                            if(!devInDevs)
                            {
                                devices.Add(device);

                            }
                        }
                        catch { Log("Ошибка парсинга принятого запроса"); }
                    }
                }
                stream.Close();
                client.Close();
                Log("device.Count:  " + devices.Count.ToString()+" ServerStart\n");
                
            }
            
        }

        void DoPingStart()
        {
            while (true)
            {
               
                //пинг всех устройств
                if (devices.Count > 0)
                {
                    for (int j = devices.Count - 1; j >= 0; j--)
                    {
                        string response = "";
                        MyWebClient webClient = new();
                        try
                        {
                            response = webClient.DownloadString("http://" + devices[j].ip + "/areyouhere");
                            Log("dev " + devices[j].ip + " is here!" + "\n");
                        }
                        catch { Log("GET dont work!!!" + "\n"); }
                        if (response == "")
                        {
                            Log("dev " + devices[j].ip + " delete" + "\n");
                            devices.RemoveAt(j);
                        }
                    }
                }
                Thread.Sleep(500);
                Log("device.Count:  " + devices.Count.ToString() + " PingStart\n");
               
            }
        }


        private class MyWebClient : WebClient
        {
            protected override WebRequest GetWebRequest(Uri uri)
            {
                WebRequest w = base.GetWebRequest(uri);
                w.Timeout = 1000;
                return w;
            }
        }
    }
}