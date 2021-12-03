# Oculus #  
The professional PCB version of my socketCar

This project involves PCB design to build a car with the PCB as the Chassis, N20 micro gear motors, ESP8266 and a motor driver (DRV8833 in this case can use any alternatives).
I have updated the socketCar design and added a pear of headlights which double down as status leds.

<img src="https://raw.githubusercontent.com/Prateek7805/Oculus/main/2.JPG" max-width='350' height='auto'/>
 
### ESP12 to DRV833 (Motor Driver) connections: ###
| ESP GPIO  | TB66FNG  |
|:---------:|:--------:|
| 14        | IN1      |
| 13        | IN2      |
| 12        | IN3      |            
| 5         | IN4      |


### Other pin connections: ###
| ESP GPIO  | PIN      |
|:---------:|:--------:|
| 15        | FLASH    |
| 16        | RST      |

### [ Working - part 1](https://www.instagram.com/p/CWa1VD3lScI/) ###
### [ Working - part 2](https://www.instagram.com/p/CWd5ZtoFdUe/) ###
  
### Pinout ###
<img src="https://raw.githubusercontent.com/Prateek7805/Oculus/main/pinout.JPG" max-width='250' height='auto'/>

### keyboard contols ###
| key                | command                 |
|:------------------:|:-----------------------:|
| Arrow UP           | Foraward                |
| Arrow DOWN         | Reverse                 |
| Arrow LEFT         | Left                    |
| Arrow RIGHT        | Right                   |
| Space              | Stop                    |
| Plus or Num Plus   | Speed +                 |
| Minus or Num Minus | Speed -                 |
| B                  | headlights Toggle       |
| H                  | headlights Brightness + |
| N                  | headlights Brightness - |

### Features ###
* sidebar - control panel
* Single page web interface
* multi-touch
* keyboard contols
* User defined controls with reset default
* Gamepad controls 
* realtime gamepad controls check
* Plug and play capabitity
* Toggle between keyboard and gamepad
* real-time with WebSocket
* deepsleep
* Soft Restart
* light-dark mode
* memory
* On the fly WiFi credential change
* Responsive design
* credential validation 
* factory reset functionality
* status indication using the headlights
* speed control
* headlight brightness control
* use of smd components so the car is reversible
* battery charging - tp4056
* car stops when out of range or disconnected
