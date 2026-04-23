#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo esc;
int escPin = 23; // connect ESC signal wire here
Servo esc2;
int escPin2 = 22;

const bool FORWARD = true;
const bool BACKWARD = false;

const char *ssid = "utexas-iot";               // Your Wi-Fi SSID
const char *password = "29869378965001014094"; // Your Wi-Fi password

WebServer server(80);

// TODO: change these if needed
const int LEFT_FW = 32;
const int LEFT_BW = 33;
const int RIGHT_FW = 25;
const int RIGHT_BW = 26;

const int drivespeed = 150;
const int rotspeed = 100;

const int SERVO = 16;
Servo myServo;
int curAngle = 90;

void handleRoot()
{
  String html = R"(
<html><body style="background:#111;color:#eee;font-family:sans-serif;text-align:center;padding-top:40px">
<h1>MILK Drive Control</h1>
<p id="status">Waiting for controller...</p>
<p>Connect Xbox controller via Bluetooth, then press any button.</p>
<script>
var gamepadIndex = null;
var prevLeft = 0, prevRight = 0;
var deadzone = 0.1;

window.addEventListener('gamepadconnected', function(e) {
  gamepadIndex = e.gamepad.index;
  document.getElementById('status').textContent = 'Connected: ' + e.gamepad.id;
});

window.addEventListener('gamepaddisconnected', function(e) {
  gamepadIndex = null;
  document.getElementById('status').textContent = 'Controller disconnected';
  fetch('/drive?left=0&right=0');
  prevLeft = 0; prevRight = 0;
});

function applyDeadzone(val) {
  return Math.abs(val) < deadzone ? 0 : val;
}

function poll() {
  if (gamepadIndex !== null) {
    var gp = navigator.getGamepads()[gamepadIndex];
    if (gp) {
      var left  = applyDeadzone(gp.axes[1]); // left stick Y
      var right = applyDeadzone(gp.axes[3]); // right stick Y
      left  = Math.round(left  * 100) / 100;
      right = Math.round(right * 100) / 100;
      if (Math.abs(left - prevLeft) > 0.02 || Math.abs(right - prevRight) > 0.02) {
        prevLeft  = left;
        prevRight = right;
        fetch('/drive?left=' + left + '&right=' + right);
      }
    }
  }
  requestAnimationFrame(poll);
}

requestAnimationFrame(poll);
</script>
</body></html>
)";
  server.send(200, "text/html", html);
}

// left/right are axis values in [-1, 1]: -1 = full forward, +1 = full backward
void handleDrive()
{
  float left = server.arg("left").toFloat();
  float right = server.arg("right").toFloat();

  left = constrain(left, -1.0f, 1.0f);
  right = constrain(right, -1.0f, 1.0f);

  // esc2 = left motor:  forward was 1500 - drivespeed  (axis -1 -> 1500 - drivespeed)
  esc2.writeMicroseconds(1500 + (int)(left * drivespeed));
  // esc  = right motor: forward was 1500 + drivespeed  (axis -1 -> 1500 + drivespeed)
  esc.writeMicroseconds(1500 - (int)(right * drivespeed));

  server.send(200, "text/plain", "OK");
}

void stopMotors()
{
  esc2.writeMicroseconds(1500);
  esc.writeMicroseconds(1500);
  server.send(200, "text/plain", "Motors Stopped");
}

void setup()
{
  Serial.begin(115200);
  pinMode(LEFT_FW, OUTPUT);
  pinMode(LEFT_BW, OUTPUT);
  pinMode(RIGHT_FW, OUTPUT);
  pinMode(RIGHT_BW, OUTPUT);

  digitalWrite(LEFT_FW, LOW);
  digitalWrite(LEFT_BW, LOW);
  digitalWrite(RIGHT_FW, LOW);
  digitalWrite(RIGHT_BW, LOW);

  // Attach the servo
  esc.attach(escPin, 1000, 2000); // min/max pulse width in µs
  esc2.attach(escPin2, 1000, 2000);
  esc.writeMicroseconds(1500); // neutral
  esc2.writeMicroseconds(1500);
  (2000);

  // Connect to Wi-Fi
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to WiFi");

  // Serve the web page
  server.on("/", HTTP_GET, handleRoot);
  server.on("/drive", HTTP_GET, handleDrive);
  server.on("/stop", HTTP_GET, stopMotors);

  server.begin();
  Serial.println("HTTP server started");
  // Print the IP address
  Serial.print("ESP32 Web Server IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();
  // Serial.println(WiFi.localIP());
}

/////////////////////////////////////////////////////////////////////////
