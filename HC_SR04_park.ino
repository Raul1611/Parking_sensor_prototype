#include<LiquidCrystal_I2C.h> //for LCD
#include<Wire.h> //for I2C communication

#define BUZZ PB1 //buzzer pin 
#define Button PC0 //button pin
const int buzz = 9;

//LED pins
#define GL1 PB4
#define GL2 PB3
#define YL1 PB2
#define YL2 PD3      
#define RL1 PB0
#define RL2 PD7

//HC_SR04 pins
#define TRIGG PD6
#define ECHO PD5

#define MAX_VAL 120
#define HYSTERESIS 3
#define STEP 20

LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd object

unsigned long duration;
unsigned int distance;
unsigned short int old;
unsigned short int flg;

void setup()
{
  //initialize lcd
  lcd.init();
  lcd.backlight();
  
  //set pins as output
  DDRB|=(1<<GL1) | (1<<GL2) | (1<<YL1) | (1<<RL1) | (1<<BUZZ);
  DDRD|=(1<<RL2) | (1<<YL2) | (1<<TRIGG);
  //set pins as input
  DDRC&=~(1<<ECHO);
  DDRC&=~(1<<Button); 
  
  //timer0_init(); //initialize timer
  //PWM_init();
}

void loop()
{  
   int d = get_distance();
   lcd.setCursor(0,0);
   lcd.print("Distance: ");

   if(d < 10)
   {
    lcd.print(d);
    lcd.setCursor(11,0);
    lcd.print(" cm ");
   }
   else if(d < 100 && d >= 10)
   {
    lcd.print(d);
    lcd.setCursor(12,0);
    lcd.print(" cm ");
   }
   else if(d <= 450 && d >= 100)
   {
    lcd.print(d);
    lcd.setCursor(13,0);
    lcd.print(" cm ");
   }
   else
    lcd.print(d);
 

  if((PINC&(1<<Button)) && !old)
  {
    flg^=1; //toggle flag
  }
  old = PINC&(1<<Button);
    
  if(!flg)
  {
    //leds off
   PORTB&=~((1<<GL1) | (1<<GL2) | (1<<YL1) | (1<<RL1));
   PORTD&=~((1<<RL2) | (1<<YL2));
  
   lcd.setCursor(0,1);
   lcd.print("Park mode off ");
  }
  else
  {
    led_indicator(d);
    lcd.setCursor(0,1);
    lcd.print("Park mode on ");

     if (d < 120) 
     {
       tone(buzz,1000);
       delay(40);
       noTone(buzz);
       delay(d*4);
     }
  }
}

int get_distance()
{
   PORTD&=~(1<<ECHO); //clear echo pin
   PORTD&=~(1<<TRIGG); //clear trigg pin
   delayMicroseconds(2);
   //set trigg pin for 10 us, so the sensors's transmiter will send pulses
   PORTD|=(1<<TRIGG);
   delayMicroseconds(10);
   //Reads the echoPin, returns the sound wave travel time in microseconds
   duration = pulseIn(ECHO, 1);
   //distance = (speed of sound (0.034 cm/us) * time) / 2 (go and back)
   distance = duration * 0.034 / 2;
   
   return distance; 
}

void led_indicator(int d)
{
  //check distance, and switch leds on/off based on distance value
  if(((d < MAX_VAL) && (d > MAX_VAL-STEP+HYSTERESIS)) || (d > MAX_VAL) )
  {
    //OCR1A=42;
    PORTB|=1<<GL1;
    PORTB&=~(1<<GL2);
    PORTB&=~(1<<YL1);
    PORTD&=~(1<<YL2);
    PORTB&=~(1<<RL1);
    PORTD&=~(1<<RL2);
  }
  else if((d < MAX_VAL-STEP-HYSTERESIS) && (d > MAX_VAL-2*STEP+HYSTERESIS))
  {
    //OCR1A=84;
    PORTB|=1<<GL1;
    PORTB|=1<<GL2;
    PORTB&=~(1<<YL1);
    PORTD&=~(1<<YL2);
    PORTB&=~(1<<RL1);
    PORTD&=~(1<<RL2);
  }
  else if((d < MAX_VAL-2*STEP-HYSTERESIS) && (d > MAX_VAL-3*STEP+HYSTERESIS))
  {
    //OCR1A=126;
    PORTB|=1<<GL1;
    PORTB|=1<<GL2;
    PORTB|=1<<YL1;
    PORTD&=~(1<<YL2);
    PORTB&=~(1<<RL1);
    PORTD&=~(1<<RL2);
  }
  else if((d < MAX_VAL-3*STEP-HYSTERESIS) && (d > MAX_VAL-4*STEP+HYSTERESIS))
  {
    //OCR1A=168;
    PORTB|=1<<GL1;
    PORTB|=1<<GL2;
    PORTB|=1<<YL1;
    PORTD|=1<<YL2;
    PORTB&=~(1<<RL1);
    PORTD&=~(1<<RL2);
  }
  else if((d < MAX_VAL-4*STEP-HYSTERESIS) && (d > MAX_VAL-5*STEP+HYSTERESIS))
  {
    //OCR1A=210;
    PORTB|=1<<GL1;
    PORTB|=1<<GL2;
    PORTB|=1<<YL1;
    PORTD|=1<<YL2;
    PORTB|=1<<RL1;
    PORTD&=~(1<<RL2);
  }
  else if((d < MAX_VAL-5*STEP-HYSTERESIS) && (d > MAX_VAL-6*STEP+HYSTERESIS))
  {
    //OCR1A=255;
    PORTB|=1<<GL1;
    PORTB|=1<<GL2;
    PORTB|=1<<YL1;
    PORTD|=1<<YL2;
    PORTB|=1<<RL1;
    PORTD|=1<<RL2;
  }
}

/*
void PWM_init()
{
  cli();//disable interrupts
  //initialize registers
  TCCR1A=0;
  TCCR1B=0;
  //Clear OC1A/OC1B on compare match (set output to low level).
  TCCR1A|=(1<<COM1A1);
  //fast pwm, 8-bit
  TCCR1A|=(1<<WGM10);
  TCCR1B|=(1<<WGM12);
  //prescaler /1024
  TCCR1B|=(1<<CS12) | (1<<CS10);
  sei();//enable interrupts
}
*/

/*
void timer0_init()
{
  cli(); //stop interrupts for till we make the settings

  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments (1ms interrupt occurence)
  OCR0A = 250;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei(); //enable interrupts
}

ISR(TIMER0_COMPA_vect){    
  
}
*/
