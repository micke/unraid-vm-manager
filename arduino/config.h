#include "vm.h"

#define HOSTNAME "vm-controller"

#define SERVER_IP IPAddress(10, 0, 0, 10)
#define CONTROLLER_HOST String(SERVER_IP.toString())
#define CONTROLLER_PORT String("3000")
#define CONTROLLER_URI String("http://") + CONTROLLER_HOST + ":" + CONTROLLER_PORT + "/"

#define WIFI_SSID "Wifi name"
#define WIFI_PASSWORD "Wifi password"

const long syncVMInterval = 5000;
const long syncVMStartingInterval = 1000;
const long debounceInterval = 50;
const long startingTimeout = 15000;
const long startingBlink = 1000;
const long selectedBlink = 200;

int shutdownPin = TX;
int destroyPin = RX;

int numberOfVMs = 4;
VM vms[] = {
  {
    { 0x52, 0x54, 0x00, 0x9d, 0x30, 0x39 },
    "a97e8d42-ddc6-a463-54d6-8a2b4918092a",
    D1,
    D0,
    false,
    false,
    0,
    false,
    0,
    false,
    0
  },
  {
    { 0x52, 0x54, 0x00, 0x14, 0x2a, 0xd3 },
    "14190542-886f-d481-cc76-f81967517693",
    D2,
    D5,
    false,
    false,
    0,
    false,
    0,
    false,
    0
  },
  {
    { 0x51, 0x54, 0x00, 0x5F, 0x89, 0x8c },
    "Arch Linux",
    D3,
    D6,
    false,
    false,
    0,
    false,
    0,
    false,
    0
  },
  {
    { 0x51, 0x54, 0x00, 0x5F, 0x89, 0x8c },
    "Windows Throwaway",
    D4,
    D7,
    false,
    false,
    0,
    false,
    0,
    false,
    0
  }
};
