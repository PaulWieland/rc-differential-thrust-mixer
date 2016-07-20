# rc-differential-thrust-mixer
An Arduino based differential thrust mixer for twin engine RC Airplanes

# What is it?
A differential thrust mixer combines RC Throttle and Yaw input from a receiver or flight controller and then outputs Left Motor & Right Motor throttle values.

# Why did you write it?
Most RC Transmitters support channel mixing right in the radio. Some radios do not. In my case, my radio supports it but I want to use an [Eagle Tree Vector](http://www.eagletreesystems.com/index.php?route=product/product&product_id=136) on a [Sea Duck](http://flitetest.com/articles/sea-duck-conwing-l-16) I bought from FliteTest. 

The Vector does not support planes that rely on differential thrust for yaw. My solution was to use an Arduino to intercept the Throttle and Rudder outputs from the Vector, mix them and send the result to the ESCs.

# How does it work?
0. I used an [Adafruit Trinket Pro w/5v logic](https://www.adafruit.com/product/2000) (non pro version does not work!)
0. Two pins are setup to read the PWM signal from the Vector
0. Two pins are setup to write the PWM (in microseconds) to the ESCs
0. When yawing left, the program calculates the % of yaw then _reduces_ the left motor by that amount while _increasing_ the right motor by the same amount.
  0. If the throttle value is set at 50% and a 50% yaw left command is given, the program will send 75% thrust to the right motor and 25% thrust to the left motor.
  0. Throttle = 50%, Yaw Left = 100%, Left Motor = 0%, Right Motor = 100%
  0. Throttle = 100%, Yaw Left = 25%, Left Motor = 75%, Right Motor = 100%
