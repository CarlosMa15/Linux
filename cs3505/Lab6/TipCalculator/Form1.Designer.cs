namespace TipCalculator
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
            this.tipComputeButton = new System.Windows.Forms.Button();
            this.billTipLabel = new System.Windows.Forms.Label();
            this.amountTextBox = new System.Windows.Forms.TextBox();
            this.totalTipTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.percentageTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // tipComputeButton
            // 
            this.tipComputeButton.Font = new System.Drawing.Font("Georgia", 13.875F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tipComputeButton.Location = new System.Drawing.Point(47, 480);
            this.tipComputeButton.Name = "tipComputeButton";
            this.tipComputeButton.Size = new System.Drawing.Size(252, 117);
            this.tipComputeButton.TabIndex = 0;
            this.tipComputeButton.Text = "Compute Tip";
            this.tipComputeButton.UseVisualStyleBackColor = true;
            this.tipComputeButton.Click += new System.EventHandler(this.tipComputeButton_Click);
            // 
            // billTipLabel
            // 
            this.billTipLabel.AutoSize = true;
            this.billTipLabel.Font = new System.Drawing.Font("Times New Roman", 13.875F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.billTipLabel.Location = new System.Drawing.Point(40, 62);
            this.billTipLabel.Name = "billTipLabel";
            this.billTipLabel.Size = new System.Drawing.Size(272, 42);
            this.billTipLabel.TabIndex = 1;
            this.billTipLabel.Text = "Enter Total Bill:";
            // 
            // amountTextBox
            // 
            this.amountTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.amountTextBox.Location = new System.Drawing.Point(507, 24);
            this.amountTextBox.Name = "amountTextBox";
            this.amountTextBox.Size = new System.Drawing.Size(458, 80);
            this.amountTextBox.TabIndex = 2;
            this.amountTextBox.TextChanged += new System.EventHandler(this.amountTextBox_TextChanged);
            // 
            // totalTipTextBox
            // 
            this.totalTipTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.totalTipTextBox.Location = new System.Drawing.Point(507, 517);
            this.totalTipTextBox.Name = "totalTipTextBox";
            this.totalTipTextBox.Size = new System.Drawing.Size(458, 80);
            this.totalTipTextBox.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe Script", 13.875F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(36, 277);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(327, 61);
            this.label1.TabIndex = 4;
            this.label1.Text = "Tip Percentage:";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // percentageTextBox
            // 
            this.percentageTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.percentageTextBox.Location = new System.Drawing.Point(507, 258);
            this.percentageTextBox.Name = "percentageTextBox";
            this.percentageTextBox.Size = new System.Drawing.Size(458, 80);
            this.percentageTextBox.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.875F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(36, 356);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(350, 61);
            this.label2.TabIndex = 6;
            this.label2.Text = "(Default 20%)";
            // 
            // Form1
            // 
            this.AcceptButton = this.tipComputeButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1074, 669);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.percentageTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.totalTipTextBox);
            this.Controls.Add(this.amountTextBox);
            this.Controls.Add(this.billTipLabel);
            this.Controls.Add(this.tipComputeButton);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button tipComputeButton;
        private System.Windows.Forms.Label billTipLabel;
        private System.Windows.Forms.TextBox amountTextBox;
        private System.Windows.Forms.TextBox totalTipTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox percentageTextBox;
        private System.Windows.Forms.Label label2;
    }
}

