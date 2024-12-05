# ESP32_monitoring_device_of_some_kinde....

<h2> It's now a &#x3B2;-relese </h2> 
And i guess it will probably stay in this phase.... For a looong time....
<br>
<br>
<br>
2024-12-05 I had to switch out the µC to make this work reliable, it's still a ESP32-S3 but Adafuits QY Py only works sometimes and i dont have enough big rubber <a href=https://en.wikipedia.org/wiki/Rubber_duck_debugging> duck <a> to figure it out myself. Everything works, but the page doesn't auto reload the three last value, only when i do a manual reload
<br>
<br>
2023-11-02 Updated with some new features.

Monitoring voltage, PTT (ham radio stuff), temperature and humidity and present it on a web server. 
This is built with a <a href="https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html?srsltid=AfmBOopuyMmE0nzHtJzi4ZkzratYAda60tjaC378q5dgVO9NSyFDrdLU">Seeedstudio XIAO ESP32-S3</a> board and a DHT22 sensor, but this code will almost certainly work in many others MCU's.
It's writen in Arduino IDE.
<br>
<br>
<br>
![alt text](https://raw.githubusercontent.com/Melkutt/ESP32_monitoring/main/src/New_Git_Pic.bmp)
<br>
<br>

It's a tweekt version on <a href="https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-web-server-arduino-ide/">Rui Santos</a> tutorial on ESP32 web server
with DHT11 sensor.
<br>
I haved to add <code>size_t mp_task_heap_size = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) - 32*1024</code> to make it work with the S2 chip.


