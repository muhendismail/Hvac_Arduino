  # HVAC SYSTEMS With NodeMCU (Air Quality Control)
  
NodeMCU ESP8266 development board was used in the project. Code was written in Arduino IDE.

## Components Used:
     
* NodeMCU ESP8266
* DHT11
* MQ-135
* LEDs
* Fans (5V)
* Battery (5V)

## Instructions

   It is necessary to install a few libraries for the project in Arduino IDE.
*	ESP8266WiFi.h
* BlynkSimpleEsp8266.h
*	DHT.h

## How it Works
 
### NdeMCU ESP8266   

 This is an air quality control (HVAC) system. It can measure the humidity, 
temperature and CO2 value of the air in the room. These values are taken with the 
central control system (NodeMCU ESP8266). The values received with the Wi-Fi
module are sent to the mobile application and can be controlled remotely. There 
are 3 input values and 5 output control devices.

   1. Temperature control (Led)
   2. Cooling control (Led)
   3. Co2 reduction (Fan)
   4. Humidity reduction (Fan with Filter)
   5. Humidity increase (Fan)

 With the data received, the temperature is kept at 21 C. 
    Temperature/coldness is controlled by gradual power (PWM) according to the temperature of the 
environment. The humidity value is kept at a balance of 50%-75%. The amount of 
Co2 is balanced below 1000 ppm with fans. These are automatically controlled.

 ## Blynk App
  
 Additionally, these values can be viewed and controlled from the mobile 
application (Blynk) and the web. If the user wishes, he can manually control the 
temperature/coldness PWM values.
