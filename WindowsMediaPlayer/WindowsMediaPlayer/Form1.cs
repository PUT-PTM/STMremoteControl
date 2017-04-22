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

namespace WindowsMediaPlayer
{
    public partial class Form1 : Form
    {
        VCPPort _vcpport;
        public Form1()
        {
            InitializeComponent();
        }
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            axWindowsMediaPlayer1.URL = paths[listBox1.SelectedIndex];
        }
        string[] files, paths;
        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                files = openFileDialog1.SafeFileNames;
                paths = openFileDialog1.FileNames;
                for (int i = 0; i < files.Length; i++)
                {
                    listBox1.Items.Add(files[i]);
                }
            }
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
            bool playing = false;
            while (true)
            {
                int indata = _vcpport._indata;
                switch (indata)
                {
                    case 67:
                        {
                            if (playing == false)
                            {
                                axWindowsMediaPlayer1.Ctlcontrols.play();
                                playing = true;
                            }
                            else
                            {
                                axWindowsMediaPlayer1.Ctlcontrols.pause();
                                playing = false;
                            }
                            indata = 0;
                            break;
                        }
                    case 7:
                        {
                            axWindowsMediaPlayer1.settings.volume -= 1;
                            indata = 0;
                            break;
                        }
                    case 21:
                        {
                            axWindowsMediaPlayer1.settings.volume += 1;
                            indata = 0;
                            break;
                        }
                    case 68:
                        {
                            axWindowsMediaPlayer1.Ctlcontrols.next();
                            indata = 0;
                            break;
                        }
                    case 64:
                        {
                            axWindowsMediaPlayer1.Ctlcontrols.previous();
                            indata = 0;
                            break;
                        }
                    default:
                        break;
                }
                
            }
        }
    }
}
