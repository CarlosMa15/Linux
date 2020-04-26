using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TipCalculator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void tipComputeButton_Click(object sender, EventArgs e)
        {
            double percent = 0;
            if (!double.TryParse(percentageTextBox.Text, out percent))
            {
                percent = .20;
            }
            String amount = amountTextBox.Text;
            double checkamount = 0;
            double.TryParse(amount, out checkamount);
            checkamount *= percent;
            totalTipTextBox.Text = "$" +checkamount.ToString();
        }

        private void amountTextBox_TextChanged(object sender, EventArgs e)
        {
            double amount = 0;
            if (!double.TryParse(amountTextBox.Text, out amount)) {
                tipComputeButton.Enabled = false;
            } else
            {
                tipComputeButton.Enabled = true;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
