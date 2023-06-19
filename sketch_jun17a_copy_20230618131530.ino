#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "DHTesp.h"

WebServer server(80);

const char* ssid = "Your WIFI SSID HERE";
const char* password = "Password goes here";

#define DHTPIN 21
DHTesp dht;

float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(9600);

  connectToWifi();

  server.on("/", handleRoot);
  server.begin();

  dht.setup(DHTPIN, DHTesp::DHT11);
}

void getTemperature(); // Function declaration
void getHumidity(); // Function declaration

void loop() {
  server.handleClient();

  getTemperature();
  getHumidity();

  delay(1000);
}

void connectToWifi() {
  WiFi.enableSTA(true);
  delay(2000);

  // Set your Static IP address
  IPAddress local_IP("yourlocal ip address goes here ");//remove quotations and insert commas where dots are - between the ip address numbers
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 1, 1);//optional
  IPAddress subnet(255, 255, 0, 0);//optional

  WiFi.config(local_IP, gateway, subnet);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String imagePath = "/web/DALL·E 2023-06-18 13.11.21.png";
  Serial.print("Image path: ");
  Serial.println(imagePath);

  String page = "<html><head>";
  page += "<style>body { text-align: center; font-family: 'Comic Sans MS', cursive; background-image: url('" + imagePath + "'); background-size: cover; }</style>";
  page += "<style>#temp { color: ";
  if (temperature > 30) {
    page += "red";
  } else if (temperature < 25) {
    page += "blue";
  } else {
    page += "black"; // Default color
  }
  page += "; }</style>";
  page += "</head><body>";
  page += "<h1>Temperature and Humidity Monitor</h1>"; // Updated title
  page += "<p>Temperature: <span id='temp'>";
  page += temperature;
  page += "&deg;C</span></p>";
  page += "<p>Humidity: ";
  page += humidity;
  page += "%</p>";
  page += "<h3>Artificial Weatherman:</h3>";
  page += "<p>";
  page += getArtificialWeatherman();
  page += "</p>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void getTemperature() {
  temperature = dht.getTemperature();
}

void getHumidity() {
  humidity = dht.getHumidity();
}

String getArtificialWeatherman() {
  if (temperature > 30 && humidity > 80) {
    return "Oh man, it's hot and very humid today. Better crank up that AC!";
  } else if (temperature > 30 && humidity > 50 && humidity <= 80) {
    return "It's hot today. Stay hydrated and keep cool!";
  } else if (temperature <= 30 && humidity > 80) {
    return "It's very humid. Prepare for a muggy day!";
  } else if (temperature <= 30 && humidity > 50 && humidity <= 80) {
    return "The weather is pleasant. Enjoy your day!";
  } else if (temperature < 10) {
    return "Brrr... It's chilly out there. Bundle up!";
  } else if (temperature > 35) {
    return "Whoa! It's scorching hot today. Find some shade!";
  } else if (humidity < 30) {
    return "It's quite dry. Don't forget to moisturize!";
  } else if (humidity > 90) {
    return "It's super humid. Embrace the tropical vibes!";
  } else {
    return "The Artificial Weatherman went for a walk :(...";
  }
}
