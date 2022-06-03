#include <Adafruit_GFX.h>           
#include <Adafruit_SSD1306.h>        
//#define OLED_RESET 10
//Adafruit_SSD1306 display(OLED_RESET);
#include <Adafruit_ADS1015.h>         
Adafruit_ADS1015 ads(0x48);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);


//inputs/outputs 
int res_2k = 6;
int res_20k = 7;
int res_470k = 8;
#define cap_in_analogPin    0          
#define chargePin           13        
#define dischargePin        11
int pulse_induct_out = 5;
int oscillate_in = 9;
const int OUT_PIN = A1;
//const int IN_PIN = A0;
int top_but = 2;
int mid_but = 3;
int bot_but = 4;


//Modes variables
int mode = 0;
int res_scale = 0;
//int cap_scale = 0;
bool mid_but_state = true;
bool top_but_state = true;


//Variables for voltage mode
float Voltage = 0.0;
float resistance_voltage = 0.0;
float battery_voltage = 0.0;
float measured_resistance = 0.0;


//Variables for resistance mode
float Res_2k_value = 2170;         //2K resistor 
float Res_20k_value = 21.90;      //20K resistor
float Res_470k_value = 0.4640;    //470K resistor


//Variables for big capacitance mode
#define resistorValue  9920.0F
unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;
//const float IN_STRAY_CAP_TO_GND = 47.48;
//const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
//const float R_PULLUP = 34.8;  
//const int MAX_ADC_VALUE = 212;


//Variables for inductance mode
double pulse, frequency, capacit, inductance;
float C_cap_value = 2E6;                    //The capacitor value used

//Variables for current mode with the ACS712 of 5A range
float Current_sensor_Resolution = 0.185;



void setup(void)
{  
  pinMode(top_but,INPUT_PULLUP);
  pinMode(mid_but,INPUT_PULLUP);
  pinMode(bot_but,INPUT_PULLUP);
  pinMode(res_2k,OUTPUT);
  pinMode(res_20k,INPUT);
  pinMode(res_470k,INPUT);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(cap_in_analogPin, OUTPUT); 
  pinMode(pulse_induct_out,OUTPUT);
  pinMode(oscillate_in,INPUT);
  
  digitalWrite(res_2k,LOW);

  pinMode(chargePin, OUTPUT);     
  digitalWrite(chargePin, LOW); 
  
  lcd.init();
  lcd.backlight(); 
  lcd.setCursor(0,0);
  lcd.print("MULTIMETER");
 
  delay(1000);
  Serial.begin(9600);
  ads.begin();
}

