#include <ArduinoSort.h>
#include <Servo.h>

// Servo related ---
Servo servo;  // create vertical servo object to control a servo
const int servo_pin = 10;

// -- Scan DEFAULT related
int scan_marginal = 10;
int distance_length = 18;
int distances[18];
int target = 0; // This is the angle for which the sentry has the closest object

// Ultrasound sensor
const int echoPin = 9;
const int trigPin = 8;

// https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

void setup() {
  // Servo
  servo.attach(servo_pin);
  servo.write(0);

  // Ultrasound
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Debug serial
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor


}

void loop() {

  /*for (int x = 180; x>0; x--) {
    servo.write(x);
    delay(10);
  }*/
  // The default stance - searching for objects
  scanForObjects();
  Serial.print("Closest object is at angle: ");
  Serial.println(checkForClosestDistance());

  //directionControl();
  //delay(1000);
  //speedControl();
  //delay(1000);
}


int readDistance() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  
  // Sets the trigPin HIGH (ACTIVE
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long sensor_duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  long sensor_distance = sensor_duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // Displays the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.print(sensor_distance);
  //Serial.println(" cm");

  // Return the distance
  return sensor_distance;
}


void scanForObjects() {
  // This function will loop through the entire 180 degrees view
  int array_place = 0;

  // Check which integer the loop should aim to reach
  if (servo.read() == 0) {
    // Start the scan loop upwards to 180
    for (int x = 0; x <= 180; x+=scan_marginal) {
        servo.write(x);
        int distance = readDistance();

        if (distance > 0) {
          distances[array_place] = distance;
        }
        array_place=x/10;
        delay(100);
    }
      


  } else {
    // Start the scan loop downards to 0
    for (int x = 180; x >= 0; x-=scan_marginal) {
        servo.write(x);
        int distance = readDistance();

        if (distance > 0) {
          distances[array_place] = distance;
        }        
        array_place=x/10;
        delay(100);

    }

  }

  
}

int checkForClosestDistance() {
  // Create a temporary replica of the distance array
  int temp_array[18];
  int place_in_temp_array = 0;
  for (int i = 0; i < distance_length; i++) {
    temp_array[i] = distances[i];
  }

  // Sort the replica
  sortArray(temp_array, distance_length);

  // Get the closest distance
  int closest_distance = temp_array[0];

  // Get the index of that value
  int current_index = 0;
  for (int n : distances) {
    if (n == closest_distance) {
      break;
    }
    current_index+=1;
  }
  
  // Finally return the index multiplied with 10 degrees to get the degrees
  return current_index*10;

}