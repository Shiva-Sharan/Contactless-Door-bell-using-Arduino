/**
 * @file Contactless_Doorbell.ino
 * @brief Smart Contactless Doorbell System using HC-SR04 Ultrasonic Sensor
 *
 * This system detects a user's presence within a specific threshold distance 
 * using an ultrasonic sensor and triggers a piezo buzzer, functioning as a 
 * touch-free doorbell to improve hygiene and automate entry workflows.
 */

// --- Pin Definitions ---
const int PIN_ULTRASONIC_TRIG = 9;
const int PIN_ULTRASONIC_ECHO = 8;
const int PIN_BUZZER = 12;

// --- System Configuration Constants ---
const int DISTANCE_THRESHOLD_CM = 50;   // Trigger threshold in centimeters
const int TRIGGER_DELAY_MS = 2000;      // Delay after triggering buzzer
const int POLL_INTERVAL_MS = 500;       // Sensor polling interval

// --- Helper Functions ---
/**
 * Retrieves the distance from the HC-SR04 sensor in centimeters.
 * @return distance in cm, or -1 if timeout/error occurs.
 */
int getDistanceCM() {
    // Ensure the trigger pin is low
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);

    // Send a 10 microsecond pulse to trigger the sensor
    digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

    // Read the echo pin pulse duration (timeout after 30000us ~ 5 meters)
    long duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, 30000);

    // If duration is 0, pulseIn timed out (no object detected in range)
    if (duration == 0) {
        return -1;
    }

    // Convert duration to distance in cm (Speed of sound = 343m/s)
    return duration * 0.034 / 2;
}

void setup() {
    // Initialize Hardware Pins
    pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
    pinMode(PIN_ULTRASONIC_ECHO, INPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    // Ensure buzzer is off on startup
    digitalWrite(PIN_BUZZER, LOW);

    // Initialize Serial Communication for debugging
    Serial.begin(9600);
    Serial.println("System Initialized: Contactless Doorbell Active.");
}

void loop() {
    int distance = getDistanceCM();

    if (distance > 0) {
        Serial.print("Target Detected at Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        // Check if target is within the activation threshold
        if (distance <= DISTANCE_THRESHOLD_CM) {
            Serial.println("Status: Threshold breached. Activating Bell.");
            
            digitalWrite(PIN_BUZZER, HIGH); // Activate doorbell
            delay(TRIGGER_DELAY_MS);        // Keep buzzer on for defined duration
            digitalWrite(PIN_BUZZER, LOW);  // Deactivate doorbell
            
            Serial.println("Status: Bell deactivated. Resetting sensor.");
        } else {
            digitalWrite(PIN_BUZZER, LOW); // Ensure buzzer is off
        }
    } else {
        Serial.println("Status: No target in range.");
        digitalWrite(PIN_BUZZER, LOW);
    }

    // Wait before next sensor poll to avoid echo overlapping
    delay(POLL_INTERVAL_MS);
}
