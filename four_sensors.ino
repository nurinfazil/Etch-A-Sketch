/******************************************************************************
QRD1114_Proximity_Example.ino
Example sketch for SparkFun's QRD1114 Reflectance Proximity Sensor
  (https://www.sparkfun.com/products/246)
Jim Lindblom @ SparkFun Electronics
May 2, 2016

Connect a QRD1114, 330 resistor and 10k resistor as follows:

QRD1114 Pin ---- Arduino ---- Resistors
    1              A0      10k Pull-up to 5V
    2              GND
    3                      330 Resistor to 5V
    4              GND

As an object comes closer to the QRD1114, the voltage on A0 should go down.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int QRD1114_PIN0 = A0; //right
const int QRD1114_PIN1 = A1; //up
const int QRD1114_PIN2 = A2; //down
const int QRD1114_PIN3 = A3; //left
int out1= 2; 
int out2= 3; 
int out3= 4; 
int out4= 5; 

void setup() 
{
  Serial.begin(9600);  //connection to computer 
  pinMode(QRD1114_PIN0, INPUT); //right
  pinMode(QRD1114_PIN1, INPUT); //up
  pinMode(QRD1114_PIN2, INPUT); //down
  pinMode(QRD1114_PIN3, INPUT); //left
  
  pinMode(out1,OUTPUT); 
  pinMode(out2,OUTPUT);
  pinMode(out3,OUTPUT);
  pinMode(out4,OUTPUT);
}

void loop() 
{
  // Read in the ADC and convert it to a voltage:
  int proximityADC0 = analogRead(QRD1114_PIN0);
  int proximityADC1 = analogRead(QRD1114_PIN1);
  int proximityADC2 = analogRead(QRD1114_PIN2); 
  int proximityADC3 = analogRead(QRD1114_PIN3);
  
  float proximityV0 = (float)proximityADC0 * 3.3 / 1023.0;  
  float proximityV1 = (float)proximityADC1 * 3.3 / 1023.0;
  float proximityV2 = (float)proximityADC2 * 3.3 / 1023.0;
  float proximityV3 = (float)proximityADC3 * 3.3 / 1023.0;    
  char direction; 
  int zero=0;

  if (proximityV0<=2.00){
      direction = 'r';
      Serial.println(direction);
      digitalWrite(out1, HIGH);
      digitalWrite(out2, LOW);
      digitalWrite(out3, LOW);
      digitalWrite(out4, LOW);
      }
  else if (proximityV1<=2.00){
      direction = 'u';
      Serial.println(direction);
      digitalWrite(out1, LOW);
      digitalWrite(out2, HIGH);
      digitalWrite(out3, LOW);
      digitalWrite(out4, LOW);
      }
  else if (proximityV2<=2.00){
      direction = 'd';
      Serial.println(direction);
      digitalWrite(out1, LOW);
      digitalWrite(out2, LOW);
      digitalWrite(out3, HIGH);
      digitalWrite(out4, LOW);
      }
  else if (proximityV3<=2.00){
      direction = 'l';
      Serial.println(direction);
      digitalWrite(out1, LOW);
      digitalWrite(out2, LOW);
      digitalWrite(out3, LOW);
      digitalWrite(out4, HIGH);
      }
  else{
      Serial.println(zero);
      digitalWrite(out1, LOW);
      digitalWrite(out2, LOW);
      digitalWrite(out3, LOW);
      digitalWrite(out4, LOW);
      }
  
  delay(100); 
}
