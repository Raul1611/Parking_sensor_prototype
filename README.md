# Parking_sensor_prototype
Parking sensor using an ultrasonic sensor.

Hardware components: 
- HC-SR04 ultrasonic sensor
- LCD 1602
- I2C communication module
- push button
- 6 leds
- active 5v buzzer
- nano ATMEGA328P board

The system has 2 states, one in wich the lcd displays the distance in real time, and one in which it behaves like a parking sensor. The push button is used to switch between states. When entering the park state, the buzzer starts emitting sound that intensifies as the distance gets smaller, also the 6 leds are used as distance indicators besides the lcd.


