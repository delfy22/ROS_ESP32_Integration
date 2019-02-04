//#define ROSSERIAL_ARDUINO_TCP_WIFI // To use Wifi comms, use this definition
#define ESP32_USE_USB // To use USB comms, use this definition

#include <HardwareSerial.h>
#include <ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include <WiFi.h>

// ------------------------------------------------------------
// Wifi stuff
const char* ssid = "test";
const char* password =  "abcdefg1";
IPAddress serverIp(10,42,0,1);      // Set the rosserial socket ROSCORE SERVER IP address
const uint16_t serverPort = 11411; // Set the rosserial socket server port

void connectToNetwork() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

}
// ------------------------------------------------------------

// Callback for subscriber, receives a 3 element Float32 array
void sub_cb (const std_msgs::Float32MultiArray& data_rec) {
  
}


// ---------------------------------------------------------------
// setup publisher, subscriber, node handle and a Float32 variable
std_msgs::Float32 inc_data; 

ros::Publisher data_pub("ESP_Data", &inc_data);

ros::Subscriber<std_msgs::Float32MultiArray> data_sub("Pos_Data", &sub_cb); 

ros::NodeHandle nh;
// --------------------------------------------------------------

  
void setup() {

//  connectToNetwork(); 

  inc_data.data = 0.0;

  // Initalise node, advertise the pub and subscribe the sub
  nh.initNode();
//  nh.getHardware()->setConnection(serverIp, serverPort); // Uncomment for Wifi comms
  nh.advertise(data_pub);
  nh.subscribe(data_sub);
}

void loop() {
  data_pub.publish(&inc_data);
  inc_data.data++;

  // Should publish the value of inc_data and receive the subbed data through the callback.
  nh.spinOnce();

  // Show what data we're publishing

  delay(500);
}
