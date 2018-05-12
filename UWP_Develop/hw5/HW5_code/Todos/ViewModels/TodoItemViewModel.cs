using System.Collections.ObjectModel;

namespace Todos.ViewModels
{
    class TodoItemViewModel
    {
        private ObservableCollection<Models.TodoItem> allItems = new ObservableCollection<Models.TodoItem>();
        public ObservableCollection<Models.TodoItem> AllItems { get { return this.allItems; } }

        private Models.TodoItem selectedItem = default(Models.TodoItem);
        public Models.TodoItem SelectedItem { get { return selectedItem; } set { this.selectedItem = value; } }

        //public TodoItemViewModel()
       // {
       // }

        public void AddTodoItem(string title, string description, string str)
        {
            this.allItems.Add(new Models.TodoItem(title, description, str));
        }

        public void RemoveTodoItem(Models.TodoItem id)
        {
            this.allItems.Remove(id);  //  Delete一个Item
            this.selectedItem = null;
        }

        public void UpdateTodoItem(string time, string titl, string descriptio)
        {
            this.selectedItem.title = titl;  // Update一个Item
            this.selectedItem.description = descriptio;
            this.SelectedItem.SetTime(time);
            this.selectedItem = null;
        }

    }
}