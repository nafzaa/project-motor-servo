#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define buzzer 3
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Servo myservo;

float roomTemp;
float objectTemp,stemp;
int readcount = 0;
float threshold = 0;

#define echoPin 11
#define trigPin 12
int maximumRange = 10;
int minimumRange = 5;
long duration, distance;
int dtime;

void setup ()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(180); 
  mlx.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.println("Temperature Sensor MLX90614");
  display.clearDisplay();
  display.setCursor(25,15);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Thermometer");
  display.setCursor(25,35);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(3000);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.031/2;

  objectTemp=0;
  objectTemp=threshold+mlx.readObjectTempC();
  roomTemp=mlx.readAmbientTempC();
  
  Serial.println("Object:" + String(objectTemp) + ", Ambient:" + String(roomTemp));
  Serial.println(distance);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,25);
  display.print("Dis;" + String(distance) + "cm");
  display.setCursor(65,25);
  display.print("Room:" + String(roomTemp).substring(0,4) + "C");
  display.display();
  display.setTextSize(2);
  display.setCursor(0,0);
  
  if (distance > maximumRange)
  {
    display.print("GET CLOSER");
  }
  if (distance < minimumRange)
  {
    display.print("TOO CLOSE!");
  }  

  if (distance >= minimumRange && distance <= maximumRange)
  {
    if (readcount == 5)
    {
      disptemp();
    }

    else
    {
      display.clearDisplay(); 
      display.setCursor(25,0);
      display.setTextSize(1);
      display.print("HOLD ON");
      display.setCursor(15,30);
      display.print("ScanTemperature");
      stemp = stemp + objectTemp;
      readcount++;
      dtime = 200;
    }
  } else
    {
      dtime = 100;
      readcount = 0;
      stemp = 0;
    }
    display.display();
    delay(dtime);
    Serial.println("count :"+String(readcount));
}

void disptemp()
{
  objectTemp = stemp / 5;
  display.setTextSize(1);
  display.print("YOUR TEMP;");
  display.setTextSize(2);
  display.setCursor(60,5);
  display.print(String(objectTemp).substring(0,4) + "C");
  display.display();
  readcount = 0;
  stemp = 0;
  if (objectTemp >=37.5)
  {
    digitalWrite (buzzer,HIGH);
    delay(2000);
    digitalWrite (buzzer,LOW);
  } else {
    myservo.write(90);
    delay(5000) 
    myservo.write(180);
  }
  dtime = 5000;
}
