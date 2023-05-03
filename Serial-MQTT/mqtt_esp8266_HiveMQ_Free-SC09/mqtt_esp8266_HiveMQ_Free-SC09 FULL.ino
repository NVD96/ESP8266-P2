/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <SoftwareSerial.h>

// Update these with values suitable for your network.

const char* ssid = "Nha nghi ca";
const char* password = "t500vinhtan";
const char* mqtt_server = "88ac8cf1653f48edb542894587248216.s2.eu.hivemq.cloud";

WiFiClientSecure espClient;               //PHAI KHAI BAO HAM WiFiClientSecure MOI CHO PHEP KET NOI MQTT FREE
PubSubClient client(espClient);
//SoftwareSerial mySerial(4, 5);



unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
String gui = "";

//byte hexToasc[23] = {0x02, 0x03, 0x05, 0x06, 0x15, 0x20, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
//byte ascTohex[23] = {'X', 'T', 'Q', 'K', 'N','S', '0', '1', '2', '3', '4', '5' ,'6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

String hexToString(byte kitu){
  
  if(kitu == 0x02) return "X";
  if(kitu == 0x03) return "T";
  if(kitu == 0x05) return "Q";
  if(kitu == 0x06) return "K";
  if(kitu == 0x15) return "N";
  if(kitu == 0x20) return "S";
  if(kitu == 0x30) return "0";
  if(kitu == 0x31) return "1";
  if(kitu == 0x32) return "2";
  if(kitu == 0x33) return "3";
  if(kitu == 0x34) return "4";
  if(kitu == 0x35) return "5";
  if(kitu == 0x36) return "6";
  if(kitu == 0x37) return "7";
  if(kitu == 0x38) return "8";
  if(kitu == 0x39) return "9";
  if(kitu == 0x41) return "A";
  if(kitu == 0x42) return "B";
  if(kitu == 0x43) return "C";
  if(kitu == 0x44) return "D";
  if(kitu == 0x45) return "E";
  if(kitu == 0x46) return "F";
  }

byte StringTohex(char kitu){
  if(kitu == 'X') return 0x02;
  if(kitu == 'T') return 0x03;
  if(kitu == 'Q') return 0x05;
  if(kitu == 'K') return 0x06;
  if(kitu == 'N') return 0x15;
  if(kitu == 'S') return 0x20;
  if(kitu == '0') return 0x30;
  if(kitu == '1') return 0x31;
  if(kitu == '2') return 0x32;
  if(kitu == '3') return 0x33;
  if(kitu == '4') return 0x34;
  if(kitu == '5') return 0x35;
  if(kitu == '6') return 0x36;
  if(kitu == '7') return 0x37;
  if(kitu == '8') return 0x38;
  if(kitu == '9') return 0x39;
  if(kitu == 'A') return 0x41;
  if(kitu == 'B') return 0x42;
  if(kitu == 'C') return 0x43;
  if(kitu == 'D') return 0x44;
  if(kitu == 'E') return 0x45;
  if(kitu == 'F') return 0x46;
  }

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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

void callback(char* topic, byte* payload, unsigned int length) {
  
  for (int i = 0; i < length; i++) {
    Serial.write(StringTohex(payload[i]));
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "VanDac", "Nguyenvandac37")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("inTopic", "hello world");
      // ... and resubscribe
      client.subscribe("outTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600, SERIAL_7E1);
  //mySerial.begin(9600, SWSERIAL_7E1);
  setup_wifi();
  espClient.setInsecure();        //PHAI SET O CHE DO SECURE MOI KET NOI DUOC MQTT HIVEMQ FREE
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  while(Serial.available()>0){
    gui += hexToString(Serial.read());
    delay(1);
    }
    
   if(gui != ""){
   client.publish("inTopic", gui.c_str());
   gui = "";
   }
   
}
