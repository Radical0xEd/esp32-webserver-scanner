# esp32-webserver-scanner
   
   Ive broken several ESP's over the years from soldering mistakes to freak accidents.  These are some ways i found to still use them, also a good run for begginers.

Library Requirements:

Ensure you have the latest AsyncTCP and ESPAsyncWebServer libraries installed in your Arduino IDE or PlatformIO environment.

   There is another project on this repo that is the same thing, but a little more advanced as the server is on the webs side and esp connects to that.  This First method I like because it can run on any esp even without use of UART-TX & UART-RX pins. (However you can program almost any pin to run the serial info) All You need to do is flash to esp.

   ESP Based Server:
   .Wi-Fi Credentials:

Replace your_wifi_ssid and your_wifi_password with your network credentials for best results, Just change************* By defualt just plug a USB-C into esp and then go to "http://192.168.1.184" as this is defualt IP. Use this web interface to run various script on esp and the site. alternitavly you can use vcc and gnd pins and change IP under the Static IP. 
  
  HTTP Endpoints: will update and should correspond with your esp's leds.
/: Confirms the server is running.
/send: Sends a test message over UART.
/read: Reads available data from UART and displays it.
if UI is being slow or not working you can try to manual interface with the chip though these endpoints. For example if the scan button is not initiating scan, you can enter "http://192.168.1.184/scan" it may say to resfresh in this case however just type "http://192.168.1.184/read"





    I've written this in Ardrino so it is a .ino However i will add the C++ aswell. Easiest way to run is as is but i am always oipen to new ideas.
