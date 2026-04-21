

/////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo esc;
int escPin = 23;
Servo esc2;
int escPin2 = 22;

const char *ssid = "utexas-iot";
const char *password = "29869378965001014094";

WebServer server(80);

const int drivespeed = 250;
const int rotspeed = 100;

void handleRoot()
{
  String html = R"(
<html><body style="background:#111;color:#eee;font-family:sans-serif;text-align:center;padding-top:40px">
<h1>MILK Drive Control</h1>
<p id="status">Waiting for controller...</p>
<p>Connect Xbox controller via Bluetooth, then press any button.</p>
<script>
var gamepadIndex = null;
var currentCmd = '';

window.addEventListener('gamepadconnected', function(e) {
  gamepadIndex = e.gamepad.index;
  document.getElementById('status').textContent = 'Connected: ' + e.gamepad.id;
});

window.addEventListener('gamepaddisconnected', function(e) {
  gamepadIndex = null;
  document.getElementById('status').textContent = 'Controller disconnected';
  fetch('/stop');
  currentCmd = '';
});

function getCmd(gp) {
  var deadzone = 0.2;
  var x = gp.axes[0]; // left stick X
  var y = gp.axes[1]; // left stick Y  (-1 = up/forward)
  if (Math.abs(y) >= Math.abs(x) && Math.abs(y) > deadzone) {
    return y < 0 ? 'forward' : 'backward';
  } else if (Math.abs(x) > deadzone) {
    return x < 0 ? 'left' : 'right';
  }
  return 'stop';
}

function poll() {
  if (gamepadIndex !== null) {
    var gp = navigator.getGamepads()[gamepadIndex];
    if (gp) {
      var cmd = getCmd(gp);
      if (cmd !== currentCmd) {
        currentCmd = cmd;
        fetch('/' + cmd);
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

void forward()
{
  esc2.writeMicroseconds(1500 - drivespeed);
  esc.writeMicroseconds(1500 + drivespeed);
  server.send(200, "text/plain", "Moving Forward");
}

void backward()
{
  esc2.writeMicroseconds(1500 + drivespeed);
  esc.writeMicroseconds(1500 - drivespeed);
  server.send(200, "text/plain", "Moving Backward");
}

void left()
{
  esc2.writeMicroseconds(1500 + rotspeed);
  esc.writeMicroseconds(1500 + rotspeed);
  server.send(200, "text/plain", "Turning Left");
}

void right()
{
  esc2.writeMicroseconds(1500 - rotspeed);
  esc.writeMicroseconds(1500 - rotspeed);
  server.send(200, "text/plain", "Turning Right");
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

  esc.attach(escPin, 1000, 2000);
  esc2.attach(escPin2, 1000, 2000);
  esc.writeMicroseconds(1500);
  esc2.writeMicroseconds(1500);
  delay(2000); // wait for ESCs to arm

  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to WiFi");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, forward);
  server.on("/backward", HTTP_GET, backward);
  server.on("/left", HTTP_GET, left);
  server.on("/right", HTTP_GET, right);
  server.on("/stop", HTTP_GET, stopMotors);

  server.begin();
  Serial.println("HTTP server started");
  Serial.print("ESP32 Web Server IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();
}
