#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

int trigPin1_left = 7;
int echoPin1_left = 6;
int trigPin1_right = 10;
int echoPin1_right = 11;
int flag1_right=0;
int flag2_right=0;
int flag1_left=0;
int flag2_left=0;
int flag1_front=0;
int flag2_front=0;
int trigPin1_front = 9;
int echoPin1_front = 8;
int screen_ON=0;
int scroll_OFF=0;

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
int ScrollOffGlobal = 1;
 
// Set your MAC address and IP address here
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 103);
 
// Make sure to leave out the http and slashes!
//const char* server = "localhost";
 byte server[] = { 192, 168, 0, 112};
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

void firstsensor(){ // This function is for first sensor.
  int duration1, distance1,d_sensors=30;
  int duration2, distance2;
  
  digitalWrite (trigPin1_left, HIGH);
  delayMicroseconds (5000);
  digitalWrite (trigPin1_left, LOW);
  duration1 = pulseIn (echoPin1_left, HIGH);
  distance1 = (duration1/2) / 29.1;

  Serial.print("LEFT Sensor: ");
  Serial.print(distance1);  
  Serial.print("cm    ");
      
  if(distance1<=20)
  {
    flag1_left=2;
    if(distance1<=5){
      flag2_left=2;
      //Serial.print("    flag1: ");
      //Serial.println(flag1);
      Serial.print(" flag2: ");
      Serial.println(flag2_left);
      }
    else 
    {
      flag2_left=-1;
      //Serial.print("  flag1: ");
      //Serial.println(flag1);
      Serial.print(" flag2: ");
      Serial.println(flag2_left);
    }  
  }
  else
  {
    flag1_left=-1;
    //Serial.print("  flag1: ");
    //Serial.print(flag1);
    Serial.print(" flag2: ");
    Serial.println(flag2_left);
  }      
}


void secondsensor(){ // This function is for first sensor.
  int duration1, distance1,d_sensors=30;
  int duration2, distance2;
  
  digitalWrite (trigPin1_right, HIGH);
  delayMicroseconds (5000);
  digitalWrite (trigPin1_right, LOW);
  duration1 = pulseIn (echoPin1_right, HIGH);
  distance1 = (duration1/2) / 29.1;

  Serial.print("RIGHT Sensor: ");
  Serial.print(distance1);  
  Serial.print("cm    ");
      
  if(distance1<=20)
  {
    flag1_right=3;
    if(distance1<=5 ){
      flag2_right=3;
      //Serial.print("    flag1: ");
      //Serial.println(flag1);
      Serial.print(" flag2: ");
      Serial.println(flag2_right);
      }
    else 
    {
      flag2_right=-1;
      //Serial.print("  flag1: ");
      //Serial.println(flag1);
      Serial.print(" flag2: ");
      Serial.println(flag2_right);
    }  
  }
  else
  {
    flag1_right=-1;
    //Serial.print("  flag1: ");
    //Serial.print(flag1);
    Serial.print(" flag2: ");
    Serial.println(flag2_right);
  }      
}


void thirdsensor(){ // This function is for first sensor.
  int duration1, distance1,d_sensors=30;
  int duration2, distance2;
  
  digitalWrite (trigPin1_front, HIGH);
  delayMicroseconds (5000);
  digitalWrite (trigPin1_front, LOW);
  duration1 = pulseIn (echoPin1_front, HIGH);
  distance1 = (duration1/2) / 29.1;

  
  Serial.print("FRONT Sensor: ");
  Serial.print(distance1);  
  Serial.print("cm    ");
      
  if(distance1<=20 )
  {
    flag1_front=4;
    if(distance1<=5 ){
      flag2_front=4;
      //Serial.print("    flag1: ");
      //Serial.println(flag1);
      Serial.print(" flag2: ");
      Serial.println(flag2_front);
      }
    else 
    {
      flag2_front=-1;
      //Serial.print("  flag1: ");
      //Serial.println(flag1);
      Serial.print(" flag2: ");
      Serial.println(flag2_front);
    }  
  }
  else
  {
    flag1_front=-1;
    //Serial.print("  flag1: ");
    //Serial.print(flag1);
    Serial.print(" flag2: ");
    Serial.println(flag1_front);
  }      
}

void loop()
{
  // This is needed at the top of the loop!
  mqttClient.loop();
 
  // Ensure that we are subscribed to the topic "MakerIOTopic"
  mqttClient.subscribe("project/input/sensors");
 // mqttClient.subscribe("MakerIOTopic");
 //data to be send. 
 ////////////////////TO-DO////////////////////////////////
 
 //Saumya use your global flags to update these two whenever sensor(s) detects
 //If someone comes then the screenOn 
 //If someone touches the sensor then the scrollOFF set to 0 
 //The default values are given here. Set your global values as these default value
// so the defalt value
firstsensor();
  delay(1000);
  secondsensor();
  delay(1000);
  thirdsensor();
  delay(1000);
  if(flag1_front==4)
  {
    screen_ON=1;
    if(flag2_front==4 )
    {
     scroll_OFF=1;
    }
  }
  if(flag1_right==3){
     screen_ON=1;
     if(flag2_right==3 ){
     scroll_OFF=1;}
     }
     
  if(flag1_left==2){ 
     screen_ON=1;
     if(flag2_left==2 ){
     scroll_OFF=1;}
  }
  ScreenGlobal=screen_ON;
  ScrollOffGlobal=scroll_OFF;
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
  delay(1000);
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
