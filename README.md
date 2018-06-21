
RaWAN Tester

A network tester for LoRaWAN networks.

## Setup
----
This is designed to work with the following hardware:
  - Adafruit Feather M0 (ATSAMD21)
  - Microchip RN2483
  - GPS (e.g. neo8m)
  - Analog joystick.
  - 1.8TFT ST7735 Display

#### Required Libraries!

  - [NeoGPS]
  - [RN2483] (Forked from arduino-device-lib by TTN)
  - [FlashStorage]
  - [Arduino Serial Command]
  - [Adafruit GFX]
  - [Adafruit ST7735]
 
#### Installation and Configuration

Hardware specific changes can be made to the "hardware.h" file. Upload this sketch to your Feather M0 using the Arduino IDE. Your IDE must have the [Adafruit SAMD] boards support added for this to work.  

## Instructions for Use
----
On boot you will be presented with the "Menu" screen containing the following functions:
  - Survey
  - Sweep  
  - Settings
  - USB
 
You can move between menu items by pushing up/down on the joystick. Click the joystick to activate the currently selected item. "Survey" and "Sweep" will not be available if the required LoRaWAN network keys for the currently selected join mode are not present; see [usb](#usb) and [settings](#settings)
  
#### Survey
"Survey" is a mode that utlises the current device settings to perform periodic or manual transmissions. If the device has been requested to connect using "Over The Air Activation", and is currently not connected to a network, upon entering this mode the device will first attempt to join a network using the internal device keys. "Survey" mode is great for performing "war driving" tests.

#### Sweep
"Sweep" is a mode that causes the device to send a transmission at each spreadfactor (SF7 - SF12, i.e. DR5 - D0); this collection of transmissions is referred to as a 'test'. This mode is useful for testing connectivity at a specific location (e.g. before deploying a sensor). If the device has been requested to connect using "Over The Air Activation", and is currently not connected to a network, upon entering this mode the device will first attempt to join a network using the internal device keys. Because of duty cycle limitations, the device may have to wait for a free-channel before it can attempt a transmission at a specific spreadfactor. During this time the device will state "No Free Chan. Cancel Test?", clicking the joystick will cancel the current and subsequent transmissions left in the test.

At the end of the test the results will be displayed on the screen. Transmissions that were successful and therefore recieved an acknowledgement from a network server will be presented as black text on a white background. Transmissions that were not successful, because they did not recieve an acknowledgement, will be presented as white text on a black background.  

From this menu you can either push left on the joystick to intiate a new test or push right to return to the main menu.

#### Settings
"Settings" lets you configure the runtime settings of the device. Upon entering this menu option you will be presented with the following configurable settings:
  - Join
  - SF  
  - TX IVL
  - Exit
 
Pushing right or left on the joystick of a currently selected setting will alter that setting. Pushing up or down on the joystick allows you to move between different settings. 

##### Join 
This allows you to change the current join mode of the device. This can either be OTAA (Over the Air Activation) or ABP (Activation by Personalisation). OTAA means that the device will perform a join-procedure with the network during which security keys and a dynamic DevAddr is assigned. Because of this, OTAA requires the presence of a network. ABP hardcodes the DevAddr and security keys in the device. This allows the device to "skip" the join procedure. 

Choosing between OTAA is your choice and if you have provisioned your device with both sets of keys then you can move between them at will. If you are starting in a place with known network connectivity, OTAA may be an easier, preferred option. If you are starting from a place without network connectivity, but may move into range during a survey or sweep, then you may prefer to use ABP. If you do use ABP, you will likely need to inform your network server to have "relaxed frame counts", as power cycles will likely loose this information and therefore prevent further sucessful transmissions.

**The "join-procedure" is only initiated when you enter "Survey" or "Sweep", so you can configure this setting at anytime.**

##### SF
"SF" (Spread Factor) allows you to adjust the spread factor used for joins and transmissions -- obviously, "sweep" mode ignores this setting! 

Selectable spreadfactors range from 7 to 12 (data rates 5 to 0). This [spreadfactor video] explains spreadfactor and its influence.

License
----

GNU Ver 3.

[NeoGPS]: <https://github.com/SlashDevin/NeoGPS>
[RN2483]: https://github.com/danielknox/arduino-device-lib
[FlashStorage]: https://github.com/cmaglie/FlashStorage
[Arduino Serial Command]: https://github.com/kroimon/Arduino-SerialCommand
[Adafruit GFX]: https://github.com/adafruit/Adafruit-GFX-Library
[Adafruit ST7735]: https://github.com/adafruit/Adafruit-ST7735-Library
[Adafruit SAMD]: https://learn.adafruit.com/adafruit-feather-m0-basic-proto/using-with-arduino-ide
[Spreadfactor video]: https://www.youtube.com/watch?v=B580NvdXtjs

