// Include the library for `servo`s
#include <Servo.h>;

// `Min` and `Max` angle for servo motor
#define MIN_ANGLE  15
#define MAX_ANGLE 165

// `Mid` and `Max` frequency for the `buzzer`
#define BUZZER_MAX_FREQ 1000
#define BUZZER_MID_FREQ  500

// `Mid` and `Far` distance for the `buzzer`
#define BUZZER_MID_DIST   35
#define BUZZER_CLOSE_DIST 15

// Define pins numbers for the `LED`
#define redPin   PD4
#define greenPin PD5
#define bluePin  PD6

// Define pin number for the `timerLed`
const int timerLed = 13;
int count = 0;
bool ledState = false;

// Define pins numbers for the `buzzer`
const int buzzerPin = 9;

// Define `Trig` and `Echo` pins of the Ultrasonic Sensor
const int trigPin   = 10;
const int echoPin   = 11;

// Variables for the `duration` and the `distance`
long duration;
int distance;

// Create a `Servo` object for controlling the `servo` motor
Servo servo;

// Initial `angle`
int angle = MIN_ANGLE;
int step  = 1;

ISR(TIMER1_COMPB_vect) {
  count++;
  if (count == 50) {
    count = 0;
    ledState = !ledState;
    digitalWrite(timerLed, ledState);
  }
}

void configure_timer() {
  // Set the `TCCR1B` register to `0`
  TCCR1B = 0;

  // Initialize counter value to `0`
  TCNT1 = 0;

  // Compare match register 16MHz/256/2Hz-1
  OCR1B = 31250;

  // CTC mode - compare to `OCR1B`
  TCCR1B |= (1 << WGM12);
  
  // Set prescaler to 256
  TCCR1B |= (1 << CS10);
}

void init_timer() {
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1B);
}

void setup() {
  // Initialize `LED` pins as output
  DDRD |= (1 << redPin);
  DDRD |= (1 << greenPin);
  DDRD |= (1 << bluePin);

  // Initialize `timerLed` pin as output
  pinMode(timerLed, OUTPUT);

  // Configure `Timer` for `1` second interval
  cli();
  configure_timer();
  init_timer();
  sei();

  // Initialize `buzzer` pin as output
  pinMode(buzzerPin, OUTPUT);

  // Set the `trigPin` as OUT and the `echoPin` as IN
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Define on which pin is the servo motor attached
  servo.attach(12);

  // Set the baud rate to `9600` for the `Serial` communication
  Serial.begin(9600);
}

void loop() {
  // Rotate the servo motor from `MIN_ANGLE` to `MAX_ANGLE` degrees and back
  // Calculate the `angle` for the next step
  if (angle <= MIN_ANGLE && step == -1)
    step = 1;
  else if (angle >= MAX_ANGLE && step == 1)
    step = -1;
  angle += step;

  // Rotate `servo` to the current `angle`
  servo.write(angle);
  // Wait for `30` milliseconds
  delay(30);
  // Calculate the `distance` measured by the Ultrasonic sensor for each degree
  distance = calculateDistance();

  // Send the current `angle` and the `distance`
  // measured by the Ultrasonic sensor into the Serial Port
  // Also, send `,` and `.` characters for indexing purposes (in the Processing IDE)
  Serial.print(angle);
  Serial.print(',');
  Serial.print(distance);  
  Serial.print('.');

  if (distance < BUZZER_CLOSE_DIST) {
    // `Close` distance - make the `buzzer` sound loud and make the `LED` red
    tone(buzzerPin, BUZZER_MAX_FREQ);
    redColor();
  }
  else if (distance < BUZZER_MID_DIST) {
    // `Mid` distance - make the `buzzer` sound medium and make the `LED` blue
    tone(buzzerPin, BUZZER_MID_FREQ);
    blueColor();
  } else {
    // `Far` distance - turn off the `buzzer` and make the `LED` green 
    noTone(buzzerPin);
    greenColor();
  }
}

void redColor() {
  PORTD |=  (1 << redPin);
  PORTD &= ~(1 << greenPin);
  PORTD &= ~(1 << bluePin);
}

void blueColor() {
  PORTD |=  (1 << bluePin);
  PORTD &= ~(1 << redPin);
  PORTD &= ~(1 << greenPin);
}

void greenColor() {
  PORTD |=  (1 << greenPin);
  PORTD &= ~(1 << redPin);
  PORTD &= ~(1 << bluePin);
}

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {
  // Set the `trigPin` to `LOW` state for 2 microseconds
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the `trigPin` to `HIGH` state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  // Set the `trigPin` back to `LOW` state
  digitalWrite(trigPin, LOW);

  // Read the `echoPin`, return the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}
