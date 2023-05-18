// Include the library for `servo`s
#include <Servo.h>;

// `Min` and `Max` angle for servo motor
#define MIN_ANGLE  15
#define MAX_ANGLE 165

// `Mid` and `Max` frequency for the `buzzer`
#define BUZZER_MAX_FREQ 1000
#define BUZZER_MID_FREQ  500

// `Mid` and `Far` distance for the `buzzer`
#define BUZZER_MID_DIST   30
#define BUZZER_CLOSE_DIST 15

// Define pins numbers for the `LED`
const int redPin    = 4;
const int greenPin  = 5;
const int bluePin   = 6;

// Define pins numbers for the `buzzer`
const int buzzerPin = 9;

// Define `Trig` and `Echo` pins of the Ultrasonic Sensor
const int trigPin   = 10;
const int echoPin   = 11;

// Variables for the `duration` and the `distance`
long duration;
int distance;

// Create a `Servo` object for controlling the `servo` motor
Servo myServo;

// Initial `angle`
int angle = MIN_ANGLE;
int step  = 1;

void setup() {
  // Initialize `LED` pins as an output
  pinMode(redPin,   OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,  OUTPUT);

  // Initialize `buzzer` pin as an output
  pinMode(buzzerPin, OUTPUT);

  // Set the `trigPin` as an OUT and the `echoPin` as an IN 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Define on which pin is the servo motor attached
  myServo.attach(12);

  // Set the baud rate to `9600` for the `Serial` communication
  Serial.begin(9600);
}

void loop() {
  // Rotate the servo motor from `MIN_ANGLE` to `MAX_ANGLE` degrees
  while (1) {
    // Calculate the `angle` for the next step
    if (angle <= MIN_ANGLE && step == -1)
      step = 1;
    else if (angle >= MAX_ANGLE && step == 1)
      step = -1;
    angle += step;

    // Rotate `servo` to the current `angle`
    myServo.write(angle);
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

    if (distance < BUZZER_CLOSE_DIST) { // Close
      tone(buzzerPin, BUZZER_MAX_FREQ);
      setColor(255, 0, 0);
    }
    else if (distance < BUZZER_MID_DIST) { // Medium
      tone(buzzerPin, BUZZER_MID_FREQ);
      setColor(255, 255, 0);
    } else { // Far
      setColor(0, 255, 0);
      noTone(buzzerPin);
    }
  }
  
}

void setColor(int red, int green, int blue) {
    analogWrite(redPin,   red);
    analogWrite(greenPin, green);
    analogWrite(bluePin,  blue);
}

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;
  return distance;
}