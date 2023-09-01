using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ReadRFID
{
    public partial class FrmMain : Form
    {
        private SerialPort RFID;
        private string DispString;
        public static readonly List<string> SupportedBaudRates = new List<string>
        {
            "300",
            "600",
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600"
        };
        public FrmMain()
        {
            InitializeComponent();
        }

        public void GetListComPort()
        {
            string[] ports = SerialPort.GetPortNames();          
            foreach (var port in ports)
            {
                cb_comport.Items.Add(port);
            }               
            if(ports.Length > 0)
            {
                cb_comport.SelectedIndex = 0;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GetListComPort();
            cb_baud_rate.DataSource = SupportedBaudRates;
            cb_baud_rate.SelectedIndex = 5;


            var listHandShake = Enum.GetValues(typeof(Handshake))
              .Cast<Handshake>()
              .Select(t => new
              {
                  Name = t.ToString()
              }).ToList();

            cb_handshake.DataSource = listHandShake;
            cb_handshake.ValueMember = "Name";
            cb_handshake.DisplayMember = "Name";
          
            var listParity = Enum.GetValues(typeof(Parity))
               .Cast<Parity>()
               .Select(t => new 
               {                 
                   Name = t.ToString()
               }).ToList();

            cb_parity.DataSource = listParity;
            cb_parity.ValueMember = "Name";
            cb_parity.DisplayMember = "Name";

            var listStopBits = Enum.GetValues(typeof(StopBits))
               .Cast<StopBits>()
               .Select(t => new
               {
                   Name = t.ToString()
               }).ToList();

            
            cb_stopbits.DataSource = listStopBits;
            cb_stopbits.ValueMember = "Name";
            cb_stopbits.DisplayMember = "Name";
            cb_stopbits.SelectedIndex = 1;
        }

        private void btn_connect_Click(object sender, EventArgs e)
        {
            if(btn_connect.Text == "Connect")
            {
                if (string.IsNullOrEmpty(cb_comport.Text))
                {
                    MessageBox.Show("Chưa chọn cổng COM kết nối, bạn ơi!", "Thông báo", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                var parity = cb_parity.Text;
                var stopbits = cb_stopbits.Text;
                var handshake = cb_handshake.Text;

                RFID = new SerialPort();
                RFID.PortName = cb_comport.Text ;
                RFID.BaudRate = Convert.ToInt32(cb_baud_rate.Text);
                RFID.DataBits = Convert.ToInt32(txt_databits.Value);
                RFID.Parity = (Parity)Enum.Parse(typeof(Parity), parity, true);
                RFID.StopBits = (StopBits)Enum.Parse(typeof(StopBits), stopbits, true);
                RFID.ReadTimeout = Convert.ToInt32(txt_read_timeout.Value);
                RFID.Handshake = (Handshake)Enum.Parse(typeof(Handshake), handshake, true);
                try
                {
                    RFID.Open();
                    if (RFID.IsOpen)
                    {
                        DispString = "";
                        txt_tag.Text = "";
                    }
                    else
                    {
                        RFID.Close();
                    }
                    RFID.DataReceived += new SerialDataReceivedEventHandler(RFID_DataReceived);
                    btn_connect.Text = "Disconnect";
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Thông báo", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                RFID.Close();
                btn_connect.Text = "Connect";
            }
            
           
        }

        private void RFID_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {          
            DispString = RFID.ReadExisting();
            this.Invoke(new EventHandler(DisplayText));
        }
        private void DisplayText(object sender, EventArgs e)
        {
            txt_tag.AppendText(DispString);
        }
    }
}
