using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;

namespace WindowsMediaPlayer
{
    public class VCPPort
    {
        public static SerialPort _serialPort;
        static int _lastReceived;
        public int _indata { get; set; }
        public VCPPort()
        {
            _serialPort = new SerialPort();
            _serialPort.PortName = "COM5";
            _serialPort.BaudRate = 115200;
            _serialPort.Parity = (Parity)Enum.Parse(typeof(Parity), _serialPort.Parity.ToString(), true);
            _serialPort.DataBits = 8;
            _serialPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), _serialPort.StopBits.ToString(), true);
            _serialPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), _serialPort.Handshake.ToString(), true);

            // Set the read/write timeouts
            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            _serialPort.Open();
        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            int test = _serialPort.ReadByte();
            if (test > 0)
            {
                if (_lastReceived != test)
                {
                    _indata = test;
                    _lastReceived = _indata;
                    Console.WriteLine("Data Received:");
                    Console.WriteLine(_indata);
                }
                else
                    _lastReceived = 0;
            }
            else
            {
                _lastReceived = 0;
            }
        }
    }
}
