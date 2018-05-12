using Todos.ViewModels;
using System;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace Todos
{
    public sealed partial class NewPage : Page
    {
        public NewPage()
        {
            this.InitializeComponent();

            ViewModel = new EditViewModels();
            DataContext = ViewModel;

            ViewModel.LoadData();
        }

        EditViewModels ViewModel { get; set; }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Visible;

            ((App)App.Current).BackRequested += NewPage_BackRequested;
            if (e.NavigationMode == NavigationMode.New)
            {
                ApplicationData.Current.LocalSettings.Values.Remove("TheWork");
            }
            else
            {
                if (ApplicationData.Current.LocalSettings.Values.ContainsKey("TheWork"))
                {
                    var composite = ApplicationData.Current.LocalSettings.Values["TheWork"] as ApplicationDataCompositeValue;
                    textBox1.Text = (string)composite["Title"];
                    textBox2.Text = (string)composite["Detail"];
                    Date_Picker.Date = DateTimeOffset.Parse((string)composite["Date"]);
                    ApplicationData.Current.LocalSettings.Values.Remove("TheWork");
                }
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            ((App)App.Current).BackRequested -= NewPage_BackRequested;

            bool suspending = ((App)App.Current).IsSuspending;
            if (suspending)
            {
                var composite = new ApplicationDataCompositeValue();
                composite["Title"] = textBox1.Text;
                composite["Detail"] = textBox2.Text;
                composite["Date"] = Date_Picker.Date.ToString();
                ApplicationData.Current.LocalSettings.Values["TheWork"] = composite;
            }
        }

        private void NewPage_BackRequested(object sender, BackRequestedEventArgs e)
        {
            ViewModel.SaveData();
        }

        private void Create_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(MainPage));
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(MainPage));
        }
    }
}
