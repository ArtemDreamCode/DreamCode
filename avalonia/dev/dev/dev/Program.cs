using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Dialogs;
using Avalonia.ReactiveUI;
using System;
using System.Globalization;
using System.Linq;
using System.Threading;

namespace dev
{
    internal class Program
    {
        // Initialization code. Don't use any Avalonia, third-party APIs or any
        // SynchronizationContext-reliant code before AppMain is called: things aren't initialized
        // yet and stuff might break.
        [STAThread]
        //public static void Main(string[] args) => BuildAvaloniaApp()
        //    .StartWithClassicDesktopLifetime(args);

        public static int Main(string[] args)
        {
            double GetScaling()
            {
                var idx = Array.IndexOf(args, "--scaling");
                if (idx != 0 && args.Length > idx + 1 &&
                    double.TryParse(args[idx + 1], NumberStyles.Any, CultureInfo.InvariantCulture, out var scaling))
                    return scaling;
                return 1;
            }

            var builder = BuildAvaloniaApp();
            
            if (args.Contains("--fbdev"))
            {
                SilenceConsole();
                return builder.StartLinuxFbDev(args, scaling: GetScaling());
            }

            return builder.StartWithClassicDesktopLifetime(args);
        }

        

        private static void SilenceConsole()
        {
            new Thread(() =>
            {
                Console.CursorVisible = false;
                while (true)
                    Console.ReadKey(true);
            })
            { IsBackground = true }.Start();
        }

        // Avalonia configuration, don't remove; also used by visual designer.
        public static AppBuilder BuildAvaloniaApp()
            => AppBuilder.Configure<App>()
                .UsePlatformDetect()
                .With(new X11PlatformOptions
                {
                    EnableMultiTouch = true,
                    UseDBusMenu = true
                })
                .With(new Win32PlatformOptions
                {
                    EnableMultitouch = true,
                    AllowEglInitialization = true
                })
                .UseSkia()
                .UseReactiveUI()
                .UseManagedSystemDialogs();
    }
}
