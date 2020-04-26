namespace HelpMenuForm
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
            this.helpPanel1 = new SS.HelpPanel();
            this.SuspendLayout();
            // 
            // helpPanel1
            // 
            this.helpPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.helpPanel1.Location = new System.Drawing.Point(1, 12);
            this.helpPanel1.Name = "helpPanel1";
            this.helpPanel1.Size = new System.Drawing.Size(958, 576);
            this.helpPanel1.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(959, 588);
            this.Controls.Add(this.helpPanel1);
            this.Name = "Form1";
            this.Text = "HELP";
            this.ResumeLayout(false);

        }

        #endregion

        private SS.HelpPanel helpPanel1;
    }
}

