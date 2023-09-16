
try:
  import usocket as socket
except:
  import socket

from network import WLAN, STA_IF
from machine import Pin
import dht

import esp
esp.osdebug(None)

import gc
gc.collect()

from time import ticks_ms

AP_NAME = 'SSID'
AP_PASS = 'PASSW'
WIFI_TIMEOUT = 60

print('Connecting...')
wlan = WLAN(STA_IF)
wlan.active(True)
wlan.connect(AP_NAME, AP_PASS)
start_time = ticks_ms()
while not wlan.isconnected():
    if (ticks_ms() - start_time > WIFI_TIMEOUT * 1000):
        break
if (wlan.isconnected()):
    print('Connected')
else:
    print('Timeout!')

sensor = dht.DHT22(Pin(18))

