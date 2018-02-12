
#define GREEN_LED_PIN 6
#define RED_LED_PIN 14
#define YELLOW_LED_PIN 5

bool buttonState = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(YELLOW_LED_PIN,OUTPUT);
  pinMode(RED_LED_PIN,OUTPUT);
  pinMode(20,INPUT_PULLUP);
  pinMode(21,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = !digitalRead(21); 
  digitalWrite(GREEN_LED_PIN,buttonState);
  digitalWrite(YELLOW_LED_PIN,buttonState);
  digitalWrite(RED_LED_PIN,buttonState);
  
}
