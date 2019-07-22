using LitJson;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using ZenFulcrum.EmbeddedBrowser;

public class RequestHandler {

    
    public MqttClient Arduino_client;
    private string broker_username;
    private string broker_password;
    private string Arduino_Broker_name;
    private int Broker_port;
    private string Arduino_TOPIC_NAME;
   

    public RequestHandler()
    {
        Arduino_Broker_name = "192.168.1.100";
        Broker_port = 1833;
        Arduino_TOPIC_NAME = "project/input/sensors";
        broker_username = "";
        broker_password = "";
        //Initalize the MQTT client
        MQTTHandler.Initialize(ref Arduino_client, Arduino_Broker_name, Broker_port, broker_username, broker_password);
        MQTTHandler.Subscribe(ref Arduino_client, Arduino_TOPIC_NAME);
    }
}
