#include <WiFiNINA.h>
#include <WiFiUdp.h>

// Set up some constants
const int DIFFUSER_OFF = 0;
const int DIFFUSER_PULSING = 1;
const int DIFFUSER_CONSTANT = 2;
const int LED_OFF = 0;
const int LED_RAINBOW = 1;
// The second button press "pauses" the rotating rainbow
const int LED_RAINBOW_PAUSE = 2;
const int LED_GREEN = 3;
const int LED_GREEN_BRIGHT = 4;
const int LED_BLUE = 5;
const int LED_BLUE_BRIGHT = 6;
const int LED_RED = 7;
const int LED_RED_BRIGHT = 8;
const int LED_YELLOW = 9;
const int LED_YELLOW_BRIGHT = 10;
const int LED_PURPLE = 11;
const int LED_PURPLE_BRIGHT = 12;
const int LED_TEAL = 13;
const int LED_TEAL_BRIGHT = 14;
const int LED_WHITE = 15;
const int LED_WHITE_BRIGHT = 16;

// Set up variables for use later
int ledPin = 13;
int diffuserPin = 12;
int ledState = LED_OFF;
int diffuserState = DIFFUSER_OFF;
char ssid[] = "";
char pass[] = "";
int radioStatus = WL_IDLE_STATUS;
unsigned int localPort = 2390;
char packetBuffer[255];
WiFiUDP Udp;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(diffuserPin, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Serial ready!");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not present");
    while(true);
  } else {
    Serial.println("WiFi detected.");
  }

  String fv = WiFi.firmwareVersion();
  Serial.print("Firmware version is: ");
  Serial.println(fv);

  while (radioStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    radioStatus = WiFi.begin(ssid, pass);

    // wait 7 seconds for connection:
    delay(7000);
  }
  
  Serial.print("Connected to network ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP server.");
  Udp.begin(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.println("Received a packet.");
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      // NUL-terminate the buffer
      packetBuffer[len] = 0;
      if (strcmp(packetBuffer, "DIFFUSE_PULSE") == 0) {
        pulseSignal(DIFFUSER_PULSING, diffuserPin);
      } else if (strcmp(packetBuffer, "DIFFUSE_CONSTANT") == 0) {
        pulseSignal(DIFFUSER_CONSTANT, diffuserPin);
      } else if (strcmp(packetBuffer, "DIFFUSE_OFF") == 0) {
        pulseSignal(DIFFUSER_OFF, diffuserPin);
      } else if (strcmp(packetBuffer, "LED_RAINBOW") == 0) {
        pulseSignal(LED_RAINBOW, ledPin);
      } else if (strcmp(packetBuffer, "LED_PURPLE") == 0) {
        pulseSignal(LED_PURPLE, ledPin);
      } else if (strcmp(packetBuffer, "LED_OFF") == 0) {
        pulseSignal(LED_OFF, ledPin);
      }
    }
  }
}

void pulseSignal(int targetState, int channel) {
  int maximum;
  int* state;
  int pulseCount;
  if (channel == ledPin) {
    maximum = 17;
    state = &ledState;
  } else if (channel == diffuserPin) {
    maximum = 3;
    state = &diffuserState;
  }
  if (*state <= targetState) {
    pulseCount = targetState - *state;
  } else {
    pulseCount = maximum + targetState - *state;
  }
  int i;
  for (i = 0; i < pulseCount; i++) {
    digitalWrite(channel, HIGH);
    delay(100);
    digitalWrite(channel, LOW);
    delay(100);
  }
  *state = targetState;
}
