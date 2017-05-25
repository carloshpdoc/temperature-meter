#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Constants
const char* ssid = "****";
const char* password = "pass";
const char* mqtt_server = "SeuBroke";

#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor 
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String temp_str; //see last code block below use these to convert the float that you get back from DHT to a string =str
String hum_str;
char temp[50];
char hum[50];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//Variables
int chk;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    Serial.println("getTemp");
  } else {
        Serial.println((char)payload[0]);
  }

}

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("espClient")) {
  client.connect("nodemcuesp-mqtt", "seu user", "seu password");
      
  Serial.println("connected");
  //subscribe to topic
  client.subscribe("getTemp");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println();

  dht.begin();
}

void loop()
{
  if (!client.connected()) {
    reconnect();
   }
  client.loop();

  float humidity = dht.readHumidity(); 
  float temperature = dht.readTemperature();
  float Ctemp = dht.convertFtoC(temperature);

  //Using Serial print to debug

  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
 
  //Preparing for mqtt send

    temp_str = String(Ctemp); //converting ftemp (the float variable above) to a string 
    temp_str.toCharArray(temp, temp_str.length() + 1);

    dtostrf(temperature, 4, 1, temp);
    client.publish("temp", temp); 
    
if (temperature > float(30)){
    Serial.print("Publish message: ");//Serial prints are to help with debuging
  
   
    client.publish("temp/realtime", temp); 
    
  Serial.print("Publish ok: ...");
  Serial.println(temp);
}
  delay(2000);
}
