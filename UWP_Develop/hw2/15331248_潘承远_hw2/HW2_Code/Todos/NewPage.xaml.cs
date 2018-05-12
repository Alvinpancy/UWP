using System;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Popups;


namespace Todos
{
    public sealed partial class NewPage : Page
    {
        public NewPage()
        {
            this.InitializeComponent();
            var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
            viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
            viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;

            if (rootFrame.CanGoBack)
            {
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Visible;
            }
            else
            {
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Collapsed;
            }
            var i = new MessageDialog("欢迎来到Todos的创建界面！").ShowAsync();
        }

        private async void displayNoWords(int num)
        {
            if (num == 1)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "Title和Details的内容为空",
                    Content = "请先输入Title和Details的内容",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
            if (num == 2)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "Title的内容为空",
                    Content = "请先输入Title的内容",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
            if (num == 3)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "Details的内容为空",
                    Content = "请先输入Details的内容",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
            if (num == 4)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "Title和Details的内容为空,设定的日期不符合（需要大于等于今天）",
                    Content = "请先输入Title和Details的内容并且修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
            if (num == 5)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "Title的内容为空，设定的日期不符合（需要大于等于今天）",
                    Content = "请先输入Title的内容并且修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
            if (num == 6)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "Details的内容为空，设定的日期不符合（需要大于等于今天）",
                    Content = "请先输入Details的内容并且修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
            if (num == 7)
            {
                ContentDialog noWifiDialog = new ContentDialog()
                {
                    Title = "设定的日期不符合（需要大于等于今天）",
                    Content = "请修改日期",
                    PrimaryButtonText = "Ok"
                };
                ContentDialogResult result = await noWifiDialog.ShowAsync();
            }
        }

        private void Create_Click(object sender, RoutedEventArgs e)
        {
            int num = 1;
            if (DatePicker.Date.AddDays(1) < DateTimeOffset.Now) num = 0;
            if (Title.Text.Trim() == String.Empty && Details.Text.Trim() == String.Empty && num == 1)
                displayNoWords(1);
            if (Title.Text.Trim() == String.Empty && Details.Text.Trim() != String.Empty && num == 1)
                displayNoWords(2);
            if (Title.Text.Trim() != String.Empty && Details.Text.Trim() == String.Empty && num == 1)
                displayNoWords(3);
            if (Title.Text.Trim() == String.Empty && Details.Text.Trim() == String.Empty && num == 0)
                displayNoWords(4);
            if (Title.Text.Trim() == String.Empty && Details.Text.Trim() != String.Empty && num == 0)
                displayNoWords(5);
            if (Title.Text.Trim() != String.Empty && Details.Text.Trim() == String.Empty && num == 0)
                displayNoWords(6);
            if (Title.Text.Trim() != String.Empty && Details.Text.Trim() != String.Empty && num == 0)
                displayNoWords(7);
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            Title.Text = String.Empty;
            Details.Text = String.Empty;
            DatePicker.Date = DateTimeOffset.Now;
        }

        private async void AppBarButton(object sender, RoutedEventArgs e)
        {
            var picker = new Windows.Storage.Pickers.FileOpenPicker();
            picker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
            picker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.PicturesLibrary;
            picker.FileTypeFilter.Add(".jpg");
            picker.FileTypeFilter.Add(".jpeg");
            picker.FileTypeFilter.Add(".png");

            Windows.Storage.StorageFile file = await picker.PickSingleFileAsync();
            if (file != null)
            {
                using (Windows.Storage.Streams.IRandomAccessStream fileStream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read))
                {
                    Windows.UI.Xaml.Media.Imaging.BitmapImage bitmapImage = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
                    bitmapImage.SetSource(fileStream);
                    ImageChange.Source = bitmapImage;
                }
            }
        }
    }
}
