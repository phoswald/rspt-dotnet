using System;
using System.Windows.Forms;
using Parsers;

namespace Calculator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void eval_Click(object sender, EventArgs e)
        {            
            try {
                CalculatorParser p = new CalculatorParser();
                string input  = _input.Text;
                string output;
                int    error;
                if(p.Parse_ROOT(input.ToCharArray(), out output, out error)) {
                    _output. Text = output;
                    _message.Text = "";
                } else {
                    string next = input.Length-error <= 50 ? input.Substring(error, input.Length-error) : input.Substring(error, 50) + "...";
                    _output. Text = "ERROR";
                    _message.Text = "Error at offset " + (error+1) + ": Cannot handle '" + next + "'.";
                }
            } catch(Exception ex) {
                _output. Text = "EXCEPTION";
                _message.Text = ex.ToString();
            }
        }
    }
}
