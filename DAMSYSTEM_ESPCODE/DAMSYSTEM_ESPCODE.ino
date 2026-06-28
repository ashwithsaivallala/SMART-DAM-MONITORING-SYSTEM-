#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
String statusMsg = "No Data Yet";

void handleRoot() {
  String html = "<html><head><title>DAM MONITORING SYSTEM</title></head><body>";
  html += "<h2>DAM MONITORING SYSTEM</h2>";
  html += "<p><b>Status:</b><br><font color='blue'>" + statusMsg + "</font></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  WiFi.softAP("DAM_MONITOR_WIFI", "12345678");
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Access Point Started");
}

void loop() {
  server.handleClient();

  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    if (msg == "HIGH") {
      statusMsg = "🚨 High Water Level - UNSAFE";
      Serial.println("Updated webpage: HIGH");
    }
    else if (msg == "LOW") {
      statusMsg = "✅ Low Water Level - SAFE";
      Serial.println("Updated webpage: LOW");
    }
  }
}