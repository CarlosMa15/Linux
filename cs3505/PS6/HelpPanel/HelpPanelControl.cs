using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Configuration;

namespace SS
{
    public partial class HelpPanel : UserControl
    {
        /// <summary>
        /// A help panel that has a left tree navigation to navigate information about
        /// the spreadsheet program.
        /// </summary>
        public HelpPanel()
        {
            InitializeComponent();
        }

        /// <summary>
        /// This listener changes the displayed text in the help panel to the appropriate 
        /// selection in the tree. The text data is saved 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var appSettings = ConfigurationManager.AppSettings;
            TreeNode selectedNode = treeView1.SelectedNode;
            string absolutePath = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.Parent.FullName;
            switch (selectedNode.Text)
            {
                case "Cells":
                    helpText.Text = appSettings[selectedNode.Text];
                    break;

                case "Formulas":
                    helpText.Text = appSettings[selectedNode.Text];
                    break;

                case "Functions":
                    helpText.Text = appSettings[selectedNode.Text];
                    break;

                case "Save/Open":
                    helpText.Text = appSettings[selectedNode.Text];
                    break;
            }
        }
    }
}
