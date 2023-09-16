
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

AP_NAME = 'Gumbalde_IoT'
AP_PASS = 'stanga719'
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

#ssid = 'Gumbalde_IoT'
#password = 'stanga719'

#station = network.WLAN(network.STA_IF)

#station.active(True)
#station.connect(ssid, password)

#while station.isconnected() == False:
#  pass

#print('Connection successful')
#print(station.ifconfig())

sensor = dht.DHT22(Pin(18))
#sensor = dht.DHT11(Pin(14))

