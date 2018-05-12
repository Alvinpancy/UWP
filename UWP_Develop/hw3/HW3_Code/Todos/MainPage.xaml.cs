using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;


namespace Todos
{
    public sealed partial class MainPage : Page
    {

        public MainPage()
        {
            this.InitializeComponent();
            var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
            viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
            viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;
            this.ViewModel = new ViewModels.TodoItemViewModel();
        }

       ViewModels.TodoItemViewModel ViewModel { get; set; }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter.GetType() == typeof(ViewModels.TodoItemViewModel))
            {
                this.ViewModel = (ViewModels.TodoItemViewModel)(e.Parameter);
            }
        }

        private void TodoItem_ItemClicked(object sender, ItemClickEventArgs e)
        {
            ViewModel.SelectedItem = (Models.TodoItem)(e.ClickedItem);
            if (InlineToDoItemViewGrid.Visibility == Visibility.Collapsed)
                Frame.Navigate(typeof(NewPage), ViewModel);
            else
            {
                createButton.Content = "Update";
                title_MainPage.Text = ViewModel.SelectedItem.title;
                description_MainPage.Text = ViewModel.SelectedItem.description;
                DatePicker_MainPage.Date = ViewModel.SelectedItem.time;
            }
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            ViewModel.SelectedItem = null;
            if (InlineToDoItemViewGrid.Visibility == Visibility.Collapsed)
                Frame.Navigate(typeof(NewPage), ViewModel);
        }

        private void Edit_Click(object sender, RoutedEventArgs e)
        {
            var dc = (sender as FrameworkElement).DataContext;
            var item = ToDoListView.ContainerFromItem(dc) as ListViewItem;
            ViewModel.SelectedItem = (Models.TodoItem)(item.Content);
            Frame.Navigate(typeof(NewPage), ViewModel);
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            var dc = (sender as FrameworkElement).DataContext;
            var item = ToDoListView.ContainerFromItem(dc) as ListViewItem;
            ViewModel.SelectedItem = (Models.TodoItem)(item.Content);
            ViewModel.RemoveTodoItem(ViewModel.SelectedItem);
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

        private void Main_Creat(object sender, RoutedEventArgs e)
        {
            int num = 1;
            if (DatePicker_MainPage.Date.AddDays(1) < DateTimeOffset.Now) num = 0;
            if (title_MainPage.Text.Trim() == String.Empty && description_MainPage.Text.Trim() == String.Empty && num == 1)
                displayNoWords(1);
            if (title_MainPage.Text.Trim() == String.Empty && description_MainPage.Text.Trim() != String.Empty && num == 1)
                displayNoWords(2);
            if (title_MainPage.Text.Trim() != String.Empty && description_MainPage.Text.Trim() == String.Empty && num == 1)
                displayNoWords(3);
            if (title_MainPage.Text.Trim() == String.Empty && description_MainPage.Text.Trim() == String.Empty && num == 0)
                displayNoWords(4);
            if (title_MainPage.Text.Trim() == String.Empty && description_MainPage.Text.Trim() != String.Empty && num == 0)
                displayNoWords(5);
            if (title_MainPage.Text.Trim() != String.Empty && description_MainPage.Text.Trim() == String.Empty && num == 0)
                displayNoWords(6);
            if (title_MainPage.Text.Trim() != String.Empty && description_MainPage.Text.Trim() != String.Empty && num == 0)
                displayNoWords(7);
            if (createButton.Content.ToString() != "Update")
            {
                if (title_MainPage.Text.Trim() != String.Empty && description_MainPage.Text.Trim() != String.Empty && num == 1)
                {
                    string str = DatePicker_MainPage.Date.ToString();
                    ViewModel.AddTodoItem(title_MainPage.Text, description_MainPage.Text, str);
                }
            }
            else
            {
                if (title_MainPage.Text.Trim() != String.Empty && description_MainPage.Text.Trim() != String.Empty && num == 1)
                    UpdateButton_Clicked(sender, e);
            }
            Frame.Navigate(typeof(MainPage), ViewModel);
        }

        private void UpdateButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)
            {
                ViewModel.UpdateTodoItem(DatePicker_MainPage.Date.ToString(), title_MainPage.Text, description_MainPage.Text);
            }
        }

        private void Main_Cancel(object sender, RoutedEventArgs e)
        {
                title_MainPage.Text = String.Empty;
                description_MainPage.Text = String.Empty;
                DatePicker_MainPage.Date = DateTimeOffset.Now;
                createButton.Content = "Creat";
        }

        private void CheckBox_bit(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(MainPage), ViewModel);  // 刷新界面
        }
    }
}
