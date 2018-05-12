using Todos.Models;
using Newtonsoft.Json;
using System;
using Windows.Storage;

namespace Todos.ViewModels
{
    class EditViewModels : ViewModelBase
    {
        private string title;
        public string Title { get { return title; } set { Set(ref title, value); } }

        private string detail;
        public string Detail { get { return detail; } set { Set(ref detail, value); } }

        private bool completed1;
        public bool Completed1 { get { return completed1; } set { Set(ref completed1, value); } }

        private bool completed2;
        public bool Completed2 { get { return completed2; } set { Set(ref completed2, value); } }

        private string date;
        public string Date { get { return date; } set { Set(ref date, value); } }

        public void LoadData()
        {
            if (ApplicationData.Current.RoamingSettings.Values.ContainsKey("Data"))
            {
                MyDataItem data = JsonConvert.DeserializeObject<MyDataItem>(
                    (string)ApplicationData.Current.RoamingSettings.Values["Data"]);
                Title = data.Title;
                Detail = data.Detail;
                Completed1 = data.Completed1;
                Completed2 = data.Completed2;
                Date = data.Date;
            }
            else
            {
                Title = string.Empty;
                Detail = string.Empty;
                Completed1 = false;
                Completed2 = false;
                Date = DateTime.Now.ToString();
            }
        }

        public void SaveData()
        {
            MyDataItem data = new MyDataItem { Title = this.Title, Detail = this.Detail, Completed1 = this.Completed1, Completed2 = this.Completed2, Date = this.Date};
            ApplicationData.Current.RoamingSettings.Values["Data"] =
                JsonConvert.SerializeObject(data);
        }
    }
}
