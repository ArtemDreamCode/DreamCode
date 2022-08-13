using ReactiveUI;
using System;
using System.Collections.Generic;
using System.Text;
using Avalonia.Collections;
using dev.Models;

namespace dev.ViewModels
{
    public class ViewModelBase : ReactiveObject
    {

        private AvaloniaList<Device>? devices;
 //       private AvaloniaList<string>? flog_data;
        public AvaloniaList<Device> Devices
        {
            get => devices;
            set
            {
                devices = value;
                this.RaiseAndSetIfChanged(ref devices, value);
            }
        }
    /*    public AvaloniaList<string> log_data
        {
            get => flog_data;
        
            set
            {
                flog_data = value;
                this.RaiseAndSetIfChanged(ref flog_data, value);
            }
        }
    */

        public ViewModelBase() {
            devices = new AvaloniaList<Device> {};
    //        log_data = new AvaloniaList<string> { };
        }

        public bool ServerIsStart = false;
        public void ShownDevice(string str)
        {
            ShownDevices = str;
        }

        private string textLog2 = "";
        
        public void Log(string str)
        {
            string s;
            s = "------" + DateTime.Now.ToString() + "-------------\n";
            s = s + str + "\n";
            s = s + "------------------------------\n";
            ShownDevices = ShownDevices + s;
        }

        public string ShownDevices
        {
            get => textLog2;
            set => this.RaiseAndSetIfChanged(ref textLog2, value);
        }
    }
}
