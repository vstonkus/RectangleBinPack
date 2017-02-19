namespace zetBlock
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
        protected override void Dispose (bool disposing)
            {
            if (disposing && (components != null))
                {
                components.Dispose ();
                }
            base.Dispose (disposing);
            }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent ()
            {
            this.components = new System.ComponentModel.Container();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.PartId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PartHeight = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PartWidth = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.PartAmount = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.MaterialId = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.MaterialHeight = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.MaterialWidth = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.MaterialAmount = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.binGroupBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.ButtonCalculate = new System.Windows.Forms.Button();
            this.binBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.amountDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.idDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.heightDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.widthDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.idDataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.widthDataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.heightDataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.binGroupBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.binBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.PartId,
            this.PartHeight,
            this.PartWidth,
            this.PartAmount,
            this.idDataGridViewTextBoxColumn1,
            this.widthDataGridViewTextBoxColumn1,
            this.heightDataGridViewTextBoxColumn1});
            this.dataGridView1.DataSource = this.binBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(12, 58);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(240, 271);
            this.dataGridView1.TabIndex = 0;
            // 
            // PartId
            // 
            this.PartId.HeaderText = "Id";
            this.PartId.Name = "PartId";
            this.PartId.ReadOnly = true;
            // 
            // PartHeight
            // 
            this.PartHeight.HeaderText = "Aukštis";
            this.PartHeight.Name = "PartHeight";
            // 
            // PartWidth
            // 
            this.PartWidth.HeaderText = "Plotis";
            this.PartWidth.Name = "PartWidth";
            // 
            // PartAmount
            // 
            this.PartAmount.HeaderText = "Kiekis";
            this.PartAmount.Name = "PartAmount";
            // 
            // dataGridView2
            // 
            this.dataGridView2.AutoGenerateColumns = false;
            this.dataGridView2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView2.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.MaterialId,
            this.MaterialHeight,
            this.MaterialWidth,
            this.MaterialAmount,
            this.amountDataGridViewTextBoxColumn,
            this.idDataGridViewTextBoxColumn,
            this.heightDataGridViewTextBoxColumn,
            this.widthDataGridViewTextBoxColumn});
            this.dataGridView2.DataBindings.Add(new System.Windows.Forms.Binding("Tag", this.binGroupBindingSource, "Id", true));
            this.dataGridView2.DataSource = this.binGroupBindingSource;
            this.dataGridView2.Location = new System.Drawing.Point(258, 58);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.Size = new System.Drawing.Size(489, 150);
            this.dataGridView2.TabIndex = 1;
            // 
            // MaterialId
            // 
            this.MaterialId.DataPropertyName = "Id";
            this.MaterialId.HeaderText = "Id";
            this.MaterialId.Name = "MaterialId";
            // 
            // MaterialHeight
            // 
            this.MaterialHeight.DataPropertyName = "Height";
            this.MaterialHeight.HeaderText = "Aukštis";
            this.MaterialHeight.Name = "MaterialHeight";
            // 
            // MaterialWidth
            // 
            this.MaterialWidth.DataPropertyName = "Width";
            this.MaterialWidth.HeaderText = "Plotis";
            this.MaterialWidth.Name = "MaterialWidth";
            // 
            // MaterialAmount
            // 
            this.MaterialAmount.DataPropertyName = "Amount";
            this.MaterialAmount.HeaderText = "Kiekis";
            this.MaterialAmount.Name = "MaterialAmount";
            this.MaterialAmount.ReadOnly = true;
            // 
            // binGroupBindingSource
            // 
            this.binGroupBindingSource.DataSource = typeof(zetBlock.BinGroup);
            // 
            // ButtonCalculate
            // 
            this.ButtonCalculate.Location = new System.Drawing.Point(299, 240);
            this.ButtonCalculate.Name = "ButtonCalculate";
            this.ButtonCalculate.Size = new System.Drawing.Size(75, 23);
            this.ButtonCalculate.TabIndex = 2;
            this.ButtonCalculate.Text = "Skaičiuoti";
            this.ButtonCalculate.UseVisualStyleBackColor = true;
            this.ButtonCalculate.Click += new System.EventHandler(this.ButtonCalculate_Click);
            // 
            // binBindingSource
            // 
            this.binBindingSource.DataSource = typeof(zetBlock.Bin);
            // 
            // amountDataGridViewTextBoxColumn
            // 
            this.amountDataGridViewTextBoxColumn.DataPropertyName = "Amount";
            this.amountDataGridViewTextBoxColumn.HeaderText = "Amount";
            this.amountDataGridViewTextBoxColumn.Name = "amountDataGridViewTextBoxColumn";
            this.amountDataGridViewTextBoxColumn.ReadOnly = true;
            // 
            // idDataGridViewTextBoxColumn
            // 
            this.idDataGridViewTextBoxColumn.DataPropertyName = "Id";
            this.idDataGridViewTextBoxColumn.HeaderText = "Id";
            this.idDataGridViewTextBoxColumn.Name = "idDataGridViewTextBoxColumn";
            // 
            // heightDataGridViewTextBoxColumn
            // 
            this.heightDataGridViewTextBoxColumn.DataPropertyName = "Height";
            this.heightDataGridViewTextBoxColumn.HeaderText = "Height";
            this.heightDataGridViewTextBoxColumn.Name = "heightDataGridViewTextBoxColumn";
            // 
            // widthDataGridViewTextBoxColumn
            // 
            this.widthDataGridViewTextBoxColumn.DataPropertyName = "Width";
            this.widthDataGridViewTextBoxColumn.HeaderText = "Width";
            this.widthDataGridViewTextBoxColumn.Name = "widthDataGridViewTextBoxColumn";
            // 
            // idDataGridViewTextBoxColumn1
            // 
            this.idDataGridViewTextBoxColumn1.DataPropertyName = "Id";
            this.idDataGridViewTextBoxColumn1.HeaderText = "Id";
            this.idDataGridViewTextBoxColumn1.Name = "idDataGridViewTextBoxColumn1";
            // 
            // widthDataGridViewTextBoxColumn1
            // 
            this.widthDataGridViewTextBoxColumn1.DataPropertyName = "Width";
            this.widthDataGridViewTextBoxColumn1.HeaderText = "Width";
            this.widthDataGridViewTextBoxColumn1.Name = "widthDataGridViewTextBoxColumn1";
            // 
            // heightDataGridViewTextBoxColumn1
            // 
            this.heightDataGridViewTextBoxColumn1.DataPropertyName = "Height";
            this.heightDataGridViewTextBoxColumn1.HeaderText = "Height";
            this.heightDataGridViewTextBoxColumn1.Name = "heightDataGridViewTextBoxColumn1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(942, 433);
            this.Controls.Add(this.ButtonCalculate);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.dataGridView1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.binGroupBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.binBindingSource)).EndInit();
            this.ResumeLayout(false);

            }

        #endregion
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn PartId;
        private System.Windows.Forms.DataGridViewTextBoxColumn PartHeight;
        private System.Windows.Forms.DataGridViewTextBoxColumn PartWidth;
        private System.Windows.Forms.DataGridViewTextBoxColumn PartAmount;
        private System.Windows.Forms.DataGridView dataGridView2;
        private System.Windows.Forms.Button ButtonCalculate;
        private System.Windows.Forms.BindingSource binBindingSource;
        private System.Windows.Forms.BindingSource binGroupBindingSource;
        private System.Windows.Forms.DataGridViewTextBoxColumn MaterialId;
        private System.Windows.Forms.DataGridViewTextBoxColumn MaterialHeight;
        private System.Windows.Forms.DataGridViewTextBoxColumn MaterialWidth;
        private System.Windows.Forms.DataGridViewTextBoxColumn MaterialAmount;
        private System.Windows.Forms.DataGridViewTextBoxColumn idDataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn widthDataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn heightDataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn amountDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn idDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn heightDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn widthDataGridViewTextBoxColumn;
        }
    }

