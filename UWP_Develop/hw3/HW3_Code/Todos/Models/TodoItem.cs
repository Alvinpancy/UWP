using System;
using System.Globalization;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace Todos.Models
{
    class TodoItem : INotifyPropertyChanged
    {
        private string id;
        public bool com = default(bool);
        public string GetId() { return id; }
        public string title { get; set; }
        public string description { get; set; }
        public bool completed
        {
            get { return com; }
            set { com = value; }
        }

        public int completedLine
        {
            get { if (com == true) return 1; else return 0; }
        }

        public DateTime time { get; set; }

        public void SetTime(string tim)
        {
            DateTimeFormatInfo dtFormat = new DateTimeFormatInfo();
            dtFormat.ShortDatePattern = "yyyy/MM/dd";
            DateTime dt = Convert.ToDateTime(tim, dtFormat);
            this.time = dt;
        }

        public TodoItem(string title, string description, string tim)
        {
            this.id = Guid.NewGuid().ToString(); //生成id
            this.title = title;
            this.description = description;
            SetTime(tim);
            this.com = false; //默认为未完成
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged([CallerMemberName]string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
