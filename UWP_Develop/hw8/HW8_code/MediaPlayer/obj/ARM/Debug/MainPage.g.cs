﻿#pragma checksum "C:\Users\admini\Desktop\important\大二下\（作）现代操作与系统开发\hw8\HW8_code\MediaPlayer\MainPage.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "CB79B1CE2ACED72928ED5F991214AD30"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace donghuademo
{
    partial class MainPage : 
        global::Windows.UI.Xaml.Controls.Page, 
        global::Windows.UI.Xaml.Markup.IComponentConnector,
        global::Windows.UI.Xaml.Markup.IComponentConnector2
    {
        /// <summary>
        /// Connect()
        /// </summary>
        [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Windows.UI.Xaml.Build.Tasks"," 14.0.0.0")]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void Connect(int connectionId, object target)
        {
            switch(connectionId)
            {
            case 1:
                {
                    this.EllStoryboard = (global::Windows.UI.Xaml.Media.Animation.Storyboard)(target);
                }
                break;
            case 2:
                {
                    this.EllStoryboard2 = (global::Windows.UI.Xaml.Media.Animation.Storyboard)(target);
                }
                break;
            case 3:
                {
                    this.EllStoryboard3 = (global::Windows.UI.Xaml.Media.Animation.Storyboard)(target);
                }
                break;
            case 4:
                {
                    this.Play = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 31 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)this.Play).Click += this.PlayButton_Clicked;
                    #line default
                }
                break;
            case 5:
                {
                    this.Pause = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 32 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)this.Pause).Click += this.PauseButton_Click;
                    #line default
                }
                break;
            case 6:
                {
                    this.Stop = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 33 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)this.Stop).Click += this.StopButton_Click;
                    #line default
                }
                break;
            case 7:
                {
                    this.Select = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 34 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)this.Select).Click += this.SelectButton_Click;
                    #line default
                }
                break;
            case 8:
                {
                    this.Volumn = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 35 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)this.Volumn).Click += this.ShowVolumnSlider;
                    #line default
                }
                break;
            case 9:
                {
                    this.FullScreen = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 42 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)this.FullScreen).Click += this.FullScreenButton_Click;
                    #line default
                }
                break;
            case 10:
                {
                    this.VolumnSlider = (global::Windows.UI.Xaml.Controls.Slider)(target);
                }
                break;
            case 11:
                {
                    this.mediaPlayer = (global::Windows.UI.Xaml.Controls.MediaElement)(target);
                    #line 55 "..\..\..\MainPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.MediaElement)this.mediaPlayer).MediaOpened += this.Element_MediaOpened;
                    #line default
                }
                break;
            case 12:
                {
                    this.timelineSlider = (global::Windows.UI.Xaml.Controls.Slider)(target);
                }
                break;
            case 13:
                {
                    this.ellipse = (global::Windows.UI.Xaml.Shapes.Ellipse)(target);
                }
                break;
            case 14:
                {
                    this.bar = (global::Windows.UI.Xaml.Shapes.Rectangle)(target);
                }
                break;
            default:
                break;
            }
            this._contentLoaded = true;
        }

        [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Windows.UI.Xaml.Build.Tasks"," 14.0.0.0")]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public global::Windows.UI.Xaml.Markup.IComponentConnector GetBindingConnector(int connectionId, object target)
        {
            global::Windows.UI.Xaml.Markup.IComponentConnector returnValue = null;
            return returnValue;
        }
    }
}

