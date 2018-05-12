using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;

namespace Todos
{
    public class BooleanToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            return (bool)value ? 1 : 0;
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return ((int)value == 1);
        }
    }
}
