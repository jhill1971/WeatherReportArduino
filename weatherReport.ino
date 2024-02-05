/*
Arduino Program for Weather Report Application
Technologies Used:
Android Studio Hedgehog
Firebase
Arduino Uno WiFi Rev.2
DHT 11 Temperature and Humidity Module
*/

// WiFi Connection Info
#include <WiFiNINA.h>
char ssid[] = "ATTCthHRfi";          //  network SSID
char pass[] = "jdb?5ri%+36t";        // network password
int keyIndex = 0;                    // network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;         //connection status
WiFiServer server(80);               //server socket
WiFiClient client = server.available();

// Firebase Connection Info
#include <Config.h>
#include <Firebase.h>
#include <Firebase_Arduino_WiFiNINA.h>
#include <Firebase_TCP_Client.h>
char firebaseHost[] = "weather-report-23eca-default-rtdb.firebaseio.com";
char firebaseAuth[] = "firebase-adminsdk-i6807@weather-report-23eca.iam.gserviceaccount.com";
FirebaseData fbdo; // Declares the Firebase data object.

// Sensor Information
#include <WCS.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#define DHTTYPE DHT11 // Sensor Type
#define DHTPIN 2 // Connect data pin of DHT11 to D2
// static const int DHT_SENSOR_PIN = 2;
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  // Open connection to Serial Monitor at 9600 baud.
  Serial.begin(9600);
  // Start DHT11 Sensor
  dht.begin();
  // Connect to WiFI
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print("*");
    delay(300);
  }
  // Confirm WiFi Connection
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  // Connect to Firebase
  Firebase.begin(firebaseHost, firebaseAuth, ssid, pass);
  Firebase.reconnectWiFi(true);

}

void loop() {
  getSensorData();
}

void getSensorData() {
  delay(2000);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C "));


if (Firebase.setFloat(fbdo, "/FirebaseIOT/temperature", temperature)){
  Serial.println("PASSED");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
  Serial.println("------------------------------------");
  Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(fbdo, "/FirebaseIOT/humidity", humidity)){
  Serial.println("PASSED");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
  Serial.println("------------------------------------");
  Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}




