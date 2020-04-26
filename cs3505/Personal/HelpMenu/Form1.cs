using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HelpMenu
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            TreeNode selectedNode = treeView1.SelectedNode;
            string absolutePath = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
            switch (selectedNode.Text)
            {
                case "Formulas":
                    try { richTextBox1.LoadFile(absolutePath + "\\HelpFiles\\FormulaHelp.txt", RichTextBoxStreamType.PlainText); }
                    catch { richTextBox1.Text = "Cannot find help file"; }
                    break;
                case "Functions":
                    try { richTextBox1.LoadFile(absolutePath + "\\HelpFiles\\FunctionHelp.txt", RichTextBoxStreamType.PlainText); }
                    catch { richTextBox1.Text = "Cannot find help file"; }
                    break;
            }
        }
    }
}
