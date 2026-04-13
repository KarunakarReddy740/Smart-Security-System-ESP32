#include <WiFi.h>

#define PIR_PIN 4
#define LED_PIN 21
#define BUZZER_PIN 5
#define BUTTON_PIN 18

const char* ssid = "iPhone";
const char* password = "12345678";

String apiKey = "PJNRIFYVU7N9U9UW";

const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  delay(30000); // PIR warm-up
}

void loop() {

  int motion = digitalRead(PIR_PIN);
  int button = digitalRead(BUTTON_PIN);
  int emergency = (button == LOW) ? 1 : 0;

  Serial.print("Motion: ");
  Serial.print(motion);
  Serial.print(" | Emergency: ");
  Serial.println(emergency);

  if (motion == HIGH || emergency == 1) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (client.connect(server, 80)) {

    String data = "api_key=" + apiKey +
                  "&field1=" + String(motion) +
                  "&field2=" + String(emergency);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data);

    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Connection Failed");
  }

  client.stop();

  delay(15000);
}
