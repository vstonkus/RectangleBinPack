using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using rbp;

namespace zetBlock
    {
    public partial class Form1 : Form
        {
        //private rbp.MaxRectsBinPack

        public Form1 ()
            {
            InitializeComponent ();

            binGroupBindingSource.Add (new BinGroup (1, 20, 30, 2));

            }

        private void ButtonCalculate_Click (object sender, EventArgs e)
            {
            GuillotineBinPack gbp = new GuillotineBinPack(10, 10);
            gbp.Insert (2, 2, true, FreeRectChoiceHeuristic.RectBestAreaFit,
                GuillotineSplitHeuristic.SplitShorterLeftoverAxis);
            binBindingSource.Add(new Bin ((int)(gbp.Occupancy () * 100), 0, 0));
            //binGroupBindingSource[0]
            //Recta
            }




        private void MatchBin()
            {
            
            }


        }
    }
