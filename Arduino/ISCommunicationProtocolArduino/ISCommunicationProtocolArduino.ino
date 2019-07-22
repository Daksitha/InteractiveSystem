#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/////Sensor 1/////
int trigPin1_left = 7;
int echoPin1_left = 6;
/////Sensor 2////////////
int trigPin1_right = 13;
int echoPin1_right = 12;
///////Sensor 3//////////
int trigPin1_front = 9;
int echoPin1_front = 8;


////////////////Distance//////////
int distance1=-1; //left sensor
int distance2=-1; //right sensor
int distance3=-1; //front sensor

// Function prototypes
void subscribeReceive(char* topic, byte* payload, unsigned int length);
//MQTT Topic names 
const char* SENSOR_TOPIC = "project/input/sensors" ; 
//json memory
StaticJsonDocument<200> doc;;
//JsonObject& doc  = jsonBuffer.createObject();
char jsonChar[200];

//MQTT message 
int ScreenGlobal = 0;
int ScrollOffGlobal = 0;
 
// Set your MAC address and IP address here
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 111);
 
// Make sure to leave out the http and slashes!
//const char* server = "localhost";
 byte server[] = { 192, 168, 1, 100};
// Ethernet and MQTT related objects
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void setup()
{
  // Useful for debugging purposes  
  Serial.begin(9600);
  pinMode(trigPin1_left, OUTPUT);
  pinMode(echoPin1_left, INPUT);

  pinMode(trigPin1_front, OUTPUT);
  pinMode(echoPin1_front, INPUT);
  
  pinMode(trigPin1_right, OUTPUT);
  pinMode(echoPin1_right, INPUT);
  //Create a JSON 


  

  // Start the ethernet connection
  Ethernet.begin(mac, ip);              
  
  // Ethernet takes some time to boot!
  delay(3000);                          
 
  // Set the MQTT server to the server stated above ^
  mqttClient.setServer(server, 1883);   
 
  // Attempt to connect to the server with the ID "myClientID"
  if (mqttClient.connect("")) 
  {
    Serial.println("Connection has been established, well done");
 
    // Establish the subscribe event
    mqttClient.setCallback(subscribeReceive);
  } 
  else 
  {
    Serial.println("Looks like the server connection failed...");
  }

}

void firstsensor(){ // This function is for left sensor.
  int duration1;
  //int duration2;
  
  digitalWrite (trigPin1_left, HIGH);
  delayMicroseconds (5000);
  digitalWrite (trigPin1_left, LOW);
  duration1 = pulseIn (echoPin1_left, HIGH);
  distance1 = (duration1/2) / 29.1;

  Serial.print("LEFT Sensor: ");
  Serial.print(distance1);  
  Serial.print("cm    ");
      
    if(distance1>= 1 && distance1<=50 )
    {
    // Serial.print("Debug Left sensor");
      ScreenGlobal = 1;
    }
    if(distance1 < 1 || distance1 > 50 )
    {
      ScreenGlobal = 0 ;
    }  
    //check for the srolling off function 
    if(distance1>= 1 && distance1<=10 )
    {
      ScrollOffGlobal = 1;
    }
    if(distance1 < 1 || distance1 > 10 )
    {
      ScrollOffGlobal= 0;
    
    } 
}

void loop()
{
  // This is needed at the top of the loop!
  mqttClient.loop();
 
  // Ensure that we are subscribed to the topic "MakerIOTopic"
  mqttClient.subscribe("project/input/sensors");
 // mqttClient.subscribe("MakerIOTopic");

  firstsensor();
  doc["screenOn"] = ScreenGlobal;
  doc["scrollOFF"] = ScrollOffGlobal;
  serializeJson(doc, jsonChar);
  size_t n = serializeJson(doc, jsonChar);
  
 /////////////////////////////////////////////////////////

  // Attempt to publish a value to the topic "MakerIOTopic"
  if(mqttClient.publish(SENSOR_TOPIC, jsonChar ))
  {
    Serial.println("Publish message success");
  }
  else
  {
    Serial.println("Could not send message :(");
  }
 
  // Dont overload the server!
  delay(2000);
}
void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  // Print the topic
  Serial.print("Topic: ");
  Serial.println(topic);
 
  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
  }
 
  // Print a newline
  Serial.println("");
}
