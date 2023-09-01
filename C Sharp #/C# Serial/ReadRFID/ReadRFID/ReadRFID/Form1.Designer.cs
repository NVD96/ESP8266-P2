namespace ReadRFID
{
    partial class FrmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.cb_comport = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txt_databits = new System.Windows.Forms.NumericUpDown();
            this.txt_read_timeout = new System.Windows.Forms.NumericUpDown();
            this.cb_baud_rate = new System.Windows.Forms.ComboBox();
            this.btn_connect = new System.Windows.Forms.Button();
            this.txt_tag = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.cb_parity = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cb_stopbits = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.cb_handshake = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.txt_databits)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.txt_read_timeout)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(64, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 18);
            this.label1.TabIndex = 0;
            this.label1.Text = "COM port:";
            // 
            // cb_comport
            // 
            this.cb_comport.FormattingEnabled = true;
            this.cb_comport.Location = new System.Drawing.Point(160, 46);
            this.cb_comport.Name = "cb_comport";
            this.cb_comport.Size = new System.Drawing.Size(177, 26);
            this.cb_comport.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(64, 87);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 18);
            this.label2.TabIndex = 2;
            this.label2.Text = "Baud rate: ";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(64, 239);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 18);
            this.label3.TabIndex = 3;
            this.label3.Text = "Data bits:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(34, 271);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(100, 18);
            this.label4.TabIndex = 4;
            this.label4.Text = "Read timeout:";
            // 
            // txt_databits
            // 
            this.txt_databits.Location = new System.Drawing.Point(160, 237);
            this.txt_databits.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.txt_databits.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.txt_databits.Name = "txt_databits";
            this.txt_databits.Size = new System.Drawing.Size(86, 24);
            this.txt_databits.TabIndex = 5;
            this.txt_databits.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            // 
            // txt_read_timeout
            // 
            this.txt_read_timeout.Location = new System.Drawing.Point(160, 271);
            this.txt_read_timeout.Maximum = new decimal(new int[] {
            9999999,
            0,
            0,
            0});
            this.txt_read_timeout.Name = "txt_read_timeout";
            this.txt_read_timeout.Size = new System.Drawing.Size(86, 24);
            this.txt_read_timeout.TabIndex = 6;
            this.txt_read_timeout.Value = new decimal(new int[] {
            200,
            0,
            0,
            0});
            // 
            // cb_baud_rate
            // 
            this.cb_baud_rate.FormattingEnabled = true;
            this.cb_baud_rate.Location = new System.Drawing.Point(160, 85);
            this.cb_baud_rate.Name = "cb_baud_rate";
            this.cb_baud_rate.Size = new System.Drawing.Size(177, 26);
            this.cb_baud_rate.TabIndex = 7;
            // 
            // btn_connect
            // 
            this.btn_connect.Location = new System.Drawing.Point(207, 309);
            this.btn_connect.Name = "btn_connect";
            this.btn_connect.Size = new System.Drawing.Size(130, 34);
            this.btn_connect.TabIndex = 8;
            this.btn_connect.Text = "Connect";
            this.btn_connect.UseVisualStyleBackColor = true;
            this.btn_connect.Click += new System.EventHandler(this.btn_connect_Click);
            // 
            // txt_tag
            // 
            this.txt_tag.Location = new System.Drawing.Point(439, 46);
            this.txt_tag.Multiline = true;
            this.txt_tag.Name = "txt_tag";
            this.txt_tag.Size = new System.Drawing.Size(297, 297);
            this.txt_tag.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(436, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(108, 18);
            this.label5.TabIndex = 10;
            this.label5.Text = "Data Received:";
            // 
            // cb_parity
            // 
            this.cb_parity.FormattingEnabled = true;
            this.cb_parity.Location = new System.Drawing.Point(160, 160);
            this.cb_parity.Name = "cb_parity";
            this.cb_parity.Size = new System.Drawing.Size(177, 26);
            this.cb_parity.TabIndex = 12;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(81, 162);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(49, 18);
            this.label6.TabIndex = 11;
            this.label6.Text = "Parity:";
            // 
            // cb_stopbits
            // 
            this.cb_stopbits.FormattingEnabled = true;
            this.cb_stopbits.Location = new System.Drawing.Point(160, 196);
            this.cb_stopbits.Name = "cb_stopbits";
            this.cb_stopbits.Size = new System.Drawing.Size(177, 26);
            this.cb_stopbits.TabIndex = 14;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(64, 198);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(66, 18);
            this.label7.TabIndex = 13;
            this.label7.Text = "Stopbits:";
            // 
            // cb_handshake
            // 
            this.cb_handshake.FormattingEnabled = true;
            this.cb_handshake.Location = new System.Drawing.Point(160, 123);
            this.cb_handshake.Name = "cb_handshake";
            this.cb_handshake.Size = new System.Drawing.Size(177, 26);
            this.cb_handshake.TabIndex = 16;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(43, 126);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(91, 18);
            this.label8.TabIndex = 15;
            this.label8.Text = "Hand shake:";
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(760, 367);
            this.Controls.Add(this.cb_handshake);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.cb_stopbits);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.cb_parity);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txt_tag);
            this.Controls.Add(this.btn_connect);
            this.Controls.Add(this.cb_baud_rate);
            this.Controls.Add(this.txt_read_timeout);
            this.Controls.Add(this.txt_databits);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cb_comport);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "FrmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Read data RFID";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.txt_databits)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.txt_read_timeout)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cb_comport;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown txt_databits;
        private System.Windows.Forms.NumericUpDown txt_read_timeout;
        private System.Windows.Forms.ComboBox cb_baud_rate;
        private System.Windows.Forms.Button btn_connect;
        private System.Windows.Forms.TextBox txt_tag;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cb_parity;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox cb_stopbits;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox cb_handshake;
        private System.Windows.Forms.Label label8;
    }
}

