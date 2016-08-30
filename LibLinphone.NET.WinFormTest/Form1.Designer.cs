namespace LibLinphone.NET.WinFormTest
{
    partial class Form1
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
            this.videoViewerPanel = new System.Windows.Forms.Panel();
            this.consoleLog = new System.Windows.Forms.TextBox();
            this.btnCall = new System.Windows.Forms.Button();
            this.calleeAddress = new System.Windows.Forms.TextBox();
            this.btnLogin = new System.Windows.Forms.Button();
            this.localAddress = new System.Windows.Forms.TextBox();
            this.password = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // videoViewerPanel
            // 
            this.videoViewerPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.videoViewerPanel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.videoViewerPanel.Location = new System.Drawing.Point(349, 35);
            this.videoViewerPanel.Name = "videoViewerPanel";
            this.videoViewerPanel.Size = new System.Drawing.Size(597, 479);
            this.videoViewerPanel.TabIndex = 0;
            // 
            // consoleLog
            // 
            this.consoleLog.Location = new System.Drawing.Point(27, 173);
            this.consoleLog.Multiline = true;
            this.consoleLog.Name = "consoleLog";
            this.consoleLog.Size = new System.Drawing.Size(316, 341);
            this.consoleLog.TabIndex = 1;
            // 
            // btnCall
            // 
            this.btnCall.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnCall.Location = new System.Drawing.Point(272, 108);
            this.btnCall.Name = "btnCall";
            this.btnCall.Size = new System.Drawing.Size(71, 36);
            this.btnCall.TabIndex = 2;
            this.btnCall.Text = "Call";
            this.btnCall.UseVisualStyleBackColor = true;
            this.btnCall.Click += new System.EventHandler(this.btnCall_Click);
            // 
            // calleeAddress
            // 
            this.calleeAddress.Location = new System.Drawing.Point(67, 117);
            this.calleeAddress.Name = "calleeAddress";
            this.calleeAddress.Size = new System.Drawing.Size(199, 20);
            this.calleeAddress.TabIndex = 3;
            this.calleeAddress.Text = "sip:<callee_user_name>@<host>:<port>";
            // 
            // btnLogin
            // 
            this.btnLogin.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnLogin.Location = new System.Drawing.Point(272, 44);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(71, 46);
            this.btnLogin.TabIndex = 2;
            this.btnLogin.Text = "Login";
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // localAddress
            // 
            this.localAddress.Location = new System.Drawing.Point(84, 44);
            this.localAddress.Name = "localAddress";
            this.localAddress.Size = new System.Drawing.Size(182, 20);
            this.localAddress.TabIndex = 3;
            this.localAddress.Text = "sip:<user_name>@<host>:<port>";
            // 
            // password
            // 
            this.password.Location = new System.Drawing.Point(84, 70);
            this.password.Name = "password";
            this.password.Size = new System.Drawing.Size(182, 20);
            this.password.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(25, 51);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "User";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 73);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Password";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 120);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Callee";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(974, 548);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.password);
            this.Controls.Add(this.localAddress);
            this.Controls.Add(this.calleeAddress);
            this.Controls.Add(this.btnLogin);
            this.Controls.Add(this.btnCall);
            this.Controls.Add(this.consoleLog);
            this.Controls.Add(this.videoViewerPanel);
            this.Name = "Form1";
            this.Text = "LibLinphone Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel videoViewerPanel;
        private System.Windows.Forms.TextBox consoleLog;
        private System.Windows.Forms.Button btnCall;
        private System.Windows.Forms.TextBox calleeAddress;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.TextBox localAddress;
        private System.Windows.Forms.TextBox password;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
    }
}

