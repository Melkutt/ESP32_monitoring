# ESP32_monitoring

<h2> My first alpha relese! </h2>

Monitoring voltage, temperature and humidity and present it on a web server. 
This is built with a Adafruit ESP32 QT board and a DHT11 sensor. 
It's writen in Arduino IDE.

It's based on <a href="https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-web-server-arduino-ide/">Rui Santos</a> tutorial on ESP32 web server
with DHT11 sensor.
<br>
I haved to add <code>size_t mp_task_heap_size = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) - 32*1024</code> to make it work with the S2 chip.


More info is coming!
