using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using LitJson;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Text;

public class MQTTHandler : MonoBehaviour
{

    static public void Initialize(ref MqttClient client, string broker_name, int broker_port, string username, string password)
    {
        if (broker_name == null)
            throw new System.ArgumentException("Paramenter cannot be null", "broker_name");
        if (broker_name.Trim() == "")
            throw new System.ArgumentException("Paramenter cannot be empty", "broker_name");
        if (username == null)
            throw new System.ArgumentException("Paramenter cannot be null", "username");
        if (password == null)
            throw new System.ArgumentException("Paramenter cannot be null", "password");

        // Forming a certificate based on a TextAsset
        //X509Certificate cert = new X509Certificate();
        //		cert.Import(certificate.bytes);
        //		Debug.Log("Using the certificate '" + cert + "'");
        string client_ID = Guid.NewGuid().ToString();
        Debug.Log("Clinet_ID" + client_ID);
        try
        {
            Debug.Log(string.Format("[MQTT]: Client {2} is trying to connect to tcp://{0}:{1}", broker_name, broker_port, client_ID));
            client = new MqttClient(broker_name);
            //client = new MqttClient(broker_name,1883,true,cert);
            client.Connect(client_ID, username,password);
            Debug.Log(string.Format("[MQTT]: The connection to tcp://{0}:{1} has succesfully established on client {2}", broker_name, broker_port, client_ID));
        }
        catch (Exception e)
        {
            throw new System.Exception(string.Format("[MQTT]: The connection to tcp://{0}:{1} has failed on client {2} due to {3}", broker_name, broker_port, client_ID, e));
        }
    }

    static public void Subscribe(ref MqttClient client, string topic_name)
    {
        if (client == null)
            throw new System.ArgumentException("Paramenter cannot be null", "client");
        if (topic_name == null)
            throw new System.ArgumentException("Paramenter cannot be null", "topic_name");
        if (topic_name.Trim() == "")
            throw new System.ArgumentException("Paramenter cannot be empty", "topic_name");
        try
        {
            byte[] qosLevels = { MqttMsgBase.QOS_LEVEL_AT_LEAST_ONCE };
            client.Subscribe(new string[] { topic_name }, qosLevels);
            Debug.Log(string.Format("[MQTT]: The subscription to topic {0} has been succesfully made on client {1}", topic_name, client.ClientId));
        }
        catch (Exception e)
        {
            throw new System.Exception(string.Format("[MQTT]: The subscription to topic {0} has failed on client {1} due to {2}", topic_name, client.ClientId, e));
        }
    }

    static public void Publish(ref MqttClient client, string topic_name, string message)
    {
        if (client == null)
            throw new System.ArgumentException("Paramenter cannot be null", "client");
        if (topic_name == null)
            throw new System.ArgumentException("Paramenter cannot be null", "topic_name");
        if (topic_name.Trim() == "")
            throw new System.ArgumentException("Paramenter cannot be empty", "topic_name");
        try
        {
            client.Publish(topic_name, Encoding.UTF8.GetBytes(message), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
        }
        catch (Exception e)
        {
            throw new System.Exception(string.Format("[MQTT]: Failed to publish the message on topic {0} on client {1} due to {2}", topic_name, client.ClientId, e));
        }

    }
}