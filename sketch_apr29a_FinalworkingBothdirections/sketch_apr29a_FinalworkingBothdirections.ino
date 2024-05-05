#define BUTTON_PIN 33  // GIOP21 pin connected to button
#define BUTTON_PIN2 32

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin

int lastState2 = LOW;  // the previous state from the input pin
int currentState2;     // the current reading from the input pin

int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 
 
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;
 
void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
    // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel); 
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP); 
  // testing
  Serial.print("Testing DC Motor...");
}
 
void loop() {
  

  currentState = digitalRead(BUTTON_PIN);
  currentState2 = digitalRead(BUTTON_PIN2);

  if (lastState == HIGH && currentState == LOW){
    Serial.println("The button is pressed");
    
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
 while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(5);
  }  
  dutyCycle = 200;    
  Serial.println("Turning right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
 delay(500);
} 

 else if (lastState == LOW && currentState == HIGH){
    Serial.println("The button is released");
     digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
 }




   if (lastState2 == HIGH && currentState2 == LOW){
    Serial.println("The button2 is pressed");
    digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
 while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(5);
  }  
  dutyCycle = 200;    
  Serial.println("Turning left");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
 delay(500);
    }
  else if (lastState2 == LOW && currentState2 == HIGH){
    Serial.println("The button2 is released");
    digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  }
  // save the the last state
  lastState = currentState;
  lastState2 = currentState2;
  // save the the last state
  }
  
  













 
