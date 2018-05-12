using System;
using System.Collections.ObjectModel;
using Windows.UI.Popups;
using SQLitePCL;

namespace Todos.ViewModels
{
    class TodoItemViewModel
    {
        private ObservableCollection<Models.TodoItem> allItems = new ObservableCollection<Models.TodoItem>();
        public ObservableCollection<Models.TodoItem> AllItems { get { return this.allItems; } }

        private Models.TodoItem selectedItem = default(Models.TodoItem);
        public Models.TodoItem SelectedItem { get { return selectedItem; } set { this.selectedItem = value; } }

        private SQLiteConnection db = App.conn;

        public TodoItemViewModel()
        {
            using (var statement = db.Prepare("SELECT * FROM Todos"))
            {
                while (SQLiteResult.DONE != statement.Step())
               {
                    string id = (string)statement[0];
                    string title = (string)statement[1];
                    string detail = (string)statement[2];
                    string time = (string)statement[3];
                    this.allItems.Add(new Models.TodoItem(id,title, detail, time));
                }
            }
        }

        public void AddTodoItem(string title, string description, string str)
        {
            var it = new Models.TodoItem(title, description, str);
            this.allItems.Add(it);
            try
            {
                using (var custstmt = db.Prepare("INSERT INTO Todos (Id, title, detail, time) VALUES (?, ?, ?, ?)"))
                {
                    custstmt.Bind(1, it.GetId());
                    custstmt.Bind(2, title);
                    custstmt.Bind(3, description);
                    custstmt.Bind(4, str);
                    custstmt.Step();
                }
            }
            catch (Exception ex)
            {
                var i = new MessageDialog(ex.Message).ShowAsync();
            }
        }

        public void RemoveTodoItem(Models.TodoItem Item)
        {
            this.allItems.Remove(Item);  //  Delete一个Item
            this.selectedItem = null;
        }

        public void DeleteDatabase(string customerId)
        {
            using (var statement = db.Prepare("DELETE FROM Todos WHERE Id = ?;"))
            {
                statement.Bind(1, customerId);
                statement.Step();
            }
        }

        public void UpdateTodoItem(string time, string titl, string descriptio)
        {
            this.selectedItem.title = titl;  // Update一个Item
            this.selectedItem.description = descriptio;
            this.SelectedItem.SetTime(time);
            this.selectedItem = null;
        }

        public void UpdataDatabase(string titl, string descriptio, string time, string id, Boolean com)
        {
            var db = App.conn;
            using (var custstmt = db.Prepare("UPDATE Todos SET title = ?, detail = ?, time = ? WHERE Id=?;"))
            {
                custstmt.Bind(1, titl);
                custstmt.Bind(2, descriptio);
                custstmt.Bind(3, time);
                custstmt.Bind(4, id);
                custstmt.Step();
            }
        }
    }
}