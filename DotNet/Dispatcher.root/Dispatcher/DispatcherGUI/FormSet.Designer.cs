namespace DispatcherGUI
{
    partial class FormSet
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormSet));
            this.buttonSave = new System.Windows.Forms.Button();
            this.buttonConcel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxSvIp = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // buttonSave
            // 
            this.buttonSave.Location = new System.Drawing.Point(167, 77);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(75, 23);
            this.buttonSave.TabIndex = 0;
            this.buttonSave.Text = "保存";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonConcel
            // 
            this.buttonConcel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonConcel.Location = new System.Drawing.Point(248, 77);
            this.buttonConcel.Name = "buttonConcel";
            this.buttonConcel.Size = new System.Drawing.Size(75, 23);
            this.buttonConcel.TabIndex = 1;
            this.buttonConcel.Text = "閉じる";
            this.buttonConcel.UseVisualStyleBackColor = true;
            this.buttonConcel.Click += new System.EventHandler(this.buttonConcel_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "サーバーIPアドレス";
            // 
            // textBoxSvIp
            // 
            this.textBoxSvIp.Location = new System.Drawing.Point(109, 17);
            this.textBoxSvIp.Name = "textBoxSvIp";
            this.textBoxSvIp.Size = new System.Drawing.Size(210, 19);
            this.textBoxSvIp.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 45);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(83, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "Debug Log出力";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Disable",
            "Enable"});
            this.comboBox1.Location = new System.Drawing.Point(109, 42);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 20);
            this.comboBox1.TabIndex = 5;
            // 
            // FormSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonConcel;
            this.ClientSize = new System.Drawing.Size(331, 112);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxSvIp);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonConcel);
            this.Controls.Add(this.buttonSave);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormSet";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "設定";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Button buttonConcel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxSvIp;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBox1;
    }
}