#include <Servo.h>
#include <Adafruit_NeoPixel.h>

/* Begin configuration settings */
const int min_command = 1000; // The minimum signal to be sent to the ESC
const int max_command = 2000; // The maximum signal to be sent to the ESC

const byte throttle_pin = 2; // The pin that the throttle input signal is plugged into
const byte yaw_pin = 3; // yaw input pin
const byte l_motor_pin = 9; // Left motor output pin
const byte r_motor_pin = 10; // Right motor output pin

const int reverse_yaw = -1; // 1 or -1 to reverse the yaw channel. 
const int yaw_deadband = 20; // ignore this much jitter on yaw channel
const int mix_strength = .5; // value from 0 (no mix) to 1 (100%) for the mix strength. May eventually set this value from another switch/channel on the receiver

/* Optional WS2812 LED Driver for marker strobes */
#define LED_PIN 4 // pin that the WS2812s are plugged into. Comment out to disable LEDs all together

/* End configuration */


#define NUM_PIXELS 6 // Can't be changed right now. First two lights are green (starboard), second two are red (port), third two are white (tail)

int throttle, yaw, modifyer = 1000;
float yaw_pct;

Servo l_motor, r_motor;

#ifdef LED_PIN
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

int framecounter = 0;
int frames_to_wait = 1000; // number of loops to wait before flashing the strobes

//int last_g_flash = 0;
//int last_r_flash = 0;


void setup() {
  // put your setup code here, to run once:

  pinMode(throttle_pin, INPUT);
  pinMode(yaw_pin, INPUT);

  l_motor.attach(l_motor_pin);
  r_motor.attach(r_motor_pin);


/* Optional WS2812 LEDs */
#ifdef LED_PIN
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,255,0));
  pixels.setPixelColor(1, pixels.Color(0,255,0));
  pixels.setPixelColor(2, pixels.Color(255,0,0));
  pixels.setPixelColor(3, pixels.Color(255,0,0));
  pixels.setPixelColor(4, pixels.Color(175,175,175));
  pixels.setPixelColor(5, pixels.Color(175,175,175));
#endif

  
  Serial.begin(115200);
}

void loop() {
  framecounter++;
  // put your main code here, to run repeatedly:

  // read input pwm from throttle and yaw
  throttle = pulseIn(throttle_pin, HIGH);
  yaw = pulseIn(yaw_pin,HIGH);

  
  if(yaw > 1500 + yaw_deadband || yaw < 1500 - yaw_deadband){
    yaw_pct = ((yaw-1500)/(float)500) * reverse_yaw;
  }else{
    yaw_pct = 0;
  }
  
  // calculate transformation
  modifyer = abs(yaw_pct) * (throttle - 1000) * mix_strength;
//  Serial.println("yaw pct"+(String) yaw_pct);
  
  // calculate & write left and right motor mixed throttle/yaw values
  if(yaw_pct < 0){
    l_motor.writeMicroseconds((throttle - modifyer) < min_command ? min_command : (throttle - modifyer));
    r_motor.writeMicroseconds((throttle + modifyer) > max_command ? max_command : (throttle + modifyer));
//    Serial.println("yaw left: "+ (String) (modifyer));
  }else{
    l_motor.writeMicroseconds((throttle + modifyer) > max_command ? max_command : (throttle + modifyer));
    r_motor.writeMicroseconds((throttle - modifyer) < min_command ? min_command : (throttle - modifyer));
//    Serial.println("yaw right: "+ (String) (modifyer));
  }

  String t;
  Serial.println("T: "+(String) throttle + "  Y: "+ (String) yaw + "  LM: " + (String) l_motor.readMicroseconds() + "  RM: "+ r_motor.readMicroseconds());


/* Strobe pattern
<framestowait>  <framestowait>  <framestowait>
          <strobe L>       <strobe R>
--------------||--------------||--------------
*/
#ifdef LED_PIN
  if(framecounter == frames_to_wait || framecounter == frames_to_wait+100){
    pixels.setPixelColor(0, pixels.Color(255,255,255));
    pixels.setPixelColor(1, pixels.Color(255,255,255));
  }else{
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(0,255,0));    
  }

  if(framecounter == frames_to_wait*2 || framecounter == (frames_to_wait*2+100)){
    pixels.setPixelColor(2, pixels.Color(255,255,255));
    pixels.setPixelColor(3, pixels.Color(255,255,255));
  }else{
    pixels.setPixelColor(2, pixels.Color(255,0,0));
    pixels.setPixelColor(3, pixels.Color(255,0,0));    
  }

  if(framecounter > frames_to_wait*3){
    framecounter = 0;
  }
#endif
  
}
