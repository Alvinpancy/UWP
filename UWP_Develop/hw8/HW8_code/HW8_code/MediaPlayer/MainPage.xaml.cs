using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Composition;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Hosting;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace donghuademo
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void PlayButton_Clicked(object sender, RoutedEventArgs e)
        {
            mediaPlayer.Play();
            EllStoryboard.Begin();
            EllStoryboard2.Begin();
        }

        private void PauseButton_Click(object sender, RoutedEventArgs e)
        {
            mediaPlayer.Pause();
            EllStoryboard.Pause();
            EllStoryboard3.Begin();
        }

        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            mediaPlayer.Stop();
            EllStoryboard.Stop();
            EllStoryboard3.Begin();
        }

        async private void SetLocalMedia()
        {
            //  使用openPicker打开本地文件
            var openPicker = new Windows.Storage.Pickers.FileOpenPicker();
            openPicker.SuggestedStartLocation =
            Windows.Storage.Pickers.PickerLocationId.VideosLibrary;
            //  下面四行是添加文件的指定格式
            openPicker.FileTypeFilter.Add(".wmv");
            openPicker.FileTypeFilter.Add(".mp4");
            openPicker.FileTypeFilter.Add(".wma");
            openPicker.FileTypeFilter.Add(".mp3");
            //  异步打开文件
            var file = await openPicker.PickSingleFileAsync();
            var stream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
            // 判断选择的文件的类型，如果是视屏，则隐藏封面，完成Big Bonus的要求
            if (file.ContentType.IndexOf("video/") == 0)
            {
                ellipse.Visibility = Visibility.Collapsed;
                bar.Visibility = Visibility.Collapsed;
            }
            //  设置MediaElement的Source
            mediaPlayer.SetSource(stream, file.ContentType);
            //  播放多媒体
            mediaPlayer.Play();
        }
        //  点击选择文件的按钮
        private void SelectButton_Click(object sender, RoutedEventArgs e)
        {
            //  调用SetLocalMedia()
            SetLocalMedia();
        }

        private void ShowVolumnSlider(object sender, RoutedEventArgs e)
        {
            FrameworkElement element = sender as FrameworkElement;
            if (element != null)
            {
                FlyoutBase.ShowAttachedFlyout(element);
            }
        }

        private void FullScreenButton_Click(object sender, object e)
        {
            mediaPlayer.IsFullWindow = !mediaPlayer.IsFullWindow;
        }


        private void Element_MediaOpened(object sender, RoutedEventArgs args)
        {
            timelineSlider.Maximum = mediaPlayer.NaturalDuration.TimeSpan.TotalSeconds;
        }

        private void Element_MediaEnded(object sender, RoutedEventArgs args)
        {
            mediaPlayer.Stop();
        }
    }
}
