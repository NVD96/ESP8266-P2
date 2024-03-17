
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</head>
<body>
<center>
<h1>CÀI ĐẶT CHỈ SỐ BAN ĐẦU<h1>
<h3>
<form action="/action_page">
  PHA A:<br>
  <input type="text" name="phaa" value=>
  <br>
  PHA B:<br>
  <input type="text" name="phab" value=>
  <br>
  PHA C:<br>
  <input type="text" name="phac" value=>
  <br><br>
  <input type="submit" value="CÀI ĐẶT">
</form> 
<h3>
</center>
</body>
</html>
)=====";

//SSID and Password of your WiFi router
const char* ssid = "K506";
const char* password = "";

unsigned long previousMillis = 0;
const long interval = 500;


//bien tam
int tempA = 0;
int tempB = 0;
int tempC = 0;

//Input contact MC
const int AA = D5;
const int BB = D6;
const int CC = D7;

//Counter MC
short countA;
short countB;
short countC;

//Status contact MC
bool statusA;
bool statusB;
bool statusC;

LiquidCrystal_I2C lcd(0x27,16,2);
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================
void handleForm() {

//Xoa EEFROM
// for (int i = 0; i < 512; ++i) {
//    EEPROM.write(i, 0);
//  }
//  EEPROM.commit();

 String phaseA = server.arg("phaa"); 
 String phaseB = server.arg("phab"); 
 String phaseC = server.arg("phac"); 
 Serial.print("PHA A:");
 Serial.println(phaseA);

 Serial.print("PHA B:");
 Serial.println(phaseB);

 Serial.print("PHA C:");
 Serial.println(phaseC);

 countA = phaseA.toInt();
 countB = phaseB.toInt();
 countC = phaseC.toInt();

  EEPROM.put(0, countA);
  EEPROM.put(2, countB);
  EEPROM.put(4, countC);
 EEPROM.commit();
 
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}

void chiso(){

 String s = "";
  s += "<head>";
  s += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>";
  s += "</head>";
  s += "<center>";
  s += "<h1>CHỈ SỐ MC";
  s += "<br>";
  s += "<table border = \"1\" ; bgcolor = \"#ffff00\">";
  s += "<tr>";
  s += "<th width = \"150px\" ; align = \"center\">";
  s += "<p>Pha A</p>";
  s += "</th>";
  s += "<th width = \"150px\" ; align = \"center\">";
  s += "<p>Pha B</p>";
  s += "</th>";
  s += "<th width = \"150px\" ; align = \"center\">";
  s += "<p>Pha C</p>";
  s += "</th>";
  s += "</tr>";
  s += "<tr>";
  s += "<th width = \"150px\" ; align = \"center\">";
  s += "<p>" + String(countA)+ "</p>";
  s += "</th>";
  s += "<th width = \"150px\" ; align = \"center\">";
  s += "<p>" + String(countB)+ "</p>";
  s += "</th>";
  s += "<th width = \"150px\" ; align = \"center\">";
  s += "<p>" + String(countC)+ "</p>";
  s += "</th>";
  s += "</tr>";
  s += "</table>";
  s += "</h1>";
  s += "</center>";
   server.send(200, "text/html", s); //Send web page
  }
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);
  EEPROM.begin(512);  //Initialize EEPROM

  digitalWrite(AA, 1);
  digitalWrite(BB, 1);
  digitalWrite(CC, 1);

 EEPROM.get(0,countA);
 EEPROM.get(2,countB);
 EEPROM.get(4,countC);
  
  Wire.begin(4, 5);           //Bắt đầu 2 chân4 SDA và chân5 SCK của I2C
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  //WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  WiFi.softAP(ssid, password);
  
  // Wait for connection
 // while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
 //   Serial.print(".");
  //}

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/setup", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
  server.on("/", chiso);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
  // Read Status Input
  statusA = digitalRead(AA);
  statusB = digitalRead(BB);
  statusC = digitalRead(CC);

  delay(10);

  if(tempA == 1 && statusA == 0){
    countA = countA +1;
    EEPROM.put(0, countA);
    EEPROM.commit();
    }

  if(tempB == 1 && statusB == 0){
    countB = countB +1;
    EEPROM.put(2, countB);
    EEPROM.commit();
    }


   if(tempC == 1 && statusC == 0){
    countC = countC +1;
    EEPROM.put(4, countC);
    EEPROM.commit();
    }
   tempA = statusA;
   tempB = statusB;
   tempC = statusC;
//Dislay LCD
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    lcd.clear();
    previousMillis = currentMillis;
   
   lcd.setCursor(0, 0);
   lcd.print("A:");
      
   lcd.setCursor(2, 0);
   lcd.print(countA);
   
   lcd.setCursor(8, 0);
   lcd.print("B:");

   lcd.setCursor(10, 0);
   lcd.print(countB);

   lcd.setCursor(0, 1);
   lcd.print("C:");
      
   lcd.setCursor(2, 1);
   lcd.print(countC);
   lcd.setCursor(16, 1);
  }

  if (currentMillis < previousMillis) {
    previousMillis = 0;
    }
  //Serial.println(countA);
  //Serial.println(countB);
  //Serial.println(previousMillis);

}