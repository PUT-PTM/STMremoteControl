using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Input;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using System.IO;

namespace Player
{
    class Program
    {

        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        const int SW_HIDE = 0;
        const int SW_SHOW = 5;

        static VCPPort _vcpport;
        static void Main(string[] args)
        {
            var handle = GetConsoleWindow();
            ShowWindow(handle, SW_HIDE);
            RegistryKey reg = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
            reg.SetValue("Player", @"C:\Users\E Kaczmarek\Desktop\Programowanie + LINUX\PTM\Player\Player\bin\Debug\Player.exe" );
            _vcpport = new VCPPort();
            Thread thr = new Thread(control);
            thr.Start();
        }

        public enum States
        {
            volumePCDown = 69,
            SleepComputer = 70,
            volumePCUp = 71,
            previousSong = 68,
            nextSong = 64,
            playSong = 67,
            volumeSongDown = 7,
            volumeSongUp = 21,

            stoplus = 25,
            dwiescieplus = 13,
            eq = 9
        }
        static private void control()
        {
            bool funkcja = false;
            int indata;
            indata = _vcpport._indata;
            while (indata >= 0)
            {
                indata = _vcpport._indata;
                if (funkcja == true)
                {
                    _vcpport._indata = 0;
                    funkcja = false;
                }

                switch ((States)_vcpport._indata)
                {
                    case States.volumePCDown:
                        {
                            indata = 0;
                            funkcja = true;
                            float vol = VolumeUtilities.GetMasterVolume();
                            if (vol > 0)
                            {
                                KeyboardSend.KeyUp(ConsoleKey.VolumeDown);
                                KeyboardSend.KeyDown(ConsoleKey.VolumeDown);
                            }
                            break;
                        }
                    case States.SleepComputer:
                        {
                            indata = 0;
                            funkcja = true;
                            Computer.SetSuspendState(true, true, true);
                            break;
                        }
                    case States.volumePCUp:
                        {
                            indata = 0;
                            funkcja = true;
                            float vol = VolumeUtilities.GetMasterVolume();
                            if (vol < 1)
                            {
                                KeyboardSend.KeyDown(ConsoleKey.VolumeUp);
                                KeyboardSend.KeyUp(ConsoleKey.VolumeUp);
                            }
                            break;
                        }
                    case States.previousSong:
                        {
                            indata = 0;
                            funkcja = true;
                            KeyboardSend.KeyDown(ConsoleKey.MediaPrevious);
                            KeyboardSend.KeyUp(ConsoleKey.MediaPrevious);
                            break;
                        }
                    case States.nextSong:
                        {
                            indata = 0;
                            funkcja = true;
                            KeyboardSend.KeyDown(ConsoleKey.MediaNext);
                            KeyboardSend.KeyUp(ConsoleKey.MediaNext);
                            break;
                        }
                    case States.playSong:
                        {
                            indata = 0;
                            funkcja = true;
                            KeyboardSend.KeyDown(ConsoleKey.MediaPlay);
                            KeyboardSend.KeyUp(ConsoleKey.MediaPlay);
                            break;
                        }
                    case States.volumeSongDown:
                        {
                            indata = 0;
                            funkcja = true;
                            float vol = VolumeUtilities.GetMasterVolume();
                            if (vol > 0)
                            {
                                KeyboardSend.KeyDown(ConsoleKey.VolumeDown);
                                KeyboardSend.KeyUp(ConsoleKey.VolumeDown);
                            }
                            break;
                        }
                    case States.volumeSongUp:
                        {
                            indata = 0;
                            funkcja = true;
                            float vol = VolumeUtilities.GetMasterVolume();
                            if (vol < 1)
                            {
                                KeyboardSend.KeyDown(ConsoleKey.VolumeUp);
                                KeyboardSend.KeyUp(ConsoleKey.VolumeUp);
                            }
                            break;
                        }
                    case States.eq:
                        {
                            indata = 0;
                            funkcja = true;
                            KeyboardSend.KeyDown(ConsoleKey.VolumeMute);
                            KeyboardSend.KeyUp(ConsoleKey.VolumeMute);
                            break;
                        }                  
                    default:
                        break;
                }
            }
        }
    }

    static class Computer
    {
        [DllImport("Powrprof.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        public static extern bool SetSuspendState(bool hiberate, bool forceCritical, bool disableWakeEvent);

    }
    static class KeyboardSend
    {
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);

        private const int KEYEVENTF_EXTENDEDKEY = 1;
        private const int KEYEVENTF_KEYUP = 2;

        public static void KeyDown(ConsoleKey vKey)
        {
            keybd_event((byte)vKey, 0, KEYEVENTF_EXTENDEDKEY, 0);
        }

        public static void KeyUp(ConsoleKey vKey)
        {
            keybd_event((byte)vKey, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }
    }
    public static class VolumeUtilities
    {
        public static float GetMasterVolume()
        {
            // get the speakers (1st render + multimedia) device
            IMMDeviceEnumerator deviceEnumerator = (IMMDeviceEnumerator)(new MMDeviceEnumerator());
            IMMDevice speakers;
            const int eRender = 0;
            const int eMultimedia = 1;
            deviceEnumerator.GetDefaultAudioEndpoint(eRender, eMultimedia, out speakers);

            object o;
            speakers.Activate(typeof(IAudioEndpointVolume).GUID, 0, IntPtr.Zero, out o);
            IAudioEndpointVolume aepv = (IAudioEndpointVolume)o;
            float volume = aepv.GetMasterVolumeLevelScalar();
            return volume;
        }

        [ComImport]
        [Guid("BCDE0395-E52F-467C-8E3D-C4579291692E")]
        private class MMDeviceEnumerator
        {
        }

        [Guid("5CDF2C82-841E-4546-9722-0CF74078229A"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        private interface IAudioEndpointVolume
        {
            void _VtblGap1_6();
            float GetMasterVolumeLevelScalar();
        }

        [Guid("A95664D2-9614-4F35-A746-DE8DB63617E6"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        private interface IMMDeviceEnumerator
        {
            void _VtblGap1_1();

            [PreserveSig]
            int GetDefaultAudioEndpoint(int dataFlow, int role, out IMMDevice ppDevice);
        }

        [Guid("D666063F-1587-4E43-81F1-B948E807363F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        private interface IMMDevice
        {
            [PreserveSig]
            int Activate([MarshalAs(UnmanagedType.LPStruct)] Guid iid, int dwClsCtx, IntPtr pActivationParams, [MarshalAs(UnmanagedType.IUnknown)] out object ppInterface);
        }
    }
}
