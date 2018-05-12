using System;
using System.Globalization;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Todos.ViewModels;

namespace Todos.Models
{
    class TodoItem : ViewModelBase
    {
        private string id;
        public string GetId() { return id; }

        private string titl;
        public string title { get { return titl; } set { Set(ref titl, value); } }

        private string detail;
        public string description { get { return detail; } set { Set(ref detail, value); } }

        private bool completed1;
        public bool completed { get { return completed1; } set { Set(ref completed1, value); } }


        public int completedLine
        {
            get { if (completed1 == true) return 1; else return 0; }
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
            this.completed1 = false; //默认为未完成
        }
    }
}
