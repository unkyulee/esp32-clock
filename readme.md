# The Clock 

"The Clock" is a DIY project for people who care about how things look as much as how they work. Clean, minimal, and a little bit obsessive, it is designed to evolve over time. New design ideas, new cases, new features. The long term goal is to build the ultimate desk clock.

<img src="./Rev.2/images/clock.jpg" width="600px" />

---

## What is this project about?

The Clock is:

* A beautiful desk clock that feels at home next to a notebook, a mechanical keyboard, or your favorite pen.
* DIY friendly, using simple materials and straightforward steps.
* Design first, with focus on proportions, light, texture, and shape.
* Iterative, improving in small steps toward a more refined result each time.

---


## What can it do?

Right now, The Clock focuses on doing the basics really well:

* Show the time
* Sync automatically over WiFi
* Optional alarm with a small buzzer



## BOM

- 4x M3 Hex Screw Length 20mm
- 4x M3 Heated Inserts OD 4.5mm Length 3mm
- 1x [EC11 15mm Half handle](https://it.aliexpress.com/item/1005005983134515.html) 
- ESP32 S3 
- [ILI9341 320 \* 240 TFT LCD display](https://www.amazon.it/DIYmalls-Display-ILI9341-Resisive-320x240/dp/B0D8BLFJXG)
- [SPEAKER BUZZER 13 mm](https://www.amazon.it/SPEAKER-13x2-5mm-cicalino-altoparlante-piezoelettrico/dp/B084KLCRGG)
- ESP32 S3

## Wiring

| ESP32 | Connects to  |
| ----- | ------------ |
| 13    | Display MISO |
| 12    | SCLK         |
| 11    | MOSI         |
| 10    | CS           |
| 46    | DC           |
| 3     | RST          |
|       |              |
| 40    | EC11 Switch  |
| 45    | EC11 A       |
| 48    | EC11 B       |
|       |              |
| 7     | Buzzer (+)   |
| GND   | Buzzer (-)   |

Bridge the display VCC and LED


## Firmware

* Use Visual Studio with PlatformIO plugin installed
* Open the folder "Rev.2/platformio" 
* Wait for the plugins and library installed
* Open PlatformIO tab, and "Build and Upload" while ESP32 is plugged to PC


## 3D Files

STL files are available from the following link

* [3D Design Files](https://github.com/unkyulee/esp32-clock/tree/main/Rev.2/STL)

## Support

The Clock is an open-source project, built and nurtured with curiosity, care, and a genuine love for DIY enthusiasts. If you've enjoyed exploring the project or building your own Clock, and you'd like to show some support, even a small gesture goes a long way. Every contribution helps me continue improving The Clock Project and sharing it with the community.  

* [Buy me a coffee](https://www.buymeacoffee.com/unkyulee)  



## License

Creative Commons Legal Code

Attribution-NonCommercial 4.0 International

CC BY-NC 4.0

This is a human-readable summary of (and not a substitute for) the license. Disclaimer.

You are free to:

Share — copy and redistribute the material in any medium or format
Adapt — remix, transform, and build upon the material
The licensor cannot revoke these freedoms as long as you follow the license terms.

Under the following terms:

Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

NonCommercial — You may not use the material for commercial purposes.

No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

Notices:

You do not have to comply with the license for elements of the material in the public domain or where your use is permitted by an applicable exception or limitation.
No warranties are given. The license may not give you all of the permissions necessary for your intended use. For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.
