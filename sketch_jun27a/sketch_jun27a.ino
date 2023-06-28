#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
DHT dht(2, DHT11); // D4
#define BUZZER 12 //  D6
#define REDPIN 14 //D5
#define GREENPIN 16 //  D0
// SDA  - D2
// SCL - D1
LiquidCrystal_I2C lcd(0x27, 16, 2);
//const char* ssid = "...";          // Your WiFi SSID
//const char* password = "iallyieIA03%";  // Your WiFi password
//const char* serverAddress = "http://192.168.8.117:80"; // ip address and the port on backend
const char* ssid = "RCA-WiFii";          // Your WiFi SSID
const char* password = "@rca@2023";  // Your WiFi password
const char* serverAddress = "http://192.168.1.150"; // ip address and the port on backend
void setup()
{
  Serial.begin(115200);
  Serial.println("DHT11 Temperature and Humidity Sensor");
  dht.begin();  pinMode(BUZZER, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
    // Initialize the LCD display
  lcd.begin(16, 2);
    lcd.init();
  lcd.backlight();
 connectToWiFi();
}
void loop()
{
  delay(10000); // Wait for 10 seconds between readings
  //Collecting temperaures and humidity in the Rwanda Coding Academy Surroundings
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read data from DHT sensor");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C");
    Serial.println("");
    Serial.print(humidity);
    Serial.print(" %");        
    // Display temperature readings on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" ");
    //Display humidity readings on the LCD
    lcd.setCursor(0,1);
     lcd.print("Humidity: ");
     lcd.print("");
     lcd.print(humidity);
     lcd.print(" ");  
     //Alerting students when it is time to wear or remove the pullovers 
    if (temperature > 25)
    {
      //turn redLED ON
      digitalWrite(REDPIN, HIGH);
      //turn green LED OFF
      digitalWrite(GREENPIN, LOW);
      Serial.println();
      Serial.println("Students can remove pullovers now!");
      //Buzzer Beeping 10 times
      for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER, HIGH); // Turn on the buzzer
    delay(100); // Wait for 100 milliseconds
    digitalWrite(BUZZER, LOW); // Turn off the buzzer
    delay(100); // Wait for another 100 milliseconds before the next beep
  }
    }
    else
    {
      Serial.println();
      Serial.println("Students can wear pullovers now!");
      //turn REDLED OFF
      digitalWrite(REDPIN, LOW);
      //Turn the greenLED ON
      digitalWrite(GREENPIN, HIGH);
      //Turn the Buzzer OFF
      digitalWrite(BUZZER, LOW);      
    }
  }
   // Sending records to the web server
  sendDataToServer(temperature, humidity);
}
void connectToWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
void sendDataToServer(float temperature, float humidity)
{
  WiFiClient client;
  HTTPClient http;
  String device = "340722SPE05820";
  // Build the URL with query parameters
  String url = serverAddress;
  url += "/weather-station/backend.php";
  url += "?device="+device;
  url += "&temperature=" + String(temperature);
  url += "&humidity=" + String(humidity);
  // Send HTTP GET request
  http.begin(client, url);
  http.setTimeout(10000); // Set timeout to 10 seconds (for example)
  int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else
  {
    Serial.println("Error sending data to server");
  }
  http.end();
}
