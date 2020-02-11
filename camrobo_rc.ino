#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define APSSID "ROBOT1"
#define APPSK "ninjaninja"

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer webServer(80);

/**
 * 目 LED
 */
#define PIN_EYE_R 16
#define PIN_EYE_L 5

/***
 * DRV8835
 */
#define PIN_AIN1 14 // 右モーター 青
#define PIN_AIN2 12 // 右モーター 黃
#define PIN_BIN1 15 // 左モーター 青
#define PIN_BIN2 13 // 左モーター 黃

#define PWM_FREQ 1000 // PWM Frequency: 1000Hz(1kHz)
#define PWM_RANGE 100 // PWM range: 100

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}                                                          

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = webServer.streamFile(file, contentType);
    file.close();
    return true;
  }
  Serial.println("\tFile Not Found.");
  return false;
}

void handleForward() {
  forward();
  webServer.send(200, "text/plain", "Forward");
}
void handleReverse() {
  reverse();
  webServer.send(200, "text/plain", "Reverse");
}
void handleLeft() {
  left();
  webServer.send(200, "text/plain", "Left");
}
void handleRight() {
  right();
  webServer.send(200, "text/plain", "Right");
}
void handleStop() {
  stop();
  webServer.send(200, "text/plain", "Stop");
}

/**
 * 起動時に1度だけ呼ばれる処理
 * 各種初期化を行う
 */
void setup() {
  delay(1000);
  SPIFFS.begin();
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  webServer.onNotFound([]() {
    if (!handleFileRead(webServer.uri())) {
      webServer.send(404, "text/plain", "404: Not Found");
    }
  });

  // 下記URLにアクセスされたら、対応する処理を呼ぶ
  webServer.on("/forward", handleForward);
  webServer.on("/reverse", handleReverse);
  webServer.on("/left", handleLeft);
  webServer.on("/right", handleRight);
  webServer.on("/stop", handleStop);
  webServer.begin();

  // Init GPIO mode
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
  pinMode(PIN_EYE_R, OUTPUT);
  pinMode(PIN_EYE_L, OUTPUT);

  // Initialize PWM
  analogWriteFreq(PWM_FREQ);
  analogWriteRange(PWM_RANGE);
}

/**
 * setup 完了後、ずっと呼ばれる処理
 * 1. Webサーバーがブラウザからのアクセスを待ち受ける
 * 2. 目を光らせ続ける
 */
void loop() {
  webServer.handleClient();
  digitalWrite(PIN_EYE_R, HIGH);
  digitalWrite(PIN_EYE_L, HIGH);
}

void forward() {
  Serial.println("forward");
  analogWrite(PIN_AIN1, 0);
  analogWrite(PIN_AIN2, 100);
  analogWrite(PIN_BIN1, 0);
  analogWrite(PIN_BIN2, 100);
}

void reverse() {
  Serial.println("reverse");
  analogWrite(PIN_AIN1, 100);
  analogWrite(PIN_AIN2, 0);
  analogWrite(PIN_BIN1, 100);
  analogWrite(PIN_BIN2, 0);
}

void left() {
  Serial.println("left");
  analogWrite(PIN_AIN1, 0);
  analogWrite(PIN_AIN2, 100);
  analogWrite(PIN_BIN1, 100);
  analogWrite(PIN_BIN2, 0);
}

void right() {
  Serial.println("left");
  analogWrite(PIN_AIN1, 100);
  analogWrite(PIN_AIN2, 0);
  analogWrite(PIN_BIN1, 0);
  analogWrite(PIN_BIN2, 100);
}

void stop() {
  Serial.println("stop");
  analogWrite(PIN_AIN1, 0);
  analogWrite(PIN_AIN2, 0);
  analogWrite(PIN_BIN1, 0);
  analogWrite(PIN_BIN2, 0);
}
