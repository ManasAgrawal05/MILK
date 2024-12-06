#include <WiFi.h>
#include <WebServer.h>

const bool FORWARD = true;
const bool BACKWARD = false;

const char* ssid = "utexas-iot";   // Your Wi-Fi SSID
const char* password = "15326009573635923140"; // Your Wi-Fi password


WebServer server(80);


// TODO: change these if needed
const int LEFT_FW = 32;
const int LEFT_BW = 33;
const int RIGHT_FW = 25;
const int RIGHT_BW = 26;


void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Control the Motors</h1>";
  html += "<p>Use the W, A, S, D keys to control the motors.</p>";
  html += "<script>";
  html += "document.addEventListener('keydown', function(event) { "
          "if (event.key === 'w') { fetch('/forward'); } "
          "if (event.key === 's') { fetch('/backward'); } "
          "if (event.key === 'a') { fetch('/left'); } "
          "if (event.key === 'd') { fetch('/right'); } });";
  html += "document.addEventListener('keyup', function(event) { "
          "if (['w', 's', 'a', 'd'].includes(event.key)) { fetch('/stop'); } });";
  html += "</script>";
  html += "</body></html>";
 
  server.send(200, "text/html", html);
}

void runLeft(bool forward) {
  // left is 1 and 2 i think
  digitalWrite(LEFT_FW, forward ? HIGH : LOW);
  digitalWrite(LEFT_BW, forward ? LOW : HIGH);
}

void runRight(bool forward) {
  // right is 3 and 4 i think
  digitalWrite(RIGHT_FW, forward ? HIGH : LOW);
  digitalWrite(RIGHT_BW, forward ? LOW : HIGH);
}

void forward() {
  runLeft(FORWARD);
  runRight(FORWARD);
  server.send(200, "text/plain", "Moving Forward");
}

void backward() {
  runLeft(BACKWARD);
  runRight(BACKWARD);
  server.send(200, "text/plain", "Moving Backward");
}

void left() {
  runLeft(FORWARD);
  runRight(BACKWARD);
  server.send(200, "text/plain", "Turning Left");
}

void right() {
  runLeft(BACKWARD);
  runRight(FORWARD);
  server.send(200, "text/plain", "Turning Right");
}

void stopMotors() {
  digitalWrite(LEFT_FW, LOW);
  digitalWrite(LEFT_BW, LOW);
  digitalWrite(RIGHT_FW, LOW);
  digitalWrite(RIGHT_BW, LOW);
  server.send(200, "text/plain", "Motors Stopped");
}

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_FW, OUTPUT);
  pinMode(LEFT_BW, OUTPUT);
  pinMode(RIGHT_FW, OUTPUT);
  pinMode(RIGHT_BW, OUTPUT);
 
  digitalWrite(LEFT_FW, LOW);
  digitalWrite(LEFT_BW, LOW);
  digitalWrite(RIGHT_FW, LOW);
  digitalWrite(RIGHT_BW, LOW);

  // Connect to Wi-Fi
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to WiFi");
 
  // Serve the web page
  server.on("/", HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, forward);
  server.on("/backward", HTTP_GET, backward);
  server.on("/left", HTTP_GET, left);
  server.on("/right", HTTP_GET, right);
  server.on("/stop", HTTP_GET, stopMotors);
  server.begin();
  Serial.println("HTTP server started");
  // Print the IP address
  Serial.print("ESP32 Web Server IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  Serial.println(WiFi.localIP());
}