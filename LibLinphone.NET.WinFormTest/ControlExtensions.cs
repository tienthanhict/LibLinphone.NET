using System;
using System.Windows.Forms;

namespace LibLinphone.NET.WinFormTest
{
    /// <summary>
    /// Source: http://www.codeproject.com/Articles/37642/Avoiding-InvokeRequired
    /// </summary>
    public static class ControlExtensions
    {
        static public void UIThreadBeginInvoke(this Control control, Action code)
        {
            if (control.InvokeRequired)
            {
                control.BeginInvoke(code);
                return;
            }
            code.Invoke();
        }

        static public void UIThreadInvoke(this Control control, Action code)
        {
            if (control.InvokeRequired)
            {
                control.Invoke(code);
                return;
            }
            code.Invoke();
        }
    }
}
