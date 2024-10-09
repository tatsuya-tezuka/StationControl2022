namespace DispatcherGUI
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.label1 = new System.Windows.Forms.Label();
            this.textBox50001 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox50002 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox50003 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxSvIp = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBoxPipeState = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxPipeCount = new System.Windows.Forms.TextBox();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.ToolStripMenuItemSetting = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemStart = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemStop = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemIns = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemUnins = new System.Windows.Forms.ToolStripMenuItem();
            this.labelServiceText = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.textBoxVersion = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(126, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Port:50001  (監視データ)";
            // 
            // textBox50001
            // 
            this.textBox50001.BackColor = System.Drawing.Color.White;
            this.textBox50001.Location = new System.Drawing.Point(148, 30);
            this.textBox50001.Name = "textBox50001";
            this.textBox50001.ReadOnly = true;
            this.textBox50001.Size = new System.Drawing.Size(147, 19);
            this.textBox50001.TabIndex = 2;
            this.textBox50001.Text = "-----";
            this.textBox50001.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(126, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Port:50002  (履歴データ)";
            // 
            // textBox50002
            // 
            this.textBox50002.BackColor = System.Drawing.Color.White;
            this.textBox50002.Location = new System.Drawing.Point(148, 55);
            this.textBox50002.Name = "textBox50002";
            this.textBox50002.ReadOnly = true;
            this.textBox50002.Size = new System.Drawing.Size(147, 19);
            this.textBox50002.TabIndex = 4;
            this.textBox50002.Text = "-----";
            this.textBox50002.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 83);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(133, 12);
            this.label3.TabIndex = 5;
            this.label3.Text = "Port:50003  (制御コマンド)";
            // 
            // textBox50003
            // 
            this.textBox50003.BackColor = System.Drawing.Color.White;
            this.textBox50003.Location = new System.Drawing.Point(148, 80);
            this.textBox50003.Name = "textBox50003";
            this.textBox50003.ReadOnly = true;
            this.textBox50003.Size = new System.Drawing.Size(147, 19);
            this.textBox50003.TabIndex = 6;
            this.textBox50003.Text = "-----";
            this.textBox50003.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 126);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(55, 12);
            this.label4.TabIndex = 7;
            this.label4.Text = "サーバーIP";
            // 
            // textBoxSvIp
            // 
            this.textBoxSvIp.BackColor = System.Drawing.Color.White;
            this.textBoxSvIp.Location = new System.Drawing.Point(96, 123);
            this.textBoxSvIp.Name = "textBoxSvIp";
            this.textBoxSvIp.ReadOnly = true;
            this.textBoxSvIp.Size = new System.Drawing.Size(199, 19);
            this.textBoxSvIp.TabIndex = 8;
            this.textBoxSvIp.Text = "-----";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.textBox50001);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBoxSvIp);
            this.groupBox1.Controls.Add(this.textBox50002);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBox50003);
            this.groupBox1.Location = new System.Drawing.Point(4, 29);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(308, 163);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "TCP通信状態";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.textBoxPipeState);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.textBoxPipeCount);
            this.groupBox2.Location = new System.Drawing.Point(318, 29);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(266, 82);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Pipe通信状態";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 33);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 1;
            this.label6.Text = "通信状態";
            // 
            // textBoxPipeState
            // 
            this.textBoxPipeState.BackColor = System.Drawing.Color.White;
            this.textBoxPipeState.Location = new System.Drawing.Point(113, 30);
            this.textBoxPipeState.Name = "textBoxPipeState";
            this.textBoxPipeState.ReadOnly = true;
            this.textBoxPipeState.Size = new System.Drawing.Size(147, 19);
            this.textBoxPipeState.TabIndex = 2;
            this.textBoxPipeState.Text = "-----";
            this.textBoxPipeState.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 58);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(41, 12);
            this.label8.TabIndex = 3;
            this.label8.Text = "接続数";
            // 
            // textBoxPipeCoount
            // 
            this.textBoxPipeCount.BackColor = System.Drawing.Color.White;
            this.textBoxPipeCount.Location = new System.Drawing.Point(113, 55);
            this.textBoxPipeCount.Name = "textBoxPipeCoount";
            this.textBoxPipeCount.ReadOnly = true;
            this.textBoxPipeCount.Size = new System.Drawing.Size(147, 19);
            this.textBoxPipeCount.TabIndex = 4;
            this.textBoxPipeCount.Text = "-----";
            this.textBoxPipeCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(509, 198);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 0;
            this.buttonCancel.Text = "閉じる";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemSetting,
            this.ToolStripMenuItemStart,
            this.ToolStripMenuItemStop,
            this.ToolStripMenuItemIns,
            this.ToolStripMenuItemUnins});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(595, 26);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // ToolStripMenuItemSetting
            // 
            this.ToolStripMenuItemSetting.Image = global::DispatcherGUI.Properties.Resources.wrench;
            this.ToolStripMenuItemSetting.Name = "ToolStripMenuItemSetting";
            this.ToolStripMenuItemSetting.Size = new System.Drawing.Size(60, 22);
            this.ToolStripMenuItemSetting.Text = "設定";
            this.ToolStripMenuItemSetting.Click += new System.EventHandler(this.ToolStripMenuItemSetting_Click);
            // 
            // ToolStripMenuItemStart
            // 
            this.ToolStripMenuItemStart.Image = global::DispatcherGUI.Properties.Resources.control_play_blue;
            this.ToolStripMenuItemStart.Name = "ToolStripMenuItemStart";
            this.ToolStripMenuItemStart.Size = new System.Drawing.Size(108, 22);
            this.ToolStripMenuItemStart.Text = "サービス開始";
            this.ToolStripMenuItemStart.Click += new System.EventHandler(this.ToolStripMenuItemStart_Click);
            // 
            // ToolStripMenuItemStop
            // 
            this.ToolStripMenuItemStop.Image = global::DispatcherGUI.Properties.Resources.control_stop_blue;
            this.ToolStripMenuItemStop.Name = "ToolStripMenuItemStop";
            this.ToolStripMenuItemStop.Size = new System.Drawing.Size(108, 22);
            this.ToolStripMenuItemStop.Text = "サービス停止";
            this.ToolStripMenuItemStop.Click += new System.EventHandler(this.ToolStripMenuItemStop_Click);
            // 
            // ToolStripMenuItemIns
            // 
            this.ToolStripMenuItemIns.Image = global::DispatcherGUI.Properties.Resources.cog_add;
            this.ToolStripMenuItemIns.Name = "ToolStripMenuItemIns";
            this.ToolStripMenuItemIns.Size = new System.Drawing.Size(108, 22);
            this.ToolStripMenuItemIns.Text = "サービス登録";
            this.ToolStripMenuItemIns.Click += new System.EventHandler(this.ToolStripMenuItemIns_Click);
            // 
            // ToolStripMenuItemUnins
            // 
            this.ToolStripMenuItemUnins.Image = global::DispatcherGUI.Properties.Resources.cog_delete;
            this.ToolStripMenuItemUnins.Name = "ToolStripMenuItemUnins";
            this.ToolStripMenuItemUnins.Size = new System.Drawing.Size(108, 22);
            this.ToolStripMenuItemUnins.Text = "サービス削除";
            this.ToolStripMenuItemUnins.Click += new System.EventHandler(this.ToolStripMenuItemUnins_Click);
            // 
            // labelServiceText
            // 
            this.labelServiceText.AutoSize = true;
            this.labelServiceText.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.labelServiceText.Location = new System.Drawing.Point(2, 198);
            this.labelServiceText.Name = "labelServiceText";
            this.labelServiceText.Size = new System.Drawing.Size(0, 13);
            this.labelServiceText.TabIndex = 15;
            // 
            // timer1
            // 
            this.timer1.Interval = 500;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.textBoxVersion);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Location = new System.Drawing.Point(318, 117);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(265, 75);
            this.groupBox3.TabIndex = 16;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Service情報";
            // 
            // textBoxVersion
            // 
            this.textBoxVersion.BackColor = System.Drawing.Color.White;
            this.textBoxVersion.Location = new System.Drawing.Point(112, 22);
            this.textBoxVersion.Name = "textBoxVersion";
            this.textBoxVersion.ReadOnly = true;
            this.textBoxVersion.Size = new System.Drawing.Size(147, 19);
            this.textBoxVersion.TabIndex = 5;
            this.textBoxVersion.Text = "-----";
            this.textBoxVersion.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 25);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 12);
            this.label5.TabIndex = 0;
            this.label5.Text = "Version";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(595, 230);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.labelServiceText);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "Dispatcher Monitor & Setting";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox50001;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox50002;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox50003;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxSvIp;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxPipeState;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxPipeCount;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemSetting;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemStart;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemStop;
        private System.Windows.Forms.Label labelServiceText;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemIns;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemUnins;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox textBoxVersion;
        private System.Windows.Forms.Label label5;
    }
}

