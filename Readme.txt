
///////// SOURCE CODE FOR TM4C123G //////
         (IDE: VS Code + PlatformIO)    


******************************************************************************************************************
Connectivity Project: Serial->LoRa->Serial communication using (Tiva TM4C123G lauchpad + Ebyte's E32-868T20D LoRa 
module) and Heltec's ESP32 WiFi LoRa (V2)



                   ********                                         ********                          ********                                         
 Actuator <------> * GPIO * <------> Transmitter/Receiver  <------> * UART * <------> Module <------> * LoRa * <---
(TM4C123G          ********               (TM4C123G)                ********       (E32-868T20D)      ********
  Button)
                                     ********
 --->  Receiver/Transmitter <------> * UART * <------> Computer       
             (ESP32)                 ********     -   (notebook)
                                                  -
                                                  -    *******
                                                  ---> * I²C * <------> OLED 
                                                       *******       (SSD1306)


TM4C123G lauchpad: https://www.ti.com/lit/ug/spmu296/spmu296.pdf?ts=1617185733555&ref_url=https%253A%252F%252Fdev.ti.com%252F
Ebyte's E32-868T20D: https://www.ebyte.com/en/product-view-news.aspx?id=132
ESP 32 WiFi LoRa (V2): https://heltec.org/project/wifi-lora-32/