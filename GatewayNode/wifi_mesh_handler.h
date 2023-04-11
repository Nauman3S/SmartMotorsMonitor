#include <painlessMesh.h>
#define MESH_PREFIX "meshService"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555
#define HOSTNAME "MQTT_Bridge"
// Prototypes
void receivedCallback(const uint32_t &from, const String &msg);
void mqttCallback(char *topic, byte *payload, unsigned int length);

IPAddress getlocalIP();
IPAddress myIP(0, 0, 0, 0);
Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
WiFiClient wifiClient;
String TrackerID = "";
int NetworkStatus = 0;

bool onFlag = false;

void APIPostReq();

Task taskAPIPostReq(TASK_SECOND * 10, TASK_FOREVER, &APIPostReq); // start with a one second interval
// HTTPReq httpClient;

void setupMeshGateway()
{
    Serial.print("GatewayTrackerID: ");
    Serial.println(ss.getMACAddress());
    mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION); // set before init() so that you can see startup messages

    // Channel set to 6. Make sure to use the same channel for your mesh and for you other
    // network (STATION_SSID)
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6);
    mesh.onReceive(&receivedCallback);

    mesh.stationManual(STATION_SSID, STATION_PASSWORD);
    mesh.setHostname(HOSTNAME);
    mesh.setRoot(true);
    // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
    mesh.setContainsRoot(true);
    userScheduler.addTask(taskAPIPostReq);
    taskAPIPostReq.enable();
}

void meshGatewayLoop()
{
    mesh.update();
    digitalWrite(BUILTIN_LED, !onFlag);

    if (myIP != getlocalIP())
    {
        myIP = getlocalIP();
        Serial.println("My IP is " + myIP.toString());

        if (mqttClient.connected())
        {
            NetworkStatus = 1;
            // mqttClient.publish("BLEMesh/from/gateway", "Ready!");
            // mqttClient.subscribe("BLEMesh/to/#");
            // httpClient.listenResponse();
        }
    }
}
void APIPostReq()
{
    Serial.println("Making Post Request");
    String topic = "BLEMesh/from/all";

    if (NetworkStatus)
    {
        // Serial.println("Status: Connected", "Nodes:" + String(TL.getTrackerPointer()), getTimeStamp());
        Serial.println("Status: Connected");
    }
    else
    {
        // Serial.println("Status: Wait", "Nodes:" + String(TL.getTrackerPointer()), getTimeStamp());
        Serial.println("Status: Wait");
    }

    taskAPIPostReq.setInterval(TASK_SECOND * 10);
}
void receivedCallback(const uint32_t &from, const String &msg)
{
    Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
    String topic = "BLEMesh/from/" + String(from);
    String temp = ss.StringSeparator(msg, ' ', 2);
    String NodeMAC = ss.StringSeparator(temp, '_', 0);
    Serial.print("HeartBeat of Node : ");
    Serial.println(NodeMAC);
    mqttPublish(topic, temp);
    
    onFlag = !onFlag; // changeLEDState
}

// void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
// {
//     char *cleanPayload = (char *)malloc(length + 1);
//     memcpy(cleanPayload, payload, length);
//     cleanPayload[length] = '\0';
//     String msg = String(cleanPayload);
//     free(cleanPayload);

//     String targetStr = String(topic).substring(16);
//     httpClient.updateResponse(topic, msg);
//     if (targetStr == "gateway")
//     {
//         if (msg == "getNodes")
//         {
//             auto nodes = mesh.getNodeList(true);
//             String str;
//             for (auto &&id : nodes)
//                 str += String(id) + String(" ");
//             // mqttClient.publish("BLEMesh/from/gateway", str.c_str());
//         }
//     }
//     else if (targetStr == "broadcast")
//     {
//         mesh.sendBroadcast(msg);
//     }
//     else
//     {
//         uint32_t target = strtoul(targetStr.c_str(), NULL, 10);
//         if (mesh.isConnected(target))
//         {
//             mesh.sendSingle(target, msg);
//         }
//         else
//         {
//             // mqttClient.publish("BLEMesh/from/gateway", "Client not connected!");
//         }
//     }
// }

IPAddress getlocalIP()
{
    return IPAddress(mesh.getStationIP());
}