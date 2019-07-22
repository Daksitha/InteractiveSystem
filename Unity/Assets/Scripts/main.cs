using LitJson;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

using UnityEngine;
using uPLibrary.Networking.M2Mqtt.Messages;
using ZenFulcrum.EmbeddedBrowser;

enum ScreenStatus { ON =1,OFF = 0};
enum ScrollStatus { OFF = 1, ON = 0  };

public class main : MonoBehaviour {
    private JsonData jsonmsg;
    private RequestHandler request;
    public GameObject ui;

    public Browser obj;
   // public GameObject displayObj;
    public int screenFlag;
    public int scrollFlag;
    public int previousScreenStat = (int)ScreenStatus.OFF;
    public int previousScrollStat = (int)ScrollStatus.OFF;



    // Use this for initialization
    void Start() {


        request = new RequestHandler();
        //displayObj = GameObject.Find("Canvas");
   
        obj = GameObject.FindGameObjectWithTag("welcomeScreen").GetComponent<Browser>();
        ui = GameObject.Find("Image");
        


        request.Gesture_client.MqttMsgPublishReceived += ArduinoMessageReceived;
        //WaitForSeconds waitTimer = new WaitForSeconds(0.1f);

        //BrowserOn(true);
       

    }

    // Update is called once per frame
    void Update() {

        if (screenFlag == (int)ScreenStatus.ON && previousScreenStat != screenFlag)
        {

            BrowserOn(true);
            previousScreenStat = screenFlag;
        }
        else if (screenFlag == (int)ScreenStatus.OFF && previousScreenStat != screenFlag)
        {
            BrowserOn(false);
           previousScreenStat = screenFlag;
        }

        if (scrollFlag == (int)ScrollStatus.ON )
        {
            //scrolling down the page
           
           BrowserScroll(obj);  

        }
        else if (scrollFlag == (int)ScrollStatus.ON)
        {
            obj.EvalJS("window.scrollTo(0, 0);");
        }





    }

  

    void BrowserScroll(Browser bObject)
    {

        obj.EvalJS("if(window.pageYOffset < document.body.scrollHeight - document.body.offsetHeight) window.scrollBy(0, 1);");
        obj.EvalJS("if (window.pageYOffset == document.body.scrollHeight - document.body.offsetHeight) window.scrollTo(0, 0);");

    }
    void BrowserOn(bool state)
    {
        if (state)
        {
            ui.SetActive(false);

            obj.LoadURL("www.daksithawithanage.wixsite.com/interactivesys", true);
            obj.EvalJS("var elem = document.querySelector('.site-message');elem.style.display = 'none'; ");
        }
        else
        {
            ui.SetActive(true);
            obj.LoadURL("www.daksithawithanage.wixsite.com/interactivesys", false);
        }
        
    }
    

    void ArduinoMessageReceived(object sender, MqttMsgPublishEventArgs e)
    {
        string msg = System.Text.Encoding.UTF8.GetString(e.Message);
        Debug.Log("MQTT INcoming MESSAGE");
        Debug.Log(msg);
        jsonmsg = JsonMapper.ToObject(msg);
        screenFlag = (int)jsonmsg["screenOn"];
        scrollFlag = (int)jsonmsg["scrollOFF"];
        
      
        
    }

}
