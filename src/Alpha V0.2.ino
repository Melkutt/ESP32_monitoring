/*********
Original created by: Rui Santos, complete project details at https://randomnerdtutorials.com  

Latest rebuilt by Melker H 2024-12-05
https://github.com/Melkutt/ESP32_monitoring/
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

size_t mp_task_heap_size = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) - 32*1024;

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSW";

#define DHTPIN D0     // GPIO18/A0/D0/

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11 (AM2301)
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

#define ADC1 9 // GPIO9/A2/D2
#define ADC2 8 // GPIO8/A3/D3
#define ADC3 7 // GPIO7/A4/D4/SDA
#define PTT  6 // GPIO6/A5/D5/SCL pin

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

String ADC1raw() {

  analogReadResolution(12);
  float v = analogRead(ADC1) * (21.3 / 4095.0); //Resistor divider 12k and 2k2 = 21.5V divied to 3.3V
  if (isnan(v)) {
    Serial.println("Failed to read ADC 1!");
    return "--";
  }
  else {
    Serial.println(v);
    return String(v);
  }
}

String ADC2raw() {

  analogReadResolution(12);
  float v1 = analogRead(ADC2) * (8.25 / 4095.0); //Resistor divider 3k3 and 2k2 = 8.25V divied to 3.3V
  if (isnan(v1)) {
    Serial.println("Failed to read ADC 2!");
    return "--";
  }
  else {
    Serial.println(v1);
    return String(v1);
  }
}

String ADC3raw() {

  analogReadResolution(12);
  float v2 = analogRead(ADC3) * (8.25 / 4095.0); //Resistor divider 3k3 and 2k2 = 8.25V divied to 3.3V
  if (isnan(v2)) {
    Serial.println("Failed to read ADC 3!");
    return "--";
  }
  else {
    Serial.println(v2);
    return String(v2);
  }
}

String PTT_in() {
 
  int p0 = digitalRead(PTT);
  //PTTval = digitaRead(PTT);
  ////int PTTval = PTT;

  if (p0 == 1) {         
   Serial.println("TX"); 
   return String("TX");
  }
  if (p0 == 0) {
   Serial.println("RX"); 
   return String("RX");
  }
  else{
   Serial.println("It's fuckd up!"); 
   return String("ERROR");
  } 
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<a class="github-corner" href="https://github.com/Melkutt/" target="_blank" aria-label="View source on GitHub"><svg width="80" height="80" viewBox="0 0 250 250" style="
      fill: #60745c;
      color: #fff;
      position: absolute;
      top: 0;
      border: 0;
      right: 0;
    " aria-hidden="true">
    <path d="M0,0 L115,115 L130,115 L142,142 L250,250 L250,0 Z"></path>
    <path d="M128.3,109.0 C113.8,99.7 119.0,89.6 119.0,89.6 C122.0,82.7 
	120.5,78.6 120.5,78.6 C119.2,72.0 123.4,76.3 123.4,76.3 C127.3,80.9 
	125.5,87.3 125.5,87.3 C122.9,97.6 130.6,101.9 134.4,103.2" fill="currentColor" 
	style="transform-origin: 130px 106px" class="octo-arm"></path>
    <path d="M115.0,115.0 C114.9,115.1 118.7,116.5 119.8,115.4 L133.7,101.6 
	C136.9,99.2 139.9,98.4 142.2,98.6 C133.8,88.0 127.5,74.4 143.8,58.0 C148.5,53.4 
	154.0,51.2 159.7,51.0 C160.3,49.4 163.2,43.6 171.4,40.1 C171.4,40.1 176.1,42.5 
	178.8,56.2 C183.1,58.6 187.2,61.8 190.9,65.4 C194.5,69.0 197.7,73.2 200.1,77.6 
	C213.8,80.2 216.3,84.9 216.3,84.9 C212.7,93.1 206.9,96.0 205.4,96.6 C205.1,102.4 
	203.0,107.8 198.3,112.5 C181.9,128.9 168.3,122.5 157.7,114.1 C157.9,116.9 156.7,120.9 
	152.7,124.9 L141.0,136.5 C139.8,137.7 141.6,141.9 141.8,141.8 Z" 
	fill="currentColor" class="octo-body"></path></svg></a>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Antenna monitoring device</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>

  <p>
    <i class="fas fa-bolt" style="color:#00add6;"></i> 
    <span class="dht-labels">12V Line</span>
    <span id="voltage">%VOLTAGE%</span>
    <sup class="units">V</sup>
  </p>

  <p>
    <i class="fas fa-bolt" style="color:#00add6;"></i> 
    <span class="dht-labels">5V Line</span>
    <span id="voltage">%VOLTAGE1%</span>
    <sup class="units">V</sup>
  </p>

  <p>
    <i class="fas fa-bolt" style="color:#00add6;"></i> 
    <span class="dht-labels">PA</span>
    <span id="voltage">%PA%</span>
    <sup class="units">V</sup>
  </p>

  <p>
  <i class="fa-solid fa-radio"style="color:#00add6;"></i>
  <span class="dht-labels">PTT</span>
  <span id="voltage">%PTT%</span>
  
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("voltage").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/voltage", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("voltage1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/voltage1", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("PA").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/PA", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("PTT").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ptt", true);
  xhttp.send();
}, 10000 ) ;

</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "VOLTAGE"){
    return ADC1raw();
  }
  else if(var == "VOLTAGE1"){
    return ADC2raw();
  }
  else if(var == "PA"){
    return ADC3raw();
  }
  else if(var == "PTT"){
    return PTT_in();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  pinMode(PTT, INPUT); //INPUT_PULLDOWN, 
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ADC1raw().c_str());
  });

    server.on("/voltage1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ADC2raw().c_str());
  });

    server.on("/ptt", HTTP_GET, [](AsyncWebServerRequest *request){  
    request->send_P(200, "text/plain", PTT_in().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
