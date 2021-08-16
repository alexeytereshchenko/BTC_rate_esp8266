# BTC_rate_esp8266
Bitcoin rate on i2c lcd display 1602

![framework](https://img.shields.io/badge/framework-platformio-orange)
![CMU](https://img.shields.io/badge/MCU-esp8266-success)
![board](https://img.shields.io/badge/board-nodeMCU%20v3-blue)

![git](/docs/assets/demo.gif)

## How to upload?

1) Install PlatformIO Core (CLI) from [here](https://docs.platformio.org/en/latest/core/installation.html).

2) Clone the repository.

        git clone https://github.com/Anth3995/BTC_rate_esp8266.git

3) Enter your SSID and password of WIFI to src/main.cpp on 14 and 15 lines.

4) Connect MCU to pc.

5) Into folder run:

        pio run --target upload

## Important information:

LCD display have to 5V power. 3.3V is small.
