#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";

// Flask server address
const char* serverAddress = "http://192.168.1.100:5000"; // Replace with your Flask server's IP and port

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  // Perform various tasks
  scanNetworks();
  delay(5000); // Wait for 5 seconds between actions
  sniffTraffic();
  delay(5000);
  downloadLogs();
  delay(5000);
  updateSettings();
  delay(10000); // Wait for 10 seconds before restarting the loop
}

void scanNetworks() {
  HTTPClient http;
  String url = String(serverAddress) + "/api/scan";

  Serial.println("Scanning networks...");
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error in GET request");
  }

  http.end();
}

void sniffTraffic() {
  HTTPClient http;
  String url = String(serverAddress) + "/api/sniff";

  Serial.println("Sniffing traffic...");
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error in GET request");
  }

  http.end();
}

void downloadLogs() {
  HTTPClient http;
  String url = String(serverAddress) + "/api/logs";

  Serial.println("Downloading logs...");
  http.begin(url);
  int httpResponseCode = http.POST("");

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error in POST request");
  }

  http.end();
}

void updateSettings() {
  HTTPClient http;
  String url = String(serverAddress) + "/api/settings";

  Serial.println("Updating settings...");
  http.begin(url);
  int httpResponseCode = http.POST("key=value"); // Replace with actual settings data if needed

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error in POST request");
  }

  http.end();
}
