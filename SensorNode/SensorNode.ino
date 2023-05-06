
// Sensor Node
#include "consts.h"
#include "headers.h"
#include "digital_input_handler.h"
#include "ads_handler.h"
#include "relay_handler.h"
#include "mpu9250_handler.h"
#include "esp32InternalTime.h"

// Prototypes
void sendMessage();
void receivedCallback(uint32_t from, String &msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

void sendMessage();                                                // Prototype
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage); // start with a one second interval

// Task to blink the number of nodes
Task blinkNoNodes;
bool onFlag = false;
String TrackerID = "";

String latestInputValues = "";
String latestIMUValues = "";
void setup()
{
    Serial.begin(115200);
    TrackerID = ss.getMACAddress();
    Serial.print("TrackerID: ");
    Serial.println(TrackerID);
    pinMode(BUILTIN_LED, OUTPUT);
    setupDigitalInputs();
    setupMPU9250();
    setupADS1115();
    setupRelays();
    setRelay(RELAY1, 0);
    setRelay(RELAY2, 0);
    setRelay(RELAY3, 0);
    setRelay(RELAY4, 0);

    mesh.setDebugMsgTypes(ERROR | DEBUG); // set before init() so that you can see error messages

    mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onNodeDelayReceived(&delayReceivedCallback);
    mesh.setContainsRoot(true);

    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();

    blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []()
                     {
      // If on, switch off, else switch on
      if (onFlag)
        onFlag = false;
      else
        onFlag = true;
      blinkNoNodes.delay(BLINK_DURATION);

      if (blinkNoNodes.isLastIteration()) {
        // Finished blinking. Reset task for next run 
        // blink number of nodes (including this node) times
        blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
        // Calculate delay based on current mesh time and BLINK_PERIOD
        // This results in blinks between nodes being synced
        blinkNoNodes.enableDelayed(BLINK_PERIOD - 
            (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
      } });
    userScheduler.addTask(blinkNoNodes);
    blinkNoNodes.enable();

    randomSeed(analogRead(A0));
}

void loop()
{
    mesh.update();
    digitalWrite(BUILTIN_LED, !onFlag);
}

void sendMessage()
{
    latestInputValues =
        getDigitalInputState(D_IN1) + String(";") + getDigitalInputState(D_IN2) + String(";") + getADS1015Values(A_IN1) + String(";") +
        getADS1015Values(A_IN2);
    latestIMUValues = getMPU9250Values();
    String msg = "FRMID: " + TrackerID + String("__"); // FRMID=From ID
    msg += mesh.getNodeId();
    // msg += " myFreeMemory: " + String(ESP.getFreeHeap());
    msg += ";" + latestInputValues;
    msg += ";" + latestIMUValues;
    mesh.sendBroadcast(msg);

    if (calc_delay)
    {
        SimpleList<uint32_t>::iterator node = nodes.begin();
        while (node != nodes.end())
        {
            mesh.startDelayMeas(*node);
            node++;
        }
        calc_delay = false;
    }

    Serial.printf("Sending message: %s\n", msg.c_str());

    taskSendMessage.setInterval(random(TASK_SECOND * 4, TASK_SECOND * 5)); // between 4 and 5 seconds
}

void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{
    // Reset blink task
    onFlag = false;
    blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
    blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
    // Reset blink task
    onFlag = false;
    blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
    blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

    nodes = mesh.getNodeList();

    Serial.printf("Num nodes: %d\n", nodes.size());
    Serial.printf("Connection list:");

    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end())
    {
        Serial.printf(" %u", *node);
        node++;
    }
    Serial.println();
    calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay)
{
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}