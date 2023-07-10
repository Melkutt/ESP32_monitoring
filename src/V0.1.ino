/*********

  Rui Santos web server --> https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
  emilv and his ArduSorting lib --> https://github.com/emilv/ArduinoSort
  RobTillaart's DTHNew lib --> https://github.com/RobTillaart/DHTNew

Adafruit QT Py ESP32S2 pin

You can't use ADC2 with Wi-Fi!
Diagram

A0   -> GPIO18 -> ADC2 CH7 -> DAC_2
A1   -> GPIO17 -> ADC2 CH6 -> DAC_1
A2   -> GPIO9  -> ADC1 CH8
A3   -> GPIO8  -> ADC1 CH7
SDA  -> GPIO7  -> ADC1 CH6
SCL  -> GPIO6  -> ADC1 CH5
TX   -> GPIO5  -> ADC1 CH4
MOSI -> GPIO35 -> SPI
MISO -> GPIO37 -> SPI
SCK  -> GPIO36 -> SPI
RX   -> GPIO16 -> ADC2 CH5 -> RTC
SD1A -> GPIO41        (on QT connecor)
SCLA -> GPIO40        (on QT connecor)
D0   -> GPIO0  -> RTC (on pushbutton)

DHT 1W sensor pin
1 VCC
2 DATA, remeber to add a pull-up resistor
3 NC
4 GND

********/

#include <WiFi.h>
#include <ArduinoSort.h>
#include <dhtnew.h>

/*
//NEOPIXEL is just for debugging for the moment!

#include <Adafruit_NeoPixel.h>

#define NUMPIXELS        1

Adafruit_NeoPixel pixels(1, 38, NEO_GRB + NEO_KHZ800);

*/


DHTNEW dht (A1);

#define ADC1 A2
#define ADC2 A3
#define ADC3 SDA

float tempArray[5];
float humiArray[5];

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PWD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const float output26 = 17;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {

  pinMode(ADC1, INPUT);
  pinMode(ADC2, INPUT);
  pinMode(ADC3, INPUT);

  Serial.begin(115200);

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
 

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

/*

  #if defined(NEOPIXEL_POWER)
  // If this board has a power control pin, we must set it to output and high
  // in order to enable the NeoPixels. We put this in an #if defined so it can
  // be reused for other boards without compilation errors
  pinMode(38, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(40); // not so bright

  */
}

void loop(){

  delay(500);
  dht.read();
  Serial.print(dht.getHumidity(), 1);
  Serial.print(",\t");
  Serial.println(dht.getTemperature(), 1);
  delay(500); 

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
     updateSensorArray(2);
     medianValue(2);

      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              /*****
               //Debugging stuff

               pixels.fill(0xFF0000);
               pixels.show();
               delay(500); // wait half a second

               // turn off
               pixels.fill(0x000000);
               pixels.show();
               delay(500); // wait half a second

               *****/
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
              
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 antenna server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
}

void updateSensorArray(float)
{
  int i = 0;
  for (byte i = 0; i < 5;){
    delay(2000);
    dht.read();
    tempArray[i] = dht.getTemperature();
    humiArray[i] = dht.getHumidity();
    ++i;
  } 
}


/*
 * This part sorts the array from low to high so updateSensor just report the median value.
 * Pretty smart and very easy way to sort out any error from the sensor!
 */

 void medianValue(float){

    sortArray(tempArray, 5);
    sortArray(humiArray, 5);
 
 }

 /*
  * This part zeros both arrays so the can be filld again with out overflows
  */
  
 void zero(){

     Serial.println("Zeroing arrays!");
      memset(tempArray,0,sizeof(tempArray));
      memset(humiArray,0,sizeof(humiArray));
 }

 /*
  * Update sensor value to Homey
  */

  void updateSensor(float) {
    
   float temperature = tempArray[2];
   float humidity = humiArray[2];

  
   Serial.print((float)temperature); Serial.println(" *C, ");
   Serial.print((float)humidity); Serial.println(" H");

}
