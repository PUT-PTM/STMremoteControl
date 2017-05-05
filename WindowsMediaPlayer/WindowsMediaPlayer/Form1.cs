using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Windows.Input;
using System.Runtime.InteropServices;

namespace WindowsMediaPlayer
{
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

        public static void KeyDown(Keys vKey)
        {
            keybd_event((byte)vKey, 0, KEYEVENTF_EXTENDEDKEY, 0);
        }

        public static void KeyUp(Keys vKey)
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
 
    public partial class Form1 : Form
    {
        VCPPort _vcpport;
        public Form1()
        {
            InitializeComponent();
            doThread();
        }
        
        private void doThread()
        {
            _vcpport = new VCPPort();
            Thread thr = new Thread(control);
            thr.Start();
        }
        private void control()
        {
            bool funkcja = false;
            int indata = _vcpport._indata;
            var pl = axWindowsMediaPlayer1.playlistCollection.newPlaylist("playlist");
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\1.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\2.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\3.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\4.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\5.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\6.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\7.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\8.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\9.mp3"));
            pl.appendItem(axWindowsMediaPlayer1.newMedia(@"C:\Users\E Kaczmarek\Music\10.mp3"));
            axWindowsMediaPlayer1.currentPlaylist = pl;
            axWindowsMediaPlayer1.Ctlcontrols.stop();
            indata = _vcpport._indata;
            while (indata >= 0) 
            {
                indata = _vcpport._indata;
                if (funkcja == true)
                {
                    _vcpport._indata = 0;
                    funkcja = false;
                }
                switch (indata)
                {
                    case 69:
                        {
                            float vol = VolumeUtilities.GetMasterVolume();
                            if (vol > 0)
                            {
                                KeyboardSend.KeyUp(Keys.VolumeDown);
                                KeyboardSend.KeyDown(Keys.VolumeDown);
                            }
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 70:
                        {
                            Computer.SetSuspendState(true, true, true);
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 71:
                        {
                            float vol = VolumeUtilities.GetMasterVolume();
                            if (vol < 1)
                            {
                                KeyboardSend.KeyDown(Keys.VolumeUp);
                                KeyboardSend.KeyUp(Keys.VolumeUp);
                            }
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 68:
                        {
                            axWindowsMediaPlayer1.Ctlcontrols.previous();
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 64:
                        {
                            axWindowsMediaPlayer1.Ctlcontrols.next();
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 67:
                        {
                            axWindowsMediaPlayer1.Ctlcontrols.play();
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 7:
                        {
                            axWindowsMediaPlayer1.settings.volume -= 1;
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 21:
                        {
                            axWindowsMediaPlayer1.settings.volume += 1;
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 9:
                        {
                            KeyboardSend.KeyDown(Keys.F17);
                            KeyboardSend.KeyUp(Keys.F17);
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 22:
                        {
                            KeyboardSend.KeyDown(Keys.F16);
                            KeyboardSend.KeyUp(Keys.F16);
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 25:
                        {
                            KeyboardSend.KeyDown(Keys.F15);
                            KeyboardSend.KeyUp(Keys.F15);
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    case 13:
                        {
                            KeyboardSend.KeyDown(Keys.F14);
                            KeyboardSend.KeyUp(Keys.F14);
                            indata = 0;
                            funkcja = true;
                            break;
                        }
                    default:
                        break;
                }
            }
        }
    }
}
