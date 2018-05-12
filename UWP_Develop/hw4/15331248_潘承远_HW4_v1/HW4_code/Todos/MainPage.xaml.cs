using Todos.ViewModels;
using Windows.Storage;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

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
            ViewModel = new EditViewModels();
            DataContext = ViewModel;
            ViewModel.LoadData();
        }

        EditViewModels ViewModel { get; set; }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(NewPage));
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {

            if (e.NavigationMode == NavigationMode.New)
            {
                ApplicationData.Current.LocalSettings.Values.Remove("Setting");
            }
            else
            {
                    if (ApplicationData.Current.LocalSettings.Values.ContainsKey("Setting"))
                    {
                        var comp = ApplicationData.Current.LocalSettings.Values["Setting"] as ApplicationDataCompositeValue;
                        CheckBox1.IsChecked = (bool?)comp["Completed1"];
                        CheckBox2.IsChecked = (bool?)comp["Completed2"];
                        ApplicationData.Current.LocalSettings.Values.Remove("Setting");
                    }

            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            ViewModel.SaveData();

            bool suspending = ((App)App.Current).IsSuspending;
            if (suspending)
            {
                var composite = new ApplicationDataCompositeValue();
                composite["Completed1"] = CheckBox1.IsChecked;
                composite["Completed2"] = CheckBox2.IsChecked;
                ApplicationDataContainer localSetting = ApplicationData.Current.LocalSettings;
                localSetting.Values["Setting"] = composite;
            }
        }
    }
}
