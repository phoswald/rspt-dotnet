namespace Calculator
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
            this.label1 = new System.Windows.Forms.Label();
            this._input = new System.Windows.Forms.TextBox();
            this._eval = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this._output = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this._message = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Expression:";
            // 
            // _input
            // 
            this._input.Location = new System.Drawing.Point(16, 30);
            this._input.Multiline = true;
            this._input.Name = "_input";
            this._input.Size = new System.Drawing.Size(699, 189);
            this._input.TabIndex = 1;
            // 
            // _eval
            // 
            this._eval.Location = new System.Drawing.Point(16, 226);
            this._eval.Name = "_eval";
            this._eval.Size = new System.Drawing.Size(75, 23);
            this._eval.TabIndex = 2;
            this._eval.Text = "Evaluate";
            this._eval.UseVisualStyleBackColor = true;
            this._eval.Click += new System.EventHandler(this.eval_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(97, 231);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Output:";
            // 
            // _output
            // 
            this._output.Location = new System.Drawing.Point(144, 228);
            this._output.Name = "_output";
            this._output.ReadOnly = true;
            this._output.Size = new System.Drawing.Size(571, 20);
            this._output.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 256);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Message:";
            // 
            // _message
            // 
            this._message.Location = new System.Drawing.Point(16, 273);
            this._message.Multiline = true;
            this._message.Name = "_message";
            this._message.ReadOnly = true;
            this._message.Size = new System.Drawing.Size(699, 150);
            this._message.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(727, 435);
            this.Controls.Add(this._message);
            this.Controls.Add(this.label3);
            this.Controls.Add(this._output);
            this.Controls.Add(this.label2);
            this.Controls.Add(this._eval);
            this.Controls.Add(this._input);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Calculator";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox _input;
        private System.Windows.Forms.Button _eval;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox _output;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox _message;
    }
}

