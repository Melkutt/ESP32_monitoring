# ESP32_monitoring

<h2> Still a alpha relese </h2>

2023-11-02 Updated with some new features.

Monitoring voltage, temperature and humidity and present it on a web server. 
This is built with a Adafruit ESP32 QT board and a DHT11 sensor. 
It's writen in Arduino IDE.
<br>
<br>
<br>
![alt text](https://raw.githubusercontent.com/Melkutt/ESP32_monitoring/main/src/New_Git_Pic.bmp)
<br>
<br>

It's based on <a href="https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-web-server-arduino-ide/">Rui Santos</a> tutorial on ESP32 web server
with DHT11 sensor.
<br>
I haved to add <code>size_t mp_task_heap_size = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) - 32*1024</code> to make it work with the S2 chip.

Almost <i class="fa-regular fa-bug"></i> free =)


More info is coming!
