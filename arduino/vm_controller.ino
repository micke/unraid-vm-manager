#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUDP.h>
#include <WakeOnLan.h>
#include <RemoteDebug.h>
#include "config.h"
#define MY_RF69_SPI_CS D8
#define ARDUINO_ARCH_ESP8266

RemoteDebug Debug;

unsigned long lastVMSync = 0;

boolean shutdownPressed = false;
boolean destroyPressed = false;

boolean wifiConnected = false;

WiFiUDP UDP;

boolean connectWifi();
void sendWOL();
void handleActivityLight(VM vm);

void setup() {
  Serial.begin(115200);
  // Make RX and TX usable as outputs
  pinMode(RX, FUNCTION_3);
  pinMode(TX, FUNCTION_3);
  pinMode(D7, FUNCTION_3);
  pinMode(D8, FUNCTION_3);
  pinMode(shutdownPin, INPUT_PULLUP);
  pinMode(destroyPin, INPUT_PULLUP);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  UDP.begin(9); //start UDP client, not sure if really necessary.

  Debug.begin(HOSTNAME);
  Debug.setSerialEnabled(true);

  for (int thisVm = 0; thisVm < numberOfVMs; thisVm++) {
    VM &vm = vms[thisVm];
    Debug.printf("Setting %s", vm.uid);
    pinMode(vm.pin, INPUT_PULLUP);
    pinMode(vm.activityPin, OUTPUT);
    Debug.printf("Done %s", vm.uid);
  }
  Debug.println("Done setting pins");
}

void loop()
{
  if (wifiConnected) {
    handleShutdown();
    handleDestroy();

    for (int thisVm = 0; thisVm < numberOfVMs; thisVm++) {
      VM &vm = vms[thisVm];
      int pin = digitalRead(vm.pin);

      if (pin == LOW) {
        vm.lastPressed = millis();

        if (vm.pressed) {
          continue;
        }

        vm.pressed = true;

        if (vm.selected) {
          vm.selected = false;
          Debug.printf("Deselected %s\n", vm.uid);
        } else if (vm.running || vm.starting) {
          vm.selected = true;
          vm.selectedAt = millis();
          Debug.printf("Selected %s\n", vm.uid);
        } else {
          vm.starting = true;
          vm.startedAt = millis();

          Debug.printf("Sending WOL for %s to %s\n", vm.uid, vm.mac);
          WakeOnLan::sendWOL(SERVER_IP, UDP, vm.mac, sizeof vm.mac);
        }
      } else {
        // if enough millis have elapsed
        if (millis() - vm.lastPressed >= debounceInterval) {
          vm.pressed = false;
        }
      }

      handleActivityLight(vm);
    }

    syncVMStatus();
  }

  Debug.handle();
}

void handleShutdown() {
  if (digitalRead(shutdownPin) == LOW){
    if (shutdownPressed) {
      return;
    }
    shutdownPressed = true;
    for (int thisVm = 0; thisVm < numberOfVMs; thisVm++) {
      VM &vm = vms[thisVm];
      if (vm.selected) {
        Debug.printf("Shutting down %s\n", vm.uid);
        HTTPClient http;
        http.begin(CONTROLLER_URI + vm.uid + "/shutdown");
        int httpCode = http.GET();
        vm.selected = false;
        vm.starting = false;
      }
    }
  } else {
    shutdownPressed = false;
  }
}

void handleDestroy() {
  if (digitalRead(destroyPin) == LOW){
    if (destroyPressed) {
      return;
    }
    destroyPressed = true;
    for (int thisVm = 0; thisVm < numberOfVMs; thisVm++) {
      VM &vm = vms[thisVm];
      if (vm.selected) {
        Debug.printf("Destroying %s\n", vm.uid);
        HTTPClient http;
        http.begin(CONTROLLER_URI + vm.uid + "/destroy");
        int httpCode = http.GET();
        vm.selected = false;
        vm.starting = false;
      }
    }
  } else {
    destroyPressed = false;
  }
}

void handleActivityLight(VM vm) {
  if (vm.selected) {
    if((millis() % (selectedBlink * 2)) > selectedBlink){
      digitalWrite(vm.activityPin, HIGH);
    } else {
      digitalWrite(vm.activityPin, LOW);
    }
  } else if (vm.running) {
    digitalWrite(vm.activityPin, HIGH);
  } else if (vm.starting) {
    if((millis() % (startingBlink * 2)) > startingBlink){
      digitalWrite(vm.activityPin, HIGH);
    } else {
      digitalWrite(vm.activityPin, LOW);
    }
  } else {
    digitalWrite(vm.activityPin, LOW);
  }
}

void syncVMStatus() {
  if (millis() - lastVMSync >= effectiveSyncVMInterval()) {
    HTTPClient http;
    http.begin(CONTROLLER_URI + "active");
    int httpCode = http.GET();
    String payload = http.getString();

    for (int thisVm = 0; thisVm < numberOfVMs; thisVm++) {
      VM &vm = vms[thisVm];
      if (payload.indexOf(vm.uid)>=0) {
        vm.running = true;
        vm.starting = false;
      } else {
        vm.running = false;
      }
    }

    lastVMSync = millis();
  }
}

boolean isAnyVMStarting() {
  boolean starting = false;
  for (int thisVm = 0; thisVm < numberOfVMs; thisVm++) {
      VM vm = vms[thisVm];
      if (vm.starting) {
        starting = true;
        break;
      }
  }
  return starting;
}

long effectiveSyncVMInterval() {
  return isAnyVMStarting() ? syncVMStartingInterval : syncVMInterval;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.hostname(HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Debug.println("");
  Debug.println("Connecting to WiFi");

  // Wait for connection
  Debug.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    Debug.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Debug.println("");
    Debug.print("Connected to ");
    Debug.println(WIFI_SSID);
    Debug.print("IP address: ");
    Debug.println(WiFi.localIP());
  }
  else {
    Debug.println("");
    Debug.println("Connection failed.");
  }

  return state;
}
