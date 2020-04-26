using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Personal
{
    public partial class Presonal : Form
    {
        public Presonal()
        {
            InitializeComponent();
        }

        private void Presonal_FormClosing(object sender, FormClosingEventArgs e)
        {
            DialogResult dialog = MessageBox.Show("Do you really want to close?","Save and Exit",MessageBoxButtons.YesNo);
            if (dialog == DialogResult.No)
            {
                e.Cancel = true;
            }
        }

        private void nameTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Enter)
            {
                if(!Regex.IsMatch(nameTextBox.Text,"^[a-zA-Z]+$"))
                {
                    try
                    {
                        throw new ArgumentException();
                    }
                    catch (ArgumentException)
                    {
                        
                    }
                    nameTextBox.Text = "";
                    DialogResult dialog = MessageBox.Show("Only Letters Allowed", "Not Allow", MessageBoxButtons.OK);
                    
                }
            }
        }
    }
}