void loop(void)
{

   
      
  if(!digitalRead(mid_but) && mid_but_state)
  {
    mode = mode + 1;
    res_scale = 0;
//    cap_scale = 0;
    mid_but_state = false;
    if(mode > 4)
    {
      mode=0;
    }
    delay(100);
  }

  if(digitalRead(mid_but) && !mid_but_state)
  {
    mid_but_state = true;
  }


  if(!digitalRead(top_but) && top_but_state)
  {
    res_scale = res_scale + 1;
//    cap_scale = cap_scale + 1;
    top_but_state = false;
    if(res_scale > 2)
    {
      res_scale=0;
    }
//    if(cap_scale > 1)
    {
//      cap_scale=0;
    }
     startTime = micros();           //Reset the counters      
     elapsedTime= micros() - startTime;
    delay(100);
  }

  if(digitalRead(top_but) && !top_but_state)
  {
    top_but_state = true;
  }

  


////////////////////////////VOLTAGE/////////////////////////////
  if(mode == 0)
  {
    int16_t adc0;
    adc0 = ads.readADC_SingleEnded(0);
    Voltage = (adc0 * 0.003);    
    Voltage = (Voltage / 0.245108); //Compensate for voltage divider
    if(Voltage > 0.6)
    {
      Voltage = Voltage +  0.55; //(voltage drop on the diode only if voltage is applied)
    }
    int16_t adc2;  
    adc2 = ads.readADC_SingleEnded(2);
//    battery_voltage = ((adc2 * 0.1875)/1000);
    battery_voltage = ((adc2 * 0.003));      
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Voltage");
    

    if(Voltage > 0)
    {    
      lcd.setCursor(12,0);
      lcd.print(battery_voltage);
      lcd.setCursor(0,1);
      lcd.print(Voltage);
      lcd.setCursor(15,1);
      lcd.print("V");
      Serial.print(battery_voltage);
      Serial.print("\n");
      delay(500);
    }
    else
    {    
      lcd.setCursor(12,0);
      lcd.print(battery_voltage);
      lcd.setCursor(0,1);
      lcd.print("0.000");
      lcd.setCursor(15,1);
      lcd.print("V");
      delay(500);
    }
  }






////////////////////////////RESISTANCE/////////////////////////////
  if(mode == 1)
  {
    if(res_scale == 0)
    {
      pinMode(res_2k,OUTPUT);
      pinMode(res_20k,INPUT);
      pinMode(res_470k,INPUT);     
      digitalWrite(res_2k,LOW);
  
      int16_t adc1; 
      adc1 = ads.readADC_SingleEnded(1);
      resistance_voltage = (adc1 * 0.003);
      int16_t adc2;   
      adc2 = ads.readADC_SingleEnded(2); 
      battery_voltage = ((adc2 * 0.003));
      measured_resistance = (Res_2k_value +10) * (  (battery_voltage/resistance_voltage)-1  );
      if(measured_resistance < 4000)
      {

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Resistance");
        lcd.setCursor(12,0);
        lcd.print(battery_voltage);
        lcd.setCursor(0,1);
        lcd.print(measured_resistance);
        lcd.setCursor(13,1);
        lcd.print("Ohm");
        delay(500);        
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Resistance");
        lcd.setCursor(12,0);
        lcd.print(battery_voltage);
        lcd.setCursor(0,1);
        lcd.print(" > 4000");
        lcd.setCursor(13,1);
        lcd.print("Ohm");
        delay(500);   
      }
    }
    if(res_scale == 1)
    {
      pinMode(res_2k,INPUT);
      pinMode(res_20k,OUTPUT);
      pinMode(res_470k,INPUT);     
      digitalWrite(res_20k,LOW);
      int16_t adc1; 
      adc1 = ads.readADC_SingleEnded(1);
      resistance_voltage = (adc1 * 0.003);
      int16_t adc2;  
      adc2 = ads.readADC_SingleEnded(2);
      battery_voltage = ((adc2 * 0.003));      
      measured_resistance = (Res_20k_value +0.01) * (  (battery_voltage/resistance_voltage)-1  );
      if(measured_resistance < 200)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Resistance");
        lcd.setCursor(12,0);
        lcd.print(battery_voltage);
        lcd.setCursor(0,1);
        lcd.print(measured_resistance);
        lcd.setCursor(12,1);
        lcd.print("KOhm");
        delay(500);         
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Resistance");
        lcd.setCursor(0,1);
        lcd.setCursor(12,0);
        lcd.print(battery_voltage);
        lcd.print(" > 200K ");
        lcd.setCursor(12,1);
        lcd.print("KOhm");
        delay(500);
      }
    }
    if(res_scale == 2)
    {
      pinMode(res_2k,INPUT);
      pinMode(res_20k,INPUT);
      pinMode(res_470k,OUTPUT);     
      digitalWrite(res_470k,LOW);
      int16_t adc1; 
      adc1 = ads.readADC_SingleEnded(1);
      resistance_voltage = (adc1 * 0.003);
      int16_t adc2; 
      adc2 = ads.readADC_SingleEnded(2);
      battery_voltage = ((adc2 * 0.003));
      measured_resistance = (Res_470k_value + 0.00001) * (  (battery_voltage/resistance_voltage)-1  );
      if(measured_resistance < 4 && measured_resistance > 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Resistance");
        lcd.setCursor(12,0);
        lcd.print(battery_voltage);
        lcd.setCursor(0,1);
        lcd.print(measured_resistance);
        lcd.setCursor(13,1);
        lcd.print("MOhm");
        delay(500);   
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Resistance");
        lcd.setCursor(12,0);
        lcd.print(battery_voltage);
        lcd.setCursor(0,1);
        lcd.print(" > 4 M");
        lcd.setCursor(13,1);
        lcd.print("MOhm");
        delay(500); 
      }
    }
  }//end of mode 1 = resistance mode


  if(mode == 2)
  {
    int16_t adc2; 
    adc2 = ads.readADC_SingleEnded(2);
    battery_voltage = (adc2 * 0.003);
    if(true)    
    {   
      pinMode(cap_in_analogPin, INPUT);  
      pinMode(OUT_PIN,OUTPUT);
      digitalWrite(OUT_PIN, LOW); 
      pinMode(chargePin, OUTPUT);   
       
      digitalWrite(chargePin, HIGH);  //apply 5 Volts
      startTime = micros();           //Start the counter
      while(analogRead(cap_in_analogPin) < 648){       //While the value is lower than 648, just wait
      }
    
      elapsedTime= micros() - startTime;
      microFarads = ((float)elapsedTime / resistorValue) ; //calculate the capacitance value
  
  
      if (microFarads > 1)
      {
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Capacitance");
         lcd.setCursor(12,0);
         lcd.print(battery_voltage);
         lcd.setCursor(0,1);
         lcd.print(microFarads);
         lcd.setCursor(14,1);
         lcd.print("uF");
         delay(500);   
      }
    
      else{
        nanoFarads = microFarads * 1000.0; 
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Capacitance");
         lcd.setCursor(12,0);
         lcd.print(battery_voltage);
         lcd.setCursor(0,1);
         lcd.print(nanoFarads);
         lcd.setCursor(14,1);
         lcd.print("nF");
         delay(500);
      }
  
      digitalWrite(chargePin, LOW);            
      pinMode(dischargePin, OUTPUT);            
      digitalWrite(dischargePin, LOW);     //discharging the capacitor     
      while(analogRead(cap_in_analogPin) > 0){         
      }//This while waits till the capaccitor is discharged
    
      pinMode(dischargePin, INPUT);      //this sets the pin to high impedance
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Discharging...");       
    }
    
  }//end of mode 2  = capacitance




  if(mode == 3)
  {
    
    digitalWrite(pulse_induct_out, HIGH);
    delay(4);//give some time to charge inductor.
    digitalWrite(pulse_induct_out,LOW);
    delayMicroseconds(100); //make sure resonation is measured
    pulse = pulseIn(oscillate_in,HIGH,5000);//returns 0 if timeout
    
    if(pulse > 0.1){ //if a timeout did not occur and it took a reading:  
      capacit = 21.E-7;
      
      int16_t adc2;  
      adc2 = ads.readADC_SingleEnded(2);
      battery_voltage = ((adc2 * 0.003));
      
      frequency = C_cap_value/(2*pulse);
      inductance = 1./(capacit*frequency*frequency*4.*3.14159*3.14159);
      inductance *= C_cap_value;

       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Inductance:");
       lcd.setCursor(12,0);
       lcd.print(battery_voltage);
       lcd.setCursor(0,1);
       lcd.print(inductance);
       lcd.setCursor(14,1);
       lcd.print("uH");
       delay(500);      
    }
    else
    {
      int16_t adc2;
      adc2 = ads.readADC_SingleEnded(2);
      battery_voltage = ((adc2 * 0.003));
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Inductance:");
       lcd.setCursor(12,0);
       lcd.print(battery_voltage);
       lcd.setCursor(0,1);
       lcd.print("NONE");
       lcd.setCursor(14,1);
       lcd.print("uH");  
       delay(500);
    }
    
  }//end of mode 3 = inductance
  
   //Current mode
   if(mode == 4)
   {  
      int16_t adc3;          
      adc3 = ads.readADC_SingleEnded(3);
      float SensorVoltage = ((adc3 * 0.003)); //Pass from digital to voltage values  
      float I = ((SensorVoltage-1.8)/Current_sensor_Resolution)*1.25; 
        
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Amperage");
      lcd.setCursor(12,0);
      lcd.print(battery_voltage);
      lcd.setCursor(0,1);
      lcd.print(I);
      lcd.setCursor(15,1);
      lcd.print("A");
      delay(500);
    
   }//end of mode 4 = current


  


  
}
