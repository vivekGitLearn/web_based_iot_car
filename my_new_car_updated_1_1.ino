#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char *ssid = "realme 3 Pro";   // Enter SSID here
const char *password = "pineapple";  // Enter Password here

/* Put IP Address details */
// IPAddress local_ip(192, 168, 1, 1);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

// Motor Pins
const int motor1Pin1 = D1;  // Motor right input 1
const int motor1Pin2 = D2;  // Motor right input 2
const int motor2Pin1 = D3;  // Motor left input 1
const int motor2Pin2 = D4;  // Motor left input 2
                            // const int enablePin1 = D5;  // Motor 1 enable pin
// const int enablePin2 = D6;  // Motor 2 enable pin

bool LED1status = LOW;
bool LED2status = LOW;
bool LED3status = LOW;
bool LED4status = LOW;
bool LED5status = LOW;


// Function to move the car forward
void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("forward");
  // delay(50);
  // analogWrite(enablePin1, 255);  // Set motor 1 speed to maximum
  // analogWrite(enablePin2, 255);  // Set motor 2 speed to maximum
}
// Function to move the car backward
void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  Serial.println("backward");
  // delay(50);
  // stopCar();
  // delay(50);
  // analogWrite(enablePin1, 255);  // Set motor 1 speed to maximum
  // analogWrite(enablePin2, 255);  // Set motor 2 speed to maximum
}
// Function to move the car right
void moveRight() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  // delay(50);
  // stopCar();
  // delay(50);
  Serial.println("right");
  // analogWrite(enablePin1, 255);  // Set motor 1 speed to maximum
  // analogWrite(enablePin2, 255);  // Set motor 2 speed to maximum
}
// Function to move the car left
void moveLeft() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  // delay(50);
  // stopCar();
  // delay(50);
  Serial.println("left");
  // analogWrite(enablePin1, 255);  // Set motor 1 speed to maximum
  // analogWrite(enablePin2, 255);  // Set motor 2 speed to maximum
}
// Function to stop the car
void stopCar() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("stop");
  // analogWrite(enablePin1, 0);  // Set motor 1 speed to 0
  // analogWrite(enablePin2, 0);  // Set motor 2 speed to 0
}

void setup() {
  Serial.begin(9600);
  // Set motor control pins as output
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  // pinMode(enablePin1, OUTPUT);
  // pinMode(enablePin2, OUTPUT);

  // Initialize motors to stop
  stopCar();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("wifi connected...!");
  Serial.print("got ip: ");
  Serial.print(WiFi.localIP());
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/left", handle_left);
  server.on("/forward", handle_forward);
  server.on("/right", handle_right);
  server.on("/stop", handle_stop);
  server.on("/backward", handle_backward);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if (LED1status) {
    moveForward();
  } else if (LED2status) {
    moveBackward();
  } else if (LED3status) {
    moveLeft();
  } else if (LED4status) {
    moveRight();
  } else {
    stopCar();
  }
}

void handle_OnConnect() {
  LED5status = HIGH;
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  Serial.println("stop");
  server.send(200, "text/html", SendHTML());
}

void handle_left() {

  LED3status = HIGH;
  LED5status = LOW;
  LED2status = LOW;
  LED1status = LOW;
  LED4status = LOW;
  Serial.println("Left");
  server.send(200, "text/html", SendHTML());
}

void handle_forward() {
  LED1status = HIGH;
  LED5status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  Serial.println("forward");
  server.send(200, "text/html", SendHTML());
}

void handle_right() {
  LED4status = HIGH;
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED5status = LOW;
  Serial.println("Right");
  server.send(200, "text/html", SendHTML());
}

void handle_backward() {
  LED2status = HIGH;
  LED1status = LOW;
  LED5status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  Serial.println("backward");
  server.send(200, "text/html", SendHTML());
}

void handle_stop() {
  LED5status = HIGH;
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  Serial.println("stop");
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>CAR Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-mid {display:inline ;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 20px;margin-bottom: 25px;margin-top: 25px; auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "   <script>\n";
  ptr += "  function sendLinkForward()\n";
  ptr += "   {\n";
  ptr += "   window.location.href = \"/forward\"; \n";
  ptr += "   }\n";
  ptr += "   function sendLinkBackWard()\n";
  ptr += "  {\n";
  ptr += "    window.location.href = \"/backward\"; \n";
  ptr += "   }\n";
  ptr += "   function sendLinkLeft()\n";
  ptr += "   {\n";
  ptr += "    window.location.href = \"/left\"; \n";
  ptr += "   }\n";
  ptr += "   function sendLinkLeft()\n";
  ptr += "   {\n";
  ptr += "    window.location.href = \"/right\"; \n";
  ptr += "   }\n";
  ptr += "   function sendLinkStop()\n";
  ptr += "   {\n";
  ptr += "    window.location.href = \"/stop\"; \n";
  ptr += "   }\n";
  ptr += "   </script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESp3288 car control</h1>\n";
  ptr += "<a class=\"button button-off\"     onclick=\"sendLinkForward()\" onmouseleave=\"sendLinkStop()\" href=\"/forward\">forward</a>\n";
  ptr += "<a class=\"button-mid button-off\" onclick=\"sendLinkLeft()\" onmouseleave=\"sendLinkStop()\" href=\"/left\">left</a>\n";
  ptr += "<a class=\"button-mid button-off\" href=\"/stop\">stop</a>\n";
  ptr += "<a class=\"button-mid button-off\" onclick=\"sendLinkBackWard()\" onmouseleave=\"sendLinkStop()\" href=\"/right\">right</a>\n";
  ptr += "<p>.</p>\n";
  ptr += "<a class=\"button button-off\" href=\"/backward\">backward</a>\n";


  // ptr += "<a class=\"button button-off\"     ontouchstart=\"sendLinkForward()\" ontouchend=\"sendLinkStop()\" href=\"/forward\">forward</a>\n";
  // ptr += "<a class=\"button-mid button-off\" ontouchstart=\"sendLinkLeft()\" ontouchend=\"sendLinkStop()\" href=\"/left\">left</a>\n";
  // ptr += "<a class=\"button-mid button-off\" href=\"/stop\">stop</a>\n";
  // ptr += "<a class=\"button-mid button-off\" ontouchstart=\"sendLinkBackWard()\" ontouchend=\"sendLinkStop()\" href=\"/right\">right</a>\n";
  // ptr += "<p>.</p>\n";
  // ptr += "<a class=\"button button-off\" href=\"/backward\">backward</a>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}