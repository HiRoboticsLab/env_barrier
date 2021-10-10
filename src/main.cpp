#include <Arduino.h>
#include <OneButton.h>
#include <painlessMesh.h>
#include <ArduinoJson.h>

#include "config.h"
#include "myServo.h"
#include "screen.h"

Screen screen;
OneButton button(pin_btn, true);
MyServo servo;

Scheduler userScheduler;
painlessMesh mesh;

void checkOnlineDevice();
Task taskCheckOnlineDevice(TASK_SECOND * 1, TASK_FOREVER, &checkOnlineDevice);

String nodeId = "";

/**
 * 刷新token
 */
// void refreshToken() {
//   long token = random(100000, 999999);
//   screen.setToken(String(token));
// }

/**
 * 控制道闸
 */
void ctrlBarrier() {
  int angle = servo.read();
  if (angle == SERVO_ANGLE_MIN) {
    servo.write(SERVO_ANGLE_MAX);
  } else {
    servo.write(SERVO_ANGLE_MIN);
  }
}

/**
 * mesh回调
 */
void meshCallback(uint32_t from, String &msg) {
  String result = msg;
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  DynamicJsonDocument doc(256);
  deserializeJson(doc, result);

  String id = doc["id"];

  if(!id.equals(nodeId)){
    return;
  }

  if(doc["cmd"] == "open"){
    servo.write(SERVO_ANGLE_MIN);
  }
  if(doc["cmd"] == "close"){
    servo.write(SERVO_ANGLE_MAX);
  }
}

/**
 * 检查mesh在线设备数量
 */
void checkOnlineDevice() {
  std::list nodes = mesh.getNodeList();
  int count = nodes.size();
  screen.setState(String(count));
}


void setup() {
  Serial.begin(115200);

  screen.title();
  screen.menu();

  button.attachClick(ctrlBarrier);

  servo.attach(pin_servo, 500, 2500);
  servo.write(SERVO_ANGLE_MIN);

  // 开启wifimesh
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&meshCallback);
  mesh.onChangedConnections(&checkOnlineDevice);
  // userScheduler.addTask(taskCheckOnlineDevice);
  // taskCheckOnlineDevice.enable();

  nodeId = String(mesh.getNodeId());

  screen.setId(nodeId);
}


void loop() {
  button.tick();
  mesh.update();
}