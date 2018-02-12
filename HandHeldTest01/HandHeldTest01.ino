
#define GREEN_LED_PIN 6
#define RED_LED_PIN 14
#define YELLOW_LED_PIN 5

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(YELLOW_LED_PIN,OUTPUT);
  pinMode(RED_LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(GREEN_LED_PIN,HIGH);
  digitalWrite(YELLOW_LED_PIN,HIGH);
  digitalWrite(RED_LED_PIN,HIGH);
  delay(100);
  digitalWrite(GREEN_LED_PIN,LOW);
  digitalWrite(YELLOW_LED_PIN,LOW);
  digitalWrite(RED_LED_PIN,LOW);
  delay(100);
  
}
