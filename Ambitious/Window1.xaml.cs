using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Ambitious
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        private bool PIDUsuario = false;
        private UInt32[] Product = new UInt32[91];
        private string SaveS1 = string.Empty;
        private string SaveS2 = string.Empty;
        private string SaveModule = string.Empty;
        private string SavePointAx = string.Empty;
        private string SavePointAy = string.Empty;
        private string SavePointBx = string.Empty;
        private string SavePointBy = string.Empty;
        private string SavePointGx = string.Empty;
        private string SavePointGy = string.Empty;
        private string SavePublicKey = string.Empty;

        public Window1()
        {
            InitializeComponent();
            LoadProductsCombo();

            //    if(!VerificarProteccion(Proteccion))
            //        Application->Terminate();
            cmbProducts.SelectedIndex = 3;
            cmbProducts_SelectionChanged(this, null);

            SaveModule = SavePointAx = SavePointAy = SavePointBx = SavePointBy = SavePointGx = SavePointGy =
                "0x00000000,0x00000000,0x00000000,0x00000000," +
                "0x00000000,0x00000000,0x00000000,0x00000000," +
                "0x00000000,0x00000000,0x00000000,0x00000000;";
            SaveS1 = SaveS2 = "0";
        }

        private void LoadProductsCombo()
        {
            cmbProducts.Items.Add("Custom PID");
            cmbProducts.Items.Add("Visual Studio .NET Enterprise Architect");
            cmbProducts.Items.Add("Visual Studio .NET Enterprise Architect OEM");
            cmbProducts.Items.Add("Windows XP Professional");
            cmbProducts.Items.Add("Windows XP Professional OEM");
            cmbProducts.Items.Add("Windows 2000 Server / Adv Serv / Multiling");
            cmbProducts.Items.Add("Windows 2000 Server / Adv Serv / Multiling OEM");
            cmbProducts.Items.Add("Windows 2000 Server Evaluation");
            cmbProducts.Items.Add("Windows 2000 Server Evaluation OEM");
            cmbProducts.Items.Add("Windows 2000 Professional");
            cmbProducts.Items.Add("Windows 2000 Professional OEM (= W2KSAMOEM)");
            cmbProducts.Items.Add("Windows 2000 Terminal Services  (wrong)");
            cmbProducts.Items.Add("Windows 2000 Terminal Services OEM  (wrong)");
            cmbProducts.Items.Add("Windows Millenium Edition");
            cmbProducts.Items.Add("Windows Millenium Edition OEM");
            cmbProducts.Items.Add("Windows 98 First / Second Edition");
            cmbProducts.Items.Add("Windows 98 First / Second Edition OEM");
            cmbProducts.Items.Add("Office XP Professional");
            cmbProducts.Items.Add("Office XP Professional OEM");
            cmbProducts.Items.Add("Office 2000 Full and Upgrade");
            cmbProducts.Items.Add("Office 2000 Full and Upgrade OEM");
            cmbProducts.Items.Add("Internet Explorer 5 Administration Kit - 1");
            cmbProducts.Items.Add("Internet Explorer 5 Administration Kit - 2");
            cmbProducts.Items.Add("Internet Explorer 5 Administration Kit - 3  (= W2KSAM)");
            cmbProducts.Items.Add("Internet Explorer 5 Administration Kit - 4");
        }

        private void ValidateKey_Click(object sender, RoutedEventArgs e)
        {
            int Resultado;

            Cursor prevCursor = this.Cursor;
        
            if(txtKey.Text == string.Empty)
            {
                MessageBox.Show("Introduce a product key first", "Result", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                txtKey.Focus();
                return;
            }
        
            this.Cursor = Cursors.Wait;

            UInt32[] Mod, AxAy, BxBy;
            CargarMagicUsuario(out Mod, out AxAy, out BxBy);
        
            byte[] keyBytes = StrToByteArray(txtKey.Text);
            unsafe
            {
                uint UpperKeyAddr1, UpperKeyAddr2, UpperKeyAddr3, UpperKeyAddr4, LowerKeyAddr;

                fixed (byte* keyBytesRef = keyBytes)
                {
                    fixed (uint* AxAyRef = AxAy, BxByRef = BxBy, ModRef = Mod)
                    {
                        Resultado = AmbitiousLib.KeyServices.ValidateKey(
                            keyBytesRef, 
                            cmbProducts.SelectedIndex, 
                            ModRef, AxAyRef, BxByRef,
                            &UpperKeyAddr1, &UpperKeyAddr2, &UpperKeyAddr3, &UpperKeyAddr4, &LowerKeyAddr);
                    }
                }

                if (UpperKeyAddr1 == 0 && UpperKeyAddr2 == 0 && UpperKeyAddr3 == 0 && UpperKeyAddr4 == 0 && LowerKeyAddr == 0)
                {
                    txtRest.Text = "Not Valid";
                    txtMagnitude.Text = "Not Valid";
                }
                else
                {
                    txtRest.Text = "0x" + LowerKeyAddr.ToString("X8");
                    txtMagnitude.Text = "0x" + /*UpperKeyAddr4.ToString("X8") + */ UpperKeyAddr3.ToString("X8") + UpperKeyAddr2.ToString("X8") + UpperKeyAddr1.ToString("X8");
                }
            }
        
            // Restaura el cursor al original
            this.Cursor = prevCursor;

            switch(Resultado)
            {
                case 0://OK:
                    MessageBox.Show("Key is VALID", "Result", MessageBoxButton.OK, MessageBoxImage.Information);
                    break;
                case 1://MAL:
                    MessageBox.Show("Key is invalid", "Result", MessageBoxButton.OK, MessageBoxImage.Error);
                    break;
                case 3://ERROR_NUMMAGICOS:
                    MessageBox.Show("Public Key incorrect", "Result", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    break;
                default:
                    MessageBox.Show(Resultado.ToString(),"Unknown value", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    break;
            }
        }

        public static byte[] StrToByteArray(string str)
        {
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
            return encoding.GetBytes(str);
        }

        private void cmbProducts_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            // Si estaba en el PID de usuario, recuerda los valores de generación
            // para no perderlos al cambiar de producto
            if(PIDUsuario)
            {
                SaveModule = txtModule.Text;
                SavePointAx = txtPointAx.Text;
                SavePointAy = txtPointAy.Text;
                SavePointBx = txtPointBx.Text;
                SavePointBy = txtPointBy.Text;
                SavePointGx = txtPointGx.Text;
                SavePointGy = txtPointGy.Text;
                SaveS1 = txtS1.Text;
                SaveS2 = txtS2.Text;
                SavePublicKey = txtPublicKey.Text;
            }

            // Accede a los números mágicos del producto
            unsafe
            {
                uint* productAddr = AmbitiousLib.KeyServices.Index2Product(cmbProducts.SelectedIndex);
                Product = new UInt32[91];
                if (productAddr != null)
                    for (int i = 0; i < 91; i++)
                        Product[i] = productAddr[i];
                else
                    Product = null;
            }

            if (Product == null)
                PIDUsuario = true;
            else
                PIDUsuario = false;

            // Output the data to the window controls 
            txtIterations.Text = "Unknown";

            if (PIDUsuario)
            {
                txtModule.Text = SaveModule;
                txtPointAx.Text = SavePointAx;
                txtPointAy.Text = SavePointAy;
                txtPointBx.Text = SavePointBx;
                txtPointBy.Text = SavePointBy;
                txtPublicKey.Text = SavePublicKey;
                txtPointGx.Text = SavePointGx;
                txtPointGy.Text = SavePointGy;
                txtS1.Text = SaveS1;
                txtS2.Text = SaveS2;
            }
            else
            {
                WriteHexa(Product, 7, 12, txtModule, true, true);
                WriteHexa(Product, 7 + 3 * 12, 12, txtPointAx, true, true);
                WriteHexa(Product, 7 + 4 * 12, 12, txtPointAy, true, true);
                WriteHexa(Product, 7 + 5 * 12, 12, txtPointBx, true, true);
                WriteHexa(Product, 7 + 6 * 12, 12, txtPointBy, true, true);
                WriteHexa(Product, 0, 7 + 7 * 12, txtPublicKey, true, true);
                txtPointGx.Text = "Unknown";
                txtPointGy.Text = "Unknown";
                txtS1.Text = "Unknown";
                txtS2.Text = "Unknown";
            }

            EnableControls(PIDUsuario);
        }

        private void EnableControls(bool PIDUsuario)
        {
            btnGenerateKey.IsEnabled = PIDUsuario;

            txtModule.IsEnabled = PIDUsuario;
            txtPointAx.IsEnabled = PIDUsuario;
            txtPointAy.IsEnabled = PIDUsuario;
            txtPointBx.IsEnabled = PIDUsuario;
            txtPointBy.IsEnabled = PIDUsuario;
            txtPointGx.IsEnabled = PIDUsuario;
            txtPointGy.IsEnabled = PIDUsuario;
            txtS1.IsEnabled = PIDUsuario;
            txtS2.IsEnabled = PIDUsuario;
            txtRest.IsEnabled = PIDUsuario;
            txtPublicKey.IsEnabled = PIDUsuario;
        }

        private void CargarMagicUsuario(out UInt32[] Mod, out UInt32[] AxAy, out UInt32[] BxBy)
        {
            Mod = new UInt32[12];
            AxAy = new UInt32[12*2];
            BxBy = new UInt32[12*2];
        
            try
            {
                ReadHexa(Mod, 0, 12, txtModule);
                ReadHexa(AxAy, 0, 12, txtPointAx);
                ReadHexa(AxAy, 12, 12, txtPointAy);
                ReadHexa(BxBy, 0, 12, txtPointBx);
                ReadHexa(BxBy, 12, 12, txtPointBy);
            }
            catch(Exception)
            {
                MessageBox.Show("Error reading hexadecimal values (Mod, A, B)", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            };
        }

        private void ReadHexa(UInt32[] values, int startIn, int numToRead, TextBox control)
        {
            for(int i=startIn, j=0; i<startIn+numToRead; i++, j+=11)
            {
                values[i] = UInt32.Parse(control.Text.Substring(j+2, 8), System.Globalization.NumberStyles.HexNumber); 
            }
        }
        private void ReadHexa(out UInt32 value, TextBox control)
        {
            value = UInt32.Parse(control.Text.Substring(2, 8), System.Globalization.NumberStyles.HexNumber);
        }

        private void WriteHexa(UInt32[] values, int startIn, int numToWrite, TextBox control, bool eraseControlFirst, bool comma)
        {
            string Resultado = string.Empty;
            for (int i = startIn; i < startIn+numToWrite; i++)
            {
                if(comma)
                    Resultado += "0x"+ values[i].ToString("X8") + ((i<startIn+numToWrite-1)? "," : ";");
                else
                    Resultado += values[i].ToString("X8") + " ";
            }
            if(eraseControlFirst)
                control.Text = Resultado;
            else
                control.Text = control.Text + Resultado;
        }

        private void btnLoadPID_Click(object sender, RoutedEventArgs e)
        {
            cmbProducts.SelectedIndex = 0;  // Force Custom ID

            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog();
            open.Filter = "License files|*.lic";
            open.DefaultExt = "lic";
            open.FileName = "CustomPID";

            if (open.ShowDialog() == true)
            {
                using (StreamReader infile = new StreamReader(open.FileName))
                {
                    txtModule.Text = infile.ReadLine();
                    txtPointAx.Text = infile.ReadLine();
                    txtPointAy.Text = infile.ReadLine();
                    txtPointBx.Text = infile.ReadLine();
                    txtPointBy.Text = infile.ReadLine();
                    txtPointGx.Text = infile.ReadLine();
                    txtPointGy.Text = infile.ReadLine();
                    txtS1.Text = infile.ReadLine();
                    txtS2.Text = infile.ReadLine();
                    txtPublicKey.Text = infile.ReadLine();
                }

                EnableControls(true);
            }
        }

        private void btnSavePID_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog save = new Microsoft.Win32.SaveFileDialog();
            save.Filter = "License files|*.lic";
            save.DefaultExt = "lic";
            save.FileName = "CustomPID";

            if (save.ShowDialog() == true)
            {
                using (StreamWriter outfile = new StreamWriter(save.FileName))
                {
                    outfile.WriteLine(txtModule.Text);
                    outfile.WriteLine(txtPointAx.Text);
                    outfile.WriteLine(txtPointAy.Text);
                    outfile.WriteLine(txtPointBx.Text);
                    outfile.WriteLine(txtPointBy.Text);
                    outfile.WriteLine(txtPointGx.Text);
                    outfile.WriteLine(txtPointGy.Text);
                    outfile.WriteLine(txtS1.Text);
                    outfile.WriteLine(txtS2.Text);
                    outfile.WriteLine(txtPublicKey.Text);
                }
            }
        }

        private void btnSearch_Click(object sender, RoutedEventArgs e)
        {
            int Resultado;

            Cursor prevCursor = this.Cursor;

            if (txtKey.Text == string.Empty)
            {
                MessageBox.Show("Introduce a product key first", "Result", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                txtKey.Focus();
                return;
            }

            this.Cursor = Cursors.Wait;

            byte[] keyBytes = StrToByteArray(txtKey.Text);
            unsafe
            {
                uint UpperKeyAddr1, UpperKeyAddr2, UpperKeyAddr3, UpperKeyAddr4, LowerKeyAddr;

                fixed (byte* keyBytesRef = keyBytes)
                {
                    Resultado = AmbitiousLib.KeyServices.SearchProductFromKey(
                        keyBytesRef,
                        &UpperKeyAddr1, &UpperKeyAddr2, &UpperKeyAddr3, &UpperKeyAddr4, &LowerKeyAddr);
                }

                if (UpperKeyAddr1 == 0 && UpperKeyAddr2 == 0 && UpperKeyAddr3 == 0 && UpperKeyAddr4 == 0 && LowerKeyAddr == 0)
                {
                    txtRest.Text = "Not Valid";
                    txtMagnitude.Text = "Not Valid";
                }
                else
                {
                    txtRest.Text = "0x" + LowerKeyAddr.ToString("X8");
                    txtMagnitude.Text = "0x" + /*UpperKeyAddr4.ToString("X8") + */ UpperKeyAddr3.ToString("X8") + UpperKeyAddr2.ToString("X8") + UpperKeyAddr1.ToString("X8");
                }
            }

            this.Cursor = prevCursor;

            if (Resultado == 0)
                MessageBox.Show("Key is invalid", "Result", MessageBoxButton.OK, MessageBoxImage.Error);
            else
                MessageBox.Show("Key belongs to product: " + cmbProducts.Items[Resultado].ToString(), "Result", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void btnGeneratePID_Click(object sender, RoutedEventArgs e)
        {
            byte[] product = new byte[394];
            byte[] g1g2 = new byte[24*4];
            UInt32[] G1G2 = new UInt32[12 * 2];
            uint s1, s2;

            cmbProducts.SelectedIndex = 0;  // Force Custom ID

            if(txtS1.Text == "")  txtS1.Text = "0";
            if(txtS2.Text == "")  txtS2.Text = "0";
            s1 = uint.Parse(txtS1.Text);
            s2 = uint.Parse(txtS2.Text);
            
            unsafe
            {
                fixed(byte* productAddr = product, g1g2Addr = g1g2)
                {
                    if (AmbitiousLib.KeyServices.GeneratePID((uint*)productAddr, (uint*)g1g2Addr, &s1, &s2) == 3)
                        MessageBox.Show("INTERNAL ERROR: Generated PID is invalid");
                    else
                    {
                        Product = new UInt32[91];
                        for (int i = 0; i < 91; i++)
                            Product[i] = ((uint*)productAddr)[i];
                        for (int i = 0; i < 24; i++)
                            G1G2[i] = ((uint*)g1g2Addr)[i];
                    }
                }
            }

            WriteHexa(Product, 7, 12, txtModule, true, true);
            WriteHexa(Product, 7 + 3 * 12, 12, txtPointAx, true, true);
            WriteHexa(Product, 7 + 4 * 12, 12, txtPointAy, true, true);
            WriteHexa(Product, 7 + 5 * 12, 12, txtPointBx, true, true);
            WriteHexa(Product, 7 + 6 * 12, 12, txtPointBy, true, true);
            WriteHexa(G1G2, 0, 12, txtPointGx, true, true);
            WriteHexa(G1G2, 12, 12, txtPointGy, true, true);
            txtS1.Text = s1.ToString();
            txtS2.Text = s2.ToString();
            WriteHexa(Product, 0, 7+7*12, txtPublicKey, true, true);
        
            EnableControls(true);
        }

        private void btnGenerateKey_Click(object sender, RoutedEventArgs e)
        {
            int Resultado;

            UInt32[] Mod, AxAy, BxBy;
            UInt32 LowerKeyAddr;
            UInt32[] GxGy = new UInt32[12 * 2];
            UInt32 s1, s2, fLow, fHigh;

            CargarMagicUsuario(out Mod, out AxAy, out BxBy);

            // Recoge valores de generación
            try
            {
                ReadHexa(GxGy, 0, 12, txtPointGx);
                ReadHexa(GxGy, 12, 12, txtPointGy);
                s1 = uint.Parse(txtS1.Text);
                s2 = uint.Parse(txtS2.Text);
                if (txtRest.Text == "") txtRest.Text = "0x00000000";
                ReadHexa(out LowerKeyAddr, txtRest);
                // Sólo se admiten 31 bits de resto!
                LowerKeyAddr &= 0x7FFFFFFF;
            }
            catch(Exception)
            {
                MessageBox.Show("Error parsing generation values (G, s1, s2, f)");
                return;
            };

            Cursor prevCursor = this.Cursor;
            this.Cursor = Cursors.Wait;

            byte[] keyBytes = StrToByteArray(txtKey.Text);
            unsafe
            {
                uint UpperKeyAddr1, UpperKeyAddr2, UpperKeyAddr3, UpperKeyAddr4;

                fixed (uint* AxAyRef = AxAy, BxByRef = BxBy, ModRef = Mod, GxGyRef = GxGy)
                {
                    byte* keyRef;
                    Resultado = AmbitiousLib.KeyServices.GenerateKey(
                        &keyRef,
                        ModRef, AxAyRef, BxByRef,
                        &LowerKeyAddr,
                        GxGyRef, &s1, &s2, &fLow, &fHigh,
                        &UpperKeyAddr1, &UpperKeyAddr2, &UpperKeyAddr3, &UpperKeyAddr4);
                    if (Resultado == 0)
                        txtKey.Text = new string((sbyte*)keyRef);
                }

                if (UpperKeyAddr1 == 0 && UpperKeyAddr2 == 0 && UpperKeyAddr3 == 0 && UpperKeyAddr4 == 0 && LowerKeyAddr == 0)
                {
                    txtRest.Text = "Not Valid";
                    txtMagnitude.Text = "Not Valid";
                    txtIterations.Text = "Unknown";
                }
                else
                {
                    txtRest.Text = "0x" + LowerKeyAddr.ToString("X8");
                    txtMagnitude.Text = "0x" + /*UpperKeyAddr4.ToString("X8") + */ UpperKeyAddr3.ToString("X8") + UpperKeyAddr2.ToString("X8") + UpperKeyAddr1.ToString("X8");
                    txtIterations.Text = "0x" + fHigh.ToString("X8") + fLow.ToString("X8");
                }
            }

            this.Cursor = prevCursor;

            switch (Resultado)
            {
                case 0://OK:
                    MessageBox.Show("Key GENERATED", "Result", MessageBoxButton.OK, MessageBoxImage.Information);
                    break;
                case 3://ERROR_NUMMAGICOS:
                    MessageBox.Show("Public Key incorrect", "Result", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    break;
                case 4://ERROR_OTROF:
                    MessageBox.Show("Key not generated. Try another 'f'", "Result", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    break;
                default:
                    MessageBox.Show(Resultado.ToString(), "Unknown value", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    break;
            }

        }
    }
}
