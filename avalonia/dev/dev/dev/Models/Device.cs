using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ReactiveUI;
using Avalonia.Collections;

namespace dev.Models
{
    public enum Operation
    {
        ChangeState
    }

    public class GetDevice : ReactiveObject
    {
        public string? job_date { get; set; }
        public AvaloniaList<Device>? devices { get; set; }
    }
    public class Device : ReactiveObject
    {
        private string? _device_guid;
        private string? _index;
        private string? _state;
        private string? _ip;
        private string? _class;
        private string? _name;
        private string? _isnewdevice;
        private string? _mac;


        public string device_guid
        {
            get { return _device_guid; }
            set { _device_guid = value; this.RaiseAndSetIfChanged(ref _device_guid, value); }
        }

        public string index
        {
            get { return _index; }
            set { _index = value; this.RaiseAndSetIfChanged(ref _index, value); }
        }

        public string state
        {
            get { return _state; }
            set { _state = value; this.RaiseAndSetIfChanged(ref _state, value); }
        }
        public string ip
        {
            get { return _ip; }
            set { _ip = value; this.RaiseAndSetIfChanged(ref _ip, value); }
        }

        public string @class
        {
            get { return _class; }
            set { _class = value; this.RaiseAndSetIfChanged(ref _class, value); }
        }
        public string name
        {
            get { return _name; }
            set { _name = value; this.RaiseAndSetIfChanged(ref _name, value); }
        }
        public string isnewdevice
        {
            get { return _isnewdevice; }
            set { _isnewdevice = value; this.RaiseAndSetIfChanged(ref _isnewdevice, value); }
        }
        public string mac
        {
            get { return _mac; }
            set { _mac = value; this.RaiseAndSetIfChanged(ref _mac, value); }
        }
    }
}
