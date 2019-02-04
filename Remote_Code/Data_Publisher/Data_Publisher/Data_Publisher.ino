#define ROSSERIAL_ARDUINO_TCP_WIFI

#if defined(ARDUINO_ARCH_ESP32)
  #include <analogWrite.h>
#endif

#include <ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
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

// ---------------------------------------------------------------
// setup LCD for debugging
#define I2C_ADDR    0x27 
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
// ---------------------------------------------------------------

// Callback for subscriber, receives a 3 element Float32 array
void sub_cb (const std_msgs::Float32MultiArray& data_rec) {
  lcd.clear();
  lcd.home();
  
  // display each array element on first line of screen
  for (int i=0; i<3; i++) {
    lcd.print (data_rec.data[i]);
    lcd.print(", ");
  } 
}


// ---------------------------------------------------------------
// setup publisher, subscriber, node handle and a Float32 variable
std_msgs::Float32 inc_data; 

ros::Publisher data_pub("ESP_Data", &inc_data);

ros::Subscriber<std_msgs::Float32MultiArray> data_sub("Pos_Data", &sub_cb); 

ros::NodeHandle nh;
// --------------------------------------------------------------

  
void setup() {
  // Start the LCD
  lcd.begin (16,2);

  connectToNetwork();

 
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  // Useful to check it comes on
  lcd.home ();
  lcd.print("Setup");  

  inc_data.data = 0.0;

  // Initalise node, advertise the pub and subscribe the sub
  nh.initNode();
  nh.getHardware()->setConnection(serverIp, serverPort);
  nh.advertise(data_pub);
  nh.subscribe(data_sub);

  lcd.clear();
  lcd.home();
  lcd.print("Setup Complete");  
}

void loop() {
  data_pub.publish(&inc_data);
  inc_data.data++;

  // Should publish the value of inc_data and receive the subbed data through the callback.
  nh.spinOnce();

  // Show what data we're publishing
  lcd.setCursor (0,1);
  lcd.print(inc_data.data);

  delay(500);
}
