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
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Poppins">
<style>
body,h1,h2,h3,h4,h5 {font-family: "Poppins", sans-serif}
body {font-size:16px;}
.w3-half img{margin-bottom:-6px;margin-top:16px;opacity:0.8;cursor:pointer}
.w3-half img:hover{opacity:1}
</style>
</head>
<body>

<!-- Sidebar/menu -->
<nav class="w3-sidebar w3-red w3-collapse w3-top w3-large w3-padding" style="z-index:3;width:300px;font-weight:bold;" id="mySidebar"><br>
  <a href="javascript:void(0)" onclick="w3_close()" class="w3-button w3-hide-large w3-display-topleft" style="width:100%;font-size:22px">Close Menu</a>
  <div class="w3-container">
    <h3 class="w3-padding-64"><b>ESP32 Control<br>Panel</b></h3>
  </div>
  <div class="w3-bar-block">
    <a href="#scan" onclick="w3_close()" class="w3-bar-item w3-button w3-hover-white">Scan</a>
    <a href="#download" onclick="w3_close()" class="w3-bar-item w3-button w3-hover-white">Download</a>
    <a href="#sniff" onclick="w3_close()" class="w3-bar-item w3-button w3-hover-white">Sniff</a>
    <a href="#settings" onclick="w3_close()" class="w3-bar-item w3-button w3-hover-white">Settings</a>
  </div>
</nav>

<!-- Top menu on small screens -->
<header class="w3-container w3-top w3-hide-large w3-red w3-xlarge w3-padding">
  <a href="javascript:void(0)" class="w3-button w3-red w3-margin-right" onclick="w3_open()">☰</a>
  <span>ESP32 Control</span>
</header>

<!-- Overlay effect when opening sidebar on small screens -->
<div class="w3-overlay w3-hide-large" onclick="w3_close()" style="cursor:pointer" title="close side menu" id="myOverlay"></div>

<!-- !PAGE CONTENT! -->
<div class="w3-main" style="margin-left:340px;margin-right:40px">

  <!-- Scan Section -->
  <div class="w3-container" style="margin-top:80px" id="scan">
    <h1 class="w3-jumbo"><b>Scan Networks</b></h1>
    <h1 class="w3-xxxlarge w3-text-red"><b>Available Networks</b></h1>
    <hr style="width:50px;border:5px solid red" class="w3-round">
    <p>Click the button below to start scanning for Wi-Fi networks.</p>
    <button class="w3-button w3-red w3-padding-large w3-hover-black">Start Scan</button>
    <p id="scan-results" class="w3-margin-top">Results will appear here...</p>
  </div>

  <!-- Download Section -->
  <div class="w3-container" id="download" style="margin-top:75px">
    <h1 class="w3-xxxlarge w3-text-red"><b>Download Logs</b></h1>
    <hr style="width:50px;border:5px solid red" class="w3-round">
    <p>Download captured data or logs from the ESP32 device.</p>
    <button class="w3-button w3-red w3-padding-large w3-hover-black">Download Logs</button>
  </div>
  
  <!-- Sniff Section -->
  <div class="w3-container" id="sniff" style="margin-top:75px">
    <h1 class="w3-xxxlarge w3-text-red"><b>Sniff Traffic</b></h1>
    <hr style="width:50px;border:5px solid red" class="w3-round">
    <p>Enable or disable traffic sniffing. Click the button to start sniffing traffic.</p>
    <button class="w3-button w3-red w3-padding-large w3-hover-black">Start Sniffing</button>
    <p id="sniff-status" class="w3-margin-top">Status: Idle</p>
  </div>

  <!-- Settings Section -->
  <div class="w3-container" id="settings" style="margin-top:75px">
    <h1 class="w3-xxxlarge w3-text-red"><b>Settings</b></h1>
    <hr style="width:50px;border:5px solid red" class="w3-round">
    <p>Modify the ESP32 configuration or view device info.</p>
    <button class="w3-button w3-red w3-padding-large w3-hover-black">Device Info</button>
    <button class="w3-button w3-red w3-padding-large w3-hover-black">Update Settings</button>
  </div>

</div>

<!-- W3.CSS Container -->
<div class="w3-light-grey w3-container w3-padding-32" style="margin-top:75px;padding-right:58px">
  <p class="w3-right">Powered by <a href="https://www.w3schools.com/w3css/default.asp" title="W3.CSS" target="_blank" class="w3-hover-opacity">w3.css</a></p>
</div>

<script>
// Script to open and close sidebar
function w3_open() {
  document.getElementById("mySidebar").style.display = "block";
  document.getElementById("myOverlay").style.display = "block";
}
 
function w3_close() {
  document.getElementById("mySidebar").style.display = "none";
  document.getElementById("myOverlay").style.display = "none";
}
</script>

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
  WiFi.begin("KRVL_Home_2.4_ext", "mart7-sandy-yon"); // Replace with your Wi-Fi credentials

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
