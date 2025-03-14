// ESP32 UDP client sending datagrams to a UDP server

#include <WiFi.h>
#include <WiFiUdp.h>

// network SSID (network name). and network password to join
char ssid[] = "ESP32-Access-Point";
char pass[] = "123456789";

// UDP information
//local broadcast address..
IPAddress udpServer(192, 168, 4, 1);
#define UDP_PORT 3000
WiFiUDP udp;

// Sensor specific variables
const int trigPin = 5;
const int echoPin = 9;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
long duration;
float distanceCm;

// The float that is sent over UDP
float floatToSend;

void setup() 
{
  Serial.begin(115200);
  // Wait for serial port to connect
  delay(1000);

  Serial.println();

  WiFi.mode(WIFI_STA);  // Connect to Wifi network.
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  Serial.println("ESP32 WiFi UDP client - Waiting to connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("ESP32 WiFi UDP client - Connected to wifi");

  // Prints wifi info when connected
  Serial.print("\nserver IP address: ");
  Serial.println(udpServer);

  // Beginning sending data
  udp.begin(UDP_PORT);
  Serial.print("server udp port ");
  Serial.println(UDP_PORT);

  // Specific to distance sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

// Update with specific sensor logic
float readSensor()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  // Prints the distance in the Serial Monitor
  Serial.println(distanceCm);

  return distanceCm;
}

void loop() 
{
  // Update the float to send here
  floatToSend = readSensor();

  // Sending the UDP data
  sendUDPData(floatToSend);

  // Debug check that the float it updated.
  Serial.println(floatToSend);

  // How often it should send
  delay(10);
}

void sendUDPData(float floatToSend)
{
  // Send Packet to UDP server
  udp.beginPacket(udpServer.toString().c_str(), UDP_PORT);
  int len = udp.write((const uint8_t *)&floatToSend, sizeof(floatToSend));
  udp.endPacket();
}
