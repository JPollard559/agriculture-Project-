 #include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WiFiServer server(80);
Servo seedDispenserServo;
Servo weedCutterServo;

// Define pins for motor control
const int motorPin1 = D1;
const int motorPin2 = D2;

// Define sensor pins
const int soilMoisturePin = A0;
const int temperaturePin = A0;
const int trigPin = D4;
const int echoPin = D5;

// Define servo pins
const int seedDispenserServoPin = D6;
const int weedCutterServoPin = D7;

// Thresholds
const int moistureThreshold = 500; // Adjust as needed
const int temperatureThreshold = 25; // Adjust as needed
const int weedThreshold = 10; // Adjust as needed

void setup() {
  Serial.begin(9600);
  delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL to connect: http://");
  Serial.println(WiFi.localIP());

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  seedDispenserServo.attach(seedDispenserServoPin);
  weedCutterServo.attach(weedCutterServoPin);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/dispense_seeds") != -1) {
    dispenseSeeds();
  }

  if (request.indexOf("/remove_weed") != -1) {
    removeWeed();
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  // Add buttons to the HTML page
  client.println("<h1>Control Functions</h1>");
  client.println("<form>");
  client.println("<button type='submit' formaction='/dispense_seeds'>Dispense Seeds</button>");
  client.println("<button type='submit' formaction='/remove_weed'>Remove Weed</button>");
  client.println("</form>");

  client.println("</html>");

  delay(1000);
}

void dispenseSeeds() {
  seedDispenserServo.write(90); // Adjust angle as needed
  delay(1000); // Adjust delay as needed
  seedDispenserServo.write(0); // Reset servo position
}

void removeWeed() {
  weedCutterServo.write(90); // Adjust angle as needed
  delay(1000); // Adjust delay as needed
  weedCutterServo.write(0); // Reset servo position
}
