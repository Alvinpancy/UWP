using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Microsoft.Toolkit.Uwp.Notifications;
using Windows.UI.Notifications;
using Windows.UI.Core;

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

    private ViewModels.TodoItemViewModel ViewModel;

    protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            this.ViewModel = (ViewModels.TodoItemViewModel)(e.Parameter);
            SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Collapsed;
            if (ViewModel.SelectedItem == null)
            {
                createButton.Content = "Create";
               // var i = new MessageDialog("欢迎来到Todos的创建界面！").ShowAsync();
            }
            else
            {
                //var i = new MessageDialog("请修改你的Todos！").ShowAsync();
                createButton.Content = "Update";
                title.Text = ViewModel.SelectedItem.title;
                description.Text = ViewModel.SelectedItem.description;
                DatePicker.Date = ViewModel.SelectedItem.time;
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            Frame.Navigate(typeof(NewPage), ViewModel);
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

        private void CreateButton_Clicked(object sender, RoutedEventArgs e)
        {
            int num = 1;
            if (DatePicker.Date.AddDays(1) < DateTimeOffset.Now) num = 0;
            if (title.Text.Trim() == String.Empty && description.Text.Trim() == String.Empty && num == 1)
                displayNoWords(1);
            if (title.Text.Trim() == String.Empty && description.Text.Trim() != String.Empty && num == 1)
                displayNoWords(2);
            if (title.Text.Trim() != String.Empty && description.Text.Trim() == String.Empty && num == 1)
                displayNoWords(3);
            if (title.Text.Trim() == String.Empty && description.Text.Trim() == String.Empty && num == 0)
                displayNoWords(4);
            if (title.Text.Trim() == String.Empty && description.Text.Trim() != String.Empty && num == 0)
                displayNoWords(5);
            if (title.Text.Trim() != String.Empty && description.Text.Trim() == String.Empty && num == 0)
                displayNoWords(6);
            if (title.Text.Trim() != String.Empty && description.Text.Trim() != String.Empty && num == 0)
                displayNoWords(7);
            if (ViewModel.SelectedItem == null)
            {
                if (title.Text.Trim() != String.Empty && description.Text.Trim() != String.Empty && num == 1)
                {
                    string str = DatePicker.Date.ToString();
                    ViewModel.AddTodoItem(title.Text, description.Text, str);
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
            }
            else
            {
                if (title.Text.Trim() != String.Empty && description.Text.Trim() != String.Empty && num == 1)
                    UpdateButton_Clicked(sender,e);
            }
            string from = title.Text;
            string subject = description.Text;
            // Construct the tile content
            TileContent content = new TileContent()
            {
                Visual = new TileVisual()
                {

                    TileSmall = new TileBinding()
                    {
                        Content = new TileBindingContentAdaptive()
                        {
                            Children =
                            {
                                new AdaptiveText()
                                {
                                    Text = from
                                },
                                new AdaptiveText()
                                {
                                    Text = subject,
                                    HintStyle = AdaptiveTextStyle.CaptionSubtle
                                }
                            },
                            BackgroundImage = new TileBackgroundImage()
                            {
                                Source = "Assets/taylor2.jpg",
                                HintOverlay = 20
                            }
                        }
                    },
                    TileMedium = new TileBinding()
                    {
                        Content = new TileBindingContentAdaptive()
                        {
                            Children =
                            {
                                new AdaptiveText()
                                {
                                    Text = from
                                },
                                new AdaptiveText()
                                {
                                    Text = subject,
                                    HintStyle = AdaptiveTextStyle.CaptionSubtle
                                }
                            },
                            BackgroundImage = new TileBackgroundImage()
                            {
                                Source = "Assets/taylor.jpg",
                                HintOverlay = 20
                            }
                        }
                    },
                    TileWide = new TileBinding()
                    {
                        Content = new TileBindingContentAdaptive()
                        {
                            Children =
                            {
                                new AdaptiveText()
                                {
                                    Text = from,
                                    HintStyle = AdaptiveTextStyle.Subtitle
                                },
                                new AdaptiveText()
                                {
                                    Text = subject,
                                    HintStyle = AdaptiveTextStyle.CaptionSubtle
                                }
                            },
                            BackgroundImage = new TileBackgroundImage()
                            {
                                Source = "Assets/taylor.jpg",
                                HintOverlay = 20
                            }
                        }
                    }
                }
            };
            var notification = new TileNotification(content.GetXml());
            TileUpdateManager.CreateTileUpdaterForApplication().Update(notification);
            TileUpdateManager.CreateTileUpdaterForApplication().EnableNotificationQueue(true);
        }

        private void CancelButton_Clicked(object sender, RoutedEventArgs e)
        {
                title.Text = String.Empty;
                description.Text = String.Empty;
                DatePicker.Date = DateTimeOffset.Now;
        }

        private  void DeleteButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)  //  删除一个项目
            {
                ViewModel.RemoveTodoItem(ViewModel.SelectedItem);
                Frame.Navigate(typeof(MainPage), ViewModel);
            }
        }

        private void BackButton_Clicked(object sender, RoutedEventArgs e)
        {
                Frame.Navigate(typeof(MainPage), ViewModel);
        }

        private void UpdateButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)
            {
                ViewModel.UpdateTodoItem(DatePicker.Date.ToString(), title.Text, description.Text);
                Frame.Navigate(typeof(MainPage), ViewModel);
            }
        }
    }
}
