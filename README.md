# ewinch_remote_controller
 transmitter and receiver code for remote controlling a paragliding winch
 Based on LILYGO® TTGO ESP32-Paxcounter LoRa32 V2.1 1.6 Version 915MHZ LoRa ESP-32 OLED
 (http://www.lilygo.cn/prod_view.aspx?TypeId=50060&Id=1271&FId=t3:50060:3) 

Note: The 915MHz Version can transmit/receive in 868MHz and 915MHz, the desired frequency is defined in the code (transmitter.ino, monitor.ino & receiver.ino)
 
 see 
  - https://www.youtube.com/watch?v=5IkagHkxbxY
  - https://www.youtube.com/shorts/iNt1cCAZv0I
  

 receiver uses PPM (Pulse Position Modulation) for driving the winch and (optional) UART to read additional information (line length, battery %, dutycycle)
 VESC UART communication depends on https://github.com/SolidGeek/VescUart/
 
## To use Arduino IDE with the Lilygo TTGO ESP32 Paxcounter LoRa32 / (and Lilygo T-Display S3)
- In Arduino IDE open File > Preferences
- in additional boards manager URLS field copy: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
- click OK
- go to Go to Tools > Board > Boards Manager
- In Boards Manager Search for ESP32 and press install button for the “ESP32 by Espressif Systems“
- Go To Tools > Board > ESP32 and select the TTGO LoRa32-OLED Board

## Install the following Arduino libraries:
- [18650CL](https://github.com/pangodream/18650CL)
- [Button2](https://github.com/LennartHennigs/Button2)
- [VescUart](https://github.com/SolidGeek/VescUart)
- [OLED-SSD1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)
- [Servo](https://www.arduino.cc/reference/en/libraries/esp32servo/)
- [TFT_eSPI](https://www.arduino.cc/reference/en/libraries/tft_espi/) / If you want to use the Lilygo T-Display S3 as a monitor

## Moving to Visual Studio Code and PlatformIO

I am moving to Visual Studio Code and PlatformIO to introduce Version control with Github and Git.
The main difference is that with VS Code and PlatformIO the main program files don't have the *.ino file extension
but *.cpp. If you want to compile and upload the code to the ESP32 boards just download all files and rename
them from *.cpp to *.ino.

## PIN Setup Receiver:
IO 13 (PWM_PIN_OUT) // connect to PPM Port "Servo" on Vesc

IO 14 (VESC_RX)   //connect to COMM Port "TX" on Vesc

IO 2 (VESC_TX)   //connect to COMM Port "RX" on Vesc

IO 15 (Servo Signal) // connect red wire to 5V, black or brown wire to GND and yellow or white cable to Pin 15. Servo is in neutral state by default

IO 12 (Relay Signal) // connect red wire to 5V, black wire to GND and white cable (signal) to Pin 12. Wire your Warning Light and VESC Cooling Fan through the relay module. Relay is off by default, will be turned on once the Remote/Transmitter is turned on. 


## PIN Setup Transmitter:
IO 15 (BUTTON_UP) //together with GND connect with push button for UP Command

IO 12 (BUTTON_DOWN ) //together with GND connect with a push button for STOP/BRAKE Command

IO 14 (BUTTON_THREE ) // additional /optional Button for Relay and Servo Control. Click once to deactivate Relay (Fan and Warning light), Click again to turn on again. Long Click to trigger the Emergency Line Cutter (Servo). Doubleclick to move Servo to Neutral Position again. By default, the Servo is in neutral position, and Relay is off. Turning on the Remote will turn on the Relay automatically.

## Monitor with LilyGo T-Display
I am adding a Monitor for the paraglider cockpit to better view winch values in your eye-sight,
based on a LilyGo T-Display S-3 board with integrated color TFT Display.
This monitor also serves to control some (additional) functions on the transmitter,
such as deploying an emergency line cutter, turning on and off a relay (that controls a cooling fan and a warning light)
and setting the maxPull Value. The Monitor communicates with the Transmitter over Wifi via ESP-NOW Protocol.

## Pin Setup Monitor
IO 16 Connect a Potentiometer to PIN 16 (dont forget GND and +3,3V)

IO 0 Button A - Relay and cooling fan are on by default. Press Button A once to turn off, and on again

IO 14 Button B - Button B should be a large and easily reachable button on your cockpit. This triggers the emergency line cutter. A double press resets the Servo / Line Cutter back to neutral/ready position

IO 12 Button C - Press Button C once to enter "settings mode". You can now turn the potentiometer to set the Max Pull Value to the transmitter (set Value according to Pilot's Take-Off weight) Press Button once more to exit settings mode and confirm the selected value. It is then sent
to the transmitter over Wifi via ESP-Now Protocol.

## Line auto stop in VESC
Line auto stop can be implemented within VESC with vesc_ppm_auto_stop.patch

For this to work properly, either connect a Potentiometer to ADC2 and GND to manually control the winch. E.g. To wind up the last meters of the line when finishing. Or to manually set a tension when used as a rewind winch. Note that the potentiometer only reduces tension/speed of the motor when it is running one of the pull programs as controlled via the transmitter!

If you do not install a Potentionmeter, connect ADC2 to GND.

## Remote Control of cooling fan and warning light (DHV Regulations)
The transmitter and receiver code now supports a Relay that is automatically turned on when the
remote is switched on. The relay can control the cooling fan of the VESC and a warning light (as required by DHV regulations)
A a single click on a third button on the remote turns the Relay (Fan and warning light) off - usefull when you launched yourself
and want to fly away and leave the winch behind.

## Remote Control of an Emergency Line Cutter (DHV Regulations)
The transmitter and receiver code now supports the connection of a Servo (on PIN 15 on Receiver).
The servo rotates 90 degrees when the third button on the transmitter is "long pressed" (one full second).
This servo should be attached to an emergency line cutter, that cuts the dyneema line in an emergency.
Note: Activating the line cutter also triggers the full brake (-20kg)
TODO - Design and build a lightweight and efficient line cutter! Bernd O., this one is on you :-)

## Work in Progress
I am currently working on adding a Lilygo T-Display S3 ( https://www.lilygo.cc/products/t-display-s3 )
to use as
- a monitor on your paraglider cockpit to have winch values in eyesight
- a control unit for the emergency line cutter and fan / warning light
- a control unit to change the settings for "maxPull" with the help of a potentiometer
Work in Progress as of march '24

## Default Config for VESC
Default VESC app config is vesc_app_config.xml
Default Motor config is vesc_motor_config_12kw_260_V4.xml or vesc_motor_config_12kw_273.xml
**PLEASE NOTE:** don't just take the standard motor config and upload to your VESC. Take it as an example only.
Make sure to run the **"Setup Motor FOC"** wizard for the VESC tool to properly detect internal resistances and other values.

## usage:
- A) prepare:
  1 - turn the VESC and receiver on
  2 - pull the line out to the desired length (the VESC measures the line length that is being unwound, needed for the autostop to work)
  3 - go through your pre-flight preparations and clip in
- B) launch:
  1 - switch to defaultPull (7kg pull value) and prePull (to tighten the line (~13kg pull value) to assist you to launch the glider
  2 - go to takeOffPull (~40kg pull value) to assist you with launching the glider and gently getting into the air with a slight pull towards a safety margin of 15-30m height
  4 - click "Up" Button to increase Pull
- C) Step Towing:
  1 - switch to defaultPull (7kg pull value) before you turn away from winch to fly back to launch site
  2 - go to prePull (~15kg pull value) during turn towards next step (towards winch) to avoid line sag
  3 - after successful turn, go to fullPull again
 
- C) Release
  Go To defaultPull (7kg pull value) before you release
  The winch will autorewind the line IF AutoStop feature is enabled (modified VESC Firmware is required, see vesc/vesc_ppm_auto_stop.patch)
 
- D) Neutral
  You can get to neutral state only if you are in Brake Mode (-7kg), Double Press the ButtonDown to activate it.
 
 ## UNCLEAR
Ask Robert:
- instruction for PPM Settings says "static int myMaxPull = 85;  // 0 - 127 [kg], must be scaled with VESC ppm settings" how to set or calibrate this ?
