using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System.Net.Http;
using Windows.Data.Xml.Dom;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上有介绍

namespace Todos
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class LifePage : Page
    {
        public LifePage()
        {
            this.InitializeComponent();
        }

        private void PButton_Click(object sender, RoutedEventArgs e)
        {
            att.Text = "";
            ctype.Text = "";
            style_citynm.Text = "";
            home.Text = "";
            queryAsync(number.Text);
        }

        private void IButton_Click(object sender, RoutedEventArgs e)
        {
            att.Text = "";
            ctype.Text = "";
            style_citynm.Text = "";
            home.Text = "";
           queryidAsync(number.Text);
        }

        async void queryAsync(string tel)
        {
            string url = "http://api.k780.com:88/?app=phone.get&phone=" + tel + "&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=xml";
            HttpClient client = new HttpClient();
            string result = await client.GetStringAsync(url);
            XmlDocument document = new XmlDocument();
            document.LoadXml(result);
            XmlNodeList list = document.GetElementsByTagName("att");
            IXmlNode node = list.Item(0);
            att.Text = node.InnerText;

            list = document.GetElementsByTagName("ctype");
            node = list.Item(0);
            ctype.Text = node.InnerText;

            list = document.GetElementsByTagName("style_citynm");
            node = list.Item(0);
            style_citynm.Text = node.InnerText;
        }

       async void queryidAsync(string id)
       {
            string url = "http://api.k780.com:88/?app=idcard.get&idcard=" + id + "&appkey=24663&sign=9ca7a95ed5d4d4aa5b745d095f89a791&format=json";
            HttpClient client = new HttpClient();
            string result = await client.GetStringAsync(url);
            JObject jo = (JObject)JsonConvert.DeserializeObject(result);
            string born = jo["result"]["born"].ToString();
            string sex = jo["result"]["sex"].ToString();
            string attt = jo["result"]["att"].ToString();
            string homee = jo["result"]["style_citynm"].ToString();
            att.Text = born;
            ctype.Text = sex;
            style_citynm.Text = attt;
            home.Text = homee;
        }


    }
}
