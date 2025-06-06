// Define pins for the ultrasonic sensor and buzzer
const int trigPin = 9;
const int echoPin = 8;
const int buzzPin = 12;
const int distanceThreshold = 50; // Distance threshold in centimeters (0.5 meters)

void setup() {
    pinMode(trigPin, OUTPUT); // Set trig pin as output
    pinMode(echoPin, INPUT); // Set echo pin as input
    pinMode(buzzPin, OUTPUT); // Set buzz pin as output
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    // Variable to store the duration and distance
    long duration;
    int distance;
    
    // Clear the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Set the trigPin high for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echoPin, pulseIn returns the duration in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance
    distance = (duration / 2) / 29.1;

    // Print the distance to the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // If the distance is less than the threshold, wait 2 seconds, then activate the buzzer
    if (distance <= distanceThreshold && distance > 0) {
        delay(2000); // Wait for 2 seconds
        digitalWrite(buzzPin, HIGH); // Turn on the buzzer
    } else {
        digitalWrite(buzzPin, LOW); // Turn off the buzzer
    }

    // Wait 500 milliseconds before the next loop
    delay(500);
}