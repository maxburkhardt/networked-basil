#include <WiFiNINA.h>
#include <WiFiUdp.h>

// Set up some constants
const int LED_PIN = 8;
const int DIFFUSER_PIN = 7;

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
int ledState = LED_OFF;
int diffuserState = DIFFUSER_OFF;
char ssid[] = "";
char pass[] = "";
int radioStatus = WL_IDLE_STATUS;
unsigned int localPort = 2390;
char packetBuffer[255];
WiFiUDP Udp;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(DIFFUSER_PIN, OUTPUT);

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
        pulseSignal(DIFFUSER_PULSING, DIFFUSER_PIN);
      } else if (strcmp(packetBuffer, "DIFFUSE_CONSTANT") == 0) {
        pulseSignal(DIFFUSER_CONSTANT, DIFFUSER_PIN);
      } else if (strcmp(packetBuffer, "DIFFUSE_OFF") == 0) {
        pulseSignal(DIFFUSER_OFF, DIFFUSER_PIN);
      } else if (strcmp(packetBuffer, "LED_RAINBOW") == 0) {
        pulseSignal(LED_RAINBOW, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_GREEN") == 0) {
        pulseSignal(LED_GREEN, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_GREEN_BRIGHT") == 0) {
        pulseSignal(LED_GREEN_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_BLUE") == 0) {
        pulseSignal(LED_BLUE, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_BLUE_BRIGHT") == 0) {
        pulseSignal(LED_BLUE_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_RED") == 0) {
        pulseSignal(LED_RED, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_RED_BRIGHT") == 0) {
        pulseSignal(LED_RED_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_YELLOW") == 0) {
        pulseSignal(LED_YELLOW, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_YELLOW_BRIGHT") == 0) {
        pulseSignal(LED_YELLOW_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_PURPLE") == 0) {
        pulseSignal(LED_PURPLE, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_PURPLE_BRIGHT") == 0) {
        pulseSignal(LED_PURPLE_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_TEAL") == 0) {
        pulseSignal(LED_TEAL, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_TEAL_BRIGHT") == 0) {
        pulseSignal(LED_TEAL_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_WHITE") == 0) {
        pulseSignal(LED_WHITE, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_WHITE_BRIGHT") == 0) {
        pulseSignal(LED_WHITE_BRIGHT, LED_PIN);
      } else if (strcmp(packetBuffer, "LED_OFF") == 0) {
        pulseSignal(LED_OFF, LED_PIN);
      }
    }
  }
}

void pulseSignal(int targetState, int channel) {
  int maximum;
  int* state;
  int pulseCount;
  if (channel == LED_PIN) {
    maximum = 17;
    state = &ledState;
  } else if (channel == DIFFUSER_PIN) {
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
    delay(75);
    digitalWrite(channel, LOW);
    delay(75);
  }
  *state = targetState;
}
