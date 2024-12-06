#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

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
const int SERVO = 34;
Servo myServo;
int curAngle = 90;

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Control the Motors</h1>";
  html += "<p>Use the W, A, S, D keys to control the motors.</p>";
  html += "<script>";
  html += "var pressed = false;";
  html += "document.addEventListener('keydown', function(event) { "
          "if (!pressed && event.key === 'w') { pressed=true;fetch('/forward'); } "
          "if (!pressed && event.key === 's') { pressed=true;fetch('/backward'); } "
          "if (!pressed && event.key === 'a') { pressed=true;fetch('/left'); } "
          "if (!pressed && event.key === 'd') { pressed=true;fetch('/right'); } });";
  html += "document.addEventListener('keyup', function(event) { "
          "pressed = false;"
          "if (['w', 'a', 's', 'd'].includes(event.key)) { fetch('/stop'); } });";
  html += "</script>";
  html += "</body></html>";
 
  server.send(200, "text/html", html);
}

void runLeft(bool forward) {

  digitalWrite(LEFT_FW, forward ? HIGH : LOW);
  digitalWrite(LEFT_BW, forward ? LOW : HIGH);
}

void runRight(bool forward) {
  digitalWrite(RIGHT_FW, forward ? HIGH : LOW);
  digitalWrite(RIGHT_BW, forward ? LOW : HIGH);
}

void spinServoClockwise() {
  curAngle -= 10;
  myServo.write(curAngle);
  server.send(200, "text/plain", "Turning clockwise")
}

void spinServoCounter() {
  curAngle += 10;
  myServo.write(curAngle);
  server.send(200, "text/plain", "Turning counterclockwise")
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
  

  //Attach the servo
  myServo.attach(SERVO);
  myServo.write(0);

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
  server.on("/weapon_clock", HTTP_GET, spinServoClockwise);
  server.on("/weapon_counter", HTTP_GET, spinServoCounter);
  
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