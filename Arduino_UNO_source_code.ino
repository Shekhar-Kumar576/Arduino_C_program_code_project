#include <Servo.h>

// Define flame sensor pins
#define FLAME_SENSOR_LEFT A0
#define FLAME_SENSOR_CENTER A1
#define FLAME_SENSOR_RIGHT A2

// Define motor driver pins
#define MOTOR_L1 5  
#define MOTOR_L2 6  
#define MOTOR_R1 9  
#define MOTOR_R2 10 

// Define relay for water pump & Servo
#define RELAY_PIN 7
#define SERVO_PIN 3

Servo waterServo;
#define FIRE_DETECTED_THRESHOLD  22  // Best for indoor LED lighting

void setup() {
    Serial.begin(9600);

    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);  

    waterServo.attach(SERVO_PIN);
    waterServo.write(90); 

    pinMode(FLAME_SENSOR_LEFT, INPUT);
    pinMode(FLAME_SENSOR_CENTER, INPUT);
    pinMode(FLAME_SENSOR_RIGHT, INPUT);

    Serial.println("🚀 Fire Fighting Robot Initialized...");
}

void loop() {
    int flameLeft = analogRead(FLAME_SENSOR_LEFT);
    int flameCenter = analogRead(FLAME_SENSOR_CENTER);
    int flameRight = analogRead(FLAME_SENSOR_RIGHT);

    // Prioritize Center Fire
    if (flameCenter < FIRE_DETECTED_THRESHOLD) {
        Serial.println("🔥 Fire Center! Approaching...");
        activateWaterPump();
        moveForward();
        delay(200);          // Drive forward slightly to get into range
        stopMotors();        // CRITICAL: Stop moving before spraying!
     
    }
    else if (flameLeft < FIRE_DETECTED_THRESHOLD) {
        Serial.println("🔥 Fire Right! Turning...");
        turnRight();
        activateWaterPump(); 
    }
    else if (flameRight < FIRE_DETECTED_THRESHOLD) {
        Serial.println("🔥 Fire Left! Turning...");
        turnLeft();
        activateWaterPump(); 
    }
    else {
        stopMotors();
        deactivateWaterPump();  
    }
    
    delay(500); // Reduced delay so robot is more responsive
}

void moveForward() {
    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
}

void turnLeft() {
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, HIGH);
    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
    delay(200);  
    stopMotors();
}

void turnRight() {
    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, HIGH);
    delay(200);  
    stopMotors();
}

void stopMotors() {
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, LOW);
}

void activateWaterPump() {
    digitalWrite(RELAY_PIN, LOW);  
    // Faster, smoother sweeping motion
    for (int pos = 60; pos <= 120; pos += 10) {  
        waterServo.write(pos);
        delay(60);
    }
    for (int pos = 120; pos >= 60; pos -= 10) {  
        waterServo.write(pos);
        delay(60);
    }
    digitalWrite(RELAY_PIN, HIGH);  
    waterServo.write(90);  
}

void deactivateWaterPump() {
    digitalWrite(RELAY_PIN, HIGH);  
    waterServo.write(90);  
}
