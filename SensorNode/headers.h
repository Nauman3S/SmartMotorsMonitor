
#include <painlessMesh.h>
// #define   MESH_PORT       5555
// #define   MESH_SSID       MESH_SERVICE
#include "SoftwareStack.h"
#ifndef BUILTIN_LED
#define BUILTIN_LED 2 // backward compatibility
#endif
#define BLINK_PERIOD 3000  // milliseconds until cycle repeat
#define BLINK_DURATION 100 // milliseconds LED is on for

// #define MESH_SERVICE "5fafc201-1fb5-459e-8fcc-c5c9c331914d"
#define MESH_PORT 5555
#define MESH_SSID "meshService"
#define MESH_PASSWORD "somethingSneaky"

#include "statusLED.h"
#include "neo_timer.h"

#define GET_CHIPID() ((uint16_t)(ESP.getEfuseMac() >> 32))

unsigned long lastPub = 0;
unsigned int updateInterval = 1;

SoftwareStack ss; // SS instance

String mac = (WiFi.macAddress());
char __mac[sizeof(mac)];