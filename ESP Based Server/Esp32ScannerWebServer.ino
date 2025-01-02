#include <WiFi.h>
#include <SPIFFS.h>

// Pin definitions for LEDs
#define RED_LED 2    // Power-on indicator
#define YELLOW_LED 4 // Indicates connected to AP
#define BLUE_LED 5   // Blinks during Wi-Fi scanning

// Wi-Fi server
WiFiServer server(80);
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// HTML page stored in program memory
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<title>ESP32 Control Panel</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { font-family: Arial, sans-serif; margin: 0; padding: 0; text-align: center; background-color: #f4f4f4; }
header { background: #333; color: #fff; padding: 10px 0; }
header h1 { margin: 0; }
button { padding: 10px 20px; margin: 10px; background: #007BFF; color: #fff; border: none; border-radius: 5px; cursor: pointer; }
button:hover { background: #0056b3; }
section { margin: 20px; padding: 20px; background: #fff; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.3); }
</style>
</head>
<body>
<header>
  <h1>ESP32 Control Panel</h1>
</header>
<section>
  <h2>Scan Wi-Fi Networks</h2>
  <p>Click the button below to scan for available Wi-Fi networks.</p>
  <button onclick="fetch('/scan').then(() => location.reload())">Start Scan</button>
  <pre id="results">%s</pre>
</section>
</body>
</html>
)rawliteral";

// Function to scan Wi-Fi networks
String scanWiFiNetworks() {
  String results = "";
  digitalWrite(BLUE_LED, HIGH); // Turn on blue LED during scanning
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    results += "SSID: " + String(WiFi.SSID(i)) + ", RSSI: " + String(WiFi.RSSI(i)) + "\n";
  }
  digitalWrite(BLUE_LED, LOW); // Turn off blue LED after scanning
  return results;
}

// Function to handle HTTP requests
void handleClient(WiFiClient client) {
  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("GET /scan") >= 0) {
    String results = scanWiFiNetworks();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.print(results);
  } else {
    char htmlBuffer[2048];
    snprintf(htmlBuffer, sizeof(htmlBuffer), htmlPage, "No scan results yet. Click 'Start Scan'.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.print(htmlBuffer);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH); // Turn on red LED initially

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure static IP");
  }

  // Connect to Wi-Fi
  WiFi.begin("MY SSID", "PWD"); // Replace with your Wi-Fi credentials

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("Connected to Wi-Fi");
  digitalWrite(YELLOW_LED, HIGH); // Turn on yellow LED when connected

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
    client.stop();
  }
}
