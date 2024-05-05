#include <Arduino.h>
#include "DHT_Async.h"
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include <HTTPClient.h>
#include <UrlEncode.h>

#include "secrets.h"
#include "ThingSpeak.h"                       // always include thingspeak header file after other header files and custom macros
#define REPORTING_PERIOD_MS_THINGSPEAK 20000  // report to ThingSpeak every 20s
#define ADC_VREF_mV    3300.0 
#define ADC_RESOLUTION 4096.0
#define PIN_LM35       35



//const char* ssid = "2593";
//const char* password = "12345678";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+353899522156";
String apiKey = "8902154";



#define BUTTON_PIN 15  // GIOP21 pin connected to button
#define BUTTON_PIN2 17


const int ledPin1 = 18; 
const int ledPin2 = 19; 

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin

int lastState2 = LOW;  // the previous state from the input pin
int currentState2;     // the current reading from the input pin


char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password
int keyIndex = 0;           // your network key Index number (needed only for WEP)

uint32_t tsLastReportThingSpeak = 0;  //4 byte unsigned int to to time ThingSpeak 20s
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number = 0;
String myStatus = "";

//const char* ssid = "VODAFONE-6FF0";
//const char* password = "eXfNsqM2LP7kbsaa";
//char ssid[] = "AndroidAP";     // your network SSID (name)
//char password[] = "rzyx6826";  // your network password
WebServer server(80);

/* Uncomment according to your sensortype. */
#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 4;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

#define REPORTING_PERIOD_MS 1000
float temperature;
float humidity;
float beat;
// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

 float readLM35() {
  int adcVal = analogRead(PIN_LM35);
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  float tempC = milliVolt / 10;
  return tempC;
}

uint32_t tsLastReport = 0;
void handleRoot() {

  String message = homePagePart1 + temperature + homePagePart2 + humidity + homePagePart3 + beat + homePagePart4 + readLM35() + homePagePart5;
  server.send(200, "text/html", message);
}




void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  Serial.println("Beat!");
}






void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
    Serial.println("");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}







void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak


  delay(2000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);   

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
   sendMessage("message bot for patient 1000 is now active");
   Serial.println("");

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
  /* pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    beat = pox.getHeartRate();
    Serial.print(beat);
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    tsLastReport = millis();
  }
*/
  if (measure_environment(&temperature, &humidity)) {
    Serial.print("T = ");
    Serial.print(temperature, 1);
    Serial.print(" deg. C, H = ");
    Serial.print(humidity, 1);
    Serial.println("%");
  }
  delay(1000);
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment(float* temperature, float* humidity) {
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 4000ul) {
    if (dht_sensor.measure(temperature, humidity)) {
      measurement_timestamp = millis();
      return (true);
    }
  }

  return (false);
}
/*
 * Main program loop.
 */
void loop() {

  // Make sure to call update as fast as possible
  server.handleClient();
  pox.update();







    currentState = digitalRead(BUTTON_PIN);
  currentState2 = digitalRead(BUTTON_PIN2);

  if (lastState == HIGH && currentState == LOW){
    Serial.println("button1 is pressed the doctor has been notified on is on the way for emergancy");
    Serial.println("");
      digitalWrite(ledPin1, HIGH);
     sendMessage("PATIENT NEEDS HELP EMERGANCY!!!");
  }
    else if (lastState == LOW && currentState == HIGH){    
    Serial.println("Button1 Was released help is on the way");
     Serial.println("");
     digitalWrite(ledPin1, LOW);      
 }




   if (lastState2 == HIGH && currentState2 == LOW){
    Serial.println("button2 is pressed assistance has been notified");
     Serial.println("");
    digitalWrite(ledPin2, HIGH);
    Serial.println("button1 is released the doctor has been notified and is on the way for assistance");  
     sendMessage("Patient needs assistance!");
    }
  else if (lastState2 == LOW && currentState2 == HIGH){
    Serial.println("button2 is released assistance is on the way"); 
    Serial.println("");
    digitalWrite(ledPin2, LOW);  
    //sendMessage("Patient needs assistance!");
  }
  // save the the last state
  lastState = currentState;
  lastState2 = currentState2;
  // save the the last state







 


  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    //  Serial.print("Heart rate:");
    beat = pox.getHeartRate();
    //  Serial.print(beat);
    //  Serial.print("bpm / SpO2:");
    //  Serial.print(pox.getSpO2());
    //  Serial.println("%");

    tsLastReport = millis();
  }

  /* Measure temperature and humidity.  If the functions returns
       true, then a measurement is available. */
  if (measure_environment(&temperature, &humidity)) {
    Serial.print("T = ");
    Serial.print(temperature, 1);
    Serial.print(" deg. C, H = ");
    Serial.print(humidity, 1);
    Serial.println("%");
   
  }

  ThingSpeak.setField(4, temperature);
  ThingSpeak.setField(3, number);

 
  if (millis() - tsLastReportThingSpeak > REPORTING_PERIOD_MS_THINGSPEAK ) {
     // figure out the status message
     if (temperature > 25 ) {
    myStatus = String("Temperature is too high.");
  } else if (temperature < 15 ) {
    myStatus = String("Temperature is too low.");
  } 
  else {
    myStatus = String("Temperature is fine.");
  }
    // set the status
    ThingSpeak.setStatus(myStatus);
  //pox.update();
    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }


    tsLastReportThingSpeak = millis();  //update the time stamp
      if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
pox.update();
    //delay(20000);  // Wait 20 seconds to update the channel again
  }
}
