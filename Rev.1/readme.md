# WiFi Digital Clock with ESP32

<img src="./images/clock.jpg" width="600px" />

### BOM

- 4x M3 Hex Screw Length 5mm
- 4x M3 Heated Inserts OD 4.5mm Length 3mm
- 1x [EC11 15mm Half handle](https://it.aliexpress.com/item/1005005983134515.html) 
- ESP32 S3 
- [ILI9341 320 \* 240 TFT LCD display](https://www.amazon.it/DIYmalls-Display-ILI9341-Resisive-320x240/dp/B0D8BLFJXG)
- 8x Cherry MX compatible switches
- [SPEAKER BUZZER 13 mm](https://www.amazon.it/SPEAKER-13x2-5mm-cicalino-altoparlante-piezoelettrico/dp/B084KLCRGG)

### Wiring

| ESP32 | Connects to  |
| ----- | ------------ |
| 13    | Display MISO |
| 12    | SCLK         |
| 11    | MOSI         |
| 10    | CS           |
| 46    | DC           |
| 3     | RST          |
|       |              |
| 15    | Button 1     |
| 16    | Button 2     |
| 17    | Button 3     |
| 18    | Button 4     |
| 1     | Button 5     |
| 2     | Button 6     |
| 42    | Button 7     |
| 41    | Button 8     |
|       |              |
| 40    | EC11 Switch  |
| 45    | EC11 A       |
| 48    | EC11 B       |
|       |              |
| 7     | Buzzer       |

<img src="./images/wiring.jpg" width="600px" />