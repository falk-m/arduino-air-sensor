//install "DFRobot CCS811"
//Install "DHT sensor library"
//install "DFRobot_CCS811"

//#################### WIFI BEGIN
#include "arduino_secrets.h" 
#include <SPI.h>
#include <WiFi101.h>

//#################### TERMOMETER BEGIN
#include "DHT.h" //DHT Bibliothek laden
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); //Der Sensor wird ab jetzt mit „dth“ angesprochen

//#################### GaSensor BEGIN
#include "DFRobot_CCS811.h"
DFRobot_CCS811 CCS811;

//#################### LED
#define LEDPIN 6

void setup()
{
  Serial.begin(9600);
  setupGasSensorCCS811();
  setupTermometer();
  setupMQ7();
  setupWifi();
  setupLed();
}

void loop() 
{
  float* humidity_Temperatur = get_Humidity_Temperatur();
  float co = getMQ7_CoValue();
  float* co2_tvov = getGasSensorCCS811_Co2_tvov();

  ledOn();
  sendMessage(humidity_Temperatur[0], humidity_Temperatur[1], co, co2_tvov[0], co2_tvov[1]);
  delay(120000);
}

void sendMessage(float humidity, float temperatur, float co, float co2, float tvov){
  WiFiClient client;
  char host[] = HOST;

  String data = String("GET ") + PATH + "?secret=" +  SECRET + "&hum=" + humidity + "&temp=" + temperatur + "&co=" + co + "&co2=" + co2 + "&tv=" + tvov + " HTTP/1.1";
  Serial.println(data);

  if (client.connect(host, 80)) {
    Serial.println("connected to server");
    client.println(data);
    client.println(String("Host: ") + host);
    client.println("Connection: close");
    client.println();
    client.stop();
  }
  else {
    Serial.println("can not connect to Server");
  }
}

void setupLed(){
   pinMode(LEDPIN, OUTPUT);
}

void ledOn(){
  digitalWrite(LED_BUILTIN, HIGH);
}

void ledOff(){
  digitalWrite(LED_BUILTIN, LOW);
}

void setupWifi()
{
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  char ssid[] = WIFI_SSID;    // your network SSID (name)
  char pass[] = WIFI_PASS;    // your network password (use for WPA, or use as key for WEP)
  int status = WL_IDLE_STATUS;

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.print("You're connected to the network");
  printCurrentNet();
  printWiFiData();
}

void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void setupMQ7()
{
  //Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(8, INPUT);
}

void setupGasSensorCCS811()
{
  Serial.begin(115200);

  /*Wait for the chip to be initialized completely, and then exit*/
  while(CCS811.begin() != 0){
    Serial.println("fCCS811: ailed to init chip, please check the chip connection");
    //Serial.println(analogRead(A4));
    //Serial.println(analogRead(A5));
    delay(5000);
 }

  Serial.println("CCS811: connected");
}

void setupTermometer() 
{
  //Serial.begin(9600); //Serielle Verbindung starten
  dht.begin(); //DHT22 Sensor starten
}

float * get_Humidity_Temperatur() {
  
  //delay(2000);//Zwei Sekunden bis zur Messung warten damit der Sensor etwas //messen kann weil er relativ langsam ist
  float Luftfeuchtigkeit = dht.readHumidity(); //die Luftfeuchtigkeit auslesen und unter „Luftfeutchtigkeit“ speichern
  
  float Temperatur = dht.readTemperature();//die Temperatur auslesen und unter „Temperatur“ speichern
  
  Serial.print("Luftfeuchtigkeit: "); //Im seriellen Monitor den Text und 
  Serial.print(Luftfeuchtigkeit); //die Dazugehörigen Werte anzeigen
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(Temperatur);
  Serial.println(" Grad Celsius");

  static float result[2];
  result[0] = Luftfeuchtigkeit;
  result[1] = Temperatur;
  return result;
}

float getMQ7_CoValue() {
 //if (digitalRead(8)==LOW){
 //Serial.println("Warnung: Grenzwert überschritten!!!");
 //}
 //Serial.println(digitalRead(8));
 //Serial.println(analogRead(A0));

  int result = analogRead(A0);
  Serial.print("co: "); 
  Serial.println(result);

  return result;
}

float * getGasSensorCCS811_Co2_tvov()
{
  static float result[2];
  result[0] = 0;
  result[1] = 0;

  if(CCS811.checkDataReady() == true){
    Serial.print("CO2: ");
    result[0] = CCS811.getCO2PPM();
    Serial.print(result[0]);
    Serial.print("ppm, TVOC: ");
    result[1] = CCS811.getTVOCPPB();
    Serial.print(result[1]);
    Serial.println("ppb");
  } else {
    Serial.println("Data is not ready!");
  }
  /*!
  * @brief Set baseline
  * @param get from getBaseline.ino
  */
  CCS811.writeBaseLine(0x447B);
  //delay cannot be less than measurement cycle
  //delay(1000);

  return result;
}
