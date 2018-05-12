using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;  
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace Animal
{
    public sealed partial class MainPage : Page
    {
        private delegate void Speaking(object sender);//声明委托, 可以同时委托saying和voicing 
        private event Speaking Say;//委托声明一个事件

        public MainPage()
        {
            this.InitializeComponent();
        }

        interface Animal
        {
            void saying(object sender);  //  动物说话 
            void voicing(object sender);  //  动物发出声音 
        }

        //  pig类实现接口 
        class pig : Animal
        {
            TextBlock word;

            public pig(TextBlock words)
            {
                this.word = words;
            }

            public void saying(object sender)  //  动物说话 
            {
                this.word.Text += "Pig: I am a pig.\n";
            }

            public async void voicing(object sender)  //  动物发出声音 
            {
                MediaElement mediaElement = new MediaElement();
                var synth = new Windows.Media.SpeechSynthesis.SpeechSynthesizer();
                Windows.Media.SpeechSynthesis.SpeechSynthesisStream stream = await synth.SynthesizeTextToStreamAsync("I am a pig");
                mediaElement.SetSource(stream, stream.ContentType);
                mediaElement.Play();
            }
        }

        //  cat类实现接口 
        class cat : Animal
        {
            TextBlock word;

            public cat(TextBlock words)
            {
                this.word = words;
            }

            public void saying(object sender)
            {
                this.word.Text += "Cat: I am a cat.\n";
            }

            public async void voicing(object sender)
            {
                MediaElement mediaElement = new MediaElement();
                var synth = new Windows.Media.SpeechSynthesis.SpeechSynthesizer();
                Windows.Media.SpeechSynthesis.SpeechSynthesisStream stream = await synth.SynthesizeTextToStreamAsync("I am a cat");
                mediaElement.SetSource(stream, stream.ContentType);
                mediaElement.Play();
            }
        }

        //  dog类实现接口 
        class dog : Animal
        {
            TextBlock word;

            public dog(TextBlock words)
            {
                this.word = words;
            }

            public void saying(object sender)
            {
                this.word.Text += "Dog: I am a dog.\n";
            }

            public async void voicing(object sender)
            {
                MediaElement mediaElement = new MediaElement();
                var synth = new Windows.Media.SpeechSynthesis.SpeechSynthesizer();
                Windows.Media.SpeechSynthesis.SpeechSynthesisStream stream = await synth.SynthesizeTextToStreamAsync("I am a dog");
                mediaElement.SetSource(stream, stream.ContentType);
                mediaElement.Play();
            }
        }

        private cat thecat;
        private dog thedog;
        private pig thepig;

        private void Button(object sender, RoutedEventArgs e)  //  Speak 按钮 
        {
            Random Ran = new Random();
            int Random_num = Ran.Next(0, 3);  //  随机 
            switch (Random_num)
            {
                case 0:
                    {
                        thecat = new cat(words);
                        Say += new Speaking(thecat.saying);
                        Say += new Speaking(thecat.voicing);
                        Say(this);
                        Say -= new Speaking(thecat.saying);
                        Say -= new Speaking(thecat.voicing);
                        break;
                    }

                case 1:
                    {
                        thedog = new dog(words);
                        Say += new Speaking(thedog.saying);
                        Say += new Speaking(thedog.voicing);
                        Say(this);
                        Say -= new Speaking(thedog.saying);
                        Say -= new Speaking(thedog.voicing);
                        break;
                    }

                default:
                    {
                        thepig = new pig(words);
                        Say += new Speaking(thepig.saying);
                        Say += new Speaking(thepig.voicing);
                        Say(this);
                        Say -= new Speaking(thepig.saying);
                        Say -= new Speaking(thepig.voicing);
                        break;
                    }
            }
        }

        private void Button1(object sender, RoutedEventArgs e)  //  OK 按钮 
        {

            String textbox_words = textBox.Text.ToString();
            switch (textbox_words)
            {
                case "cat":
                    {
                        thecat = new cat(words);
                        Say += new Speaking(thecat.saying);
                        Say += new Speaking(thecat.voicing);
                        Say(this);
                        Say -= new Speaking(thecat.saying);
                        Say -= new Speaking(thecat.voicing);
                        textBox.Text = "";
                        break;
                    }

                case "dog":
                    {
                        thedog = new dog(words);
                        Say += new Speaking(thedog.saying);
                        Say += new Speaking(thedog.voicing);
                        Say(this);
                        Say -= new Speaking(thedog.saying);
                        Say -= new Speaking(thedog.voicing);
                        textBox.Text = "";
                        break;
                    }

                case "pig":
                    {
                        thepig = new pig(words);
                        Say += new Speaking(thepig.saying);
                        Say += new Speaking(thepig.voicing);
                        Say(this);
                        Say -= new Speaking(thepig.saying);
                        Say -= new Speaking(thepig.voicing);
                        textBox.Text = "";
                        break;
                    }

                default:
                    textBox.Text = "";
                    break;
            }
        }
    }
}