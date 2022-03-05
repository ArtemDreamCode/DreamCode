using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ReactiveUI;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace testServ.Models
{
    public class Device : ReactiveObject
    {
        public string? device_guid { get; set; }
        public string? state { get; set; }
        public string? ip { get; set; }
        public string? Class { get; set; }
        public string? name { get; set; }
        public string? isnewdevice { get; set; }
        public string? mac { get; set; }
    }

}
