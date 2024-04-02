

#include <Servo.h> // Include the Servo library to control servo motors.
#include "animationTools.h"

// Pin assignments
int servoPin = 14; // Servo motor control pin connected to pin A0 on the Arduino.
int buttonPin = 15; // Button pin connected to digital pin A1 on the Arduino.

// Servo positions for different states
int stopValue = 94; // Value to stop the servo
int state1angle; // Servo position angle for state 1.

// Current state of the system
bool currentState = 0; // The starting state of the system. (0 by default)

// Button state variables
bool buttonCurrent; // Stores the current read value of the button.
bool buttonPrev = true; // Stores the previous read value of the button to detect state changes.

// Servo object
Servo notifier; // Create a Servo object to control the servo motor.


//Create a Timeline
Timeline state1Rotation;
Timeline state0Rotation;



void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second.
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as an input with an internal pull-up resistor.
  notifier.attach(servoPin); // Attach the servo motor to the servo pin.

  //add keyframes to your timelineVariable
  state1Rotation.addKey(0, 180);     //rotate counterclockwise
  state1Rotation.addKey(1000, 180);  //do that for 1 second
  state1Rotation.addKey(1001, stopValue); //stop 
  state1Rotation.addKey(1300, stopValue); //hold for 300ms
  state1Rotation.addKey(1301, 50); //rotate back in the other direction at half speed
  state1Rotation.addKey(2000, 50);   //do that for 700 ms
  state1Rotation.addKey(2001, stopValue); //stop
  //by default the timelines loop, but you can turn that off with
  state1Rotation.loop = false;

 
  state0Rotation.addKey(0, stopValue);
  state0Rotation.addKey(1000,50);
  state0Rotation.addKey(2500, stopValue);
  state0Rotation.addKey(4500, 120);
  state0Rotation.addKey(5500, stopValue);
}

void loop() {
  determineState(); // Determine the current state based on the button input.
  Serial.println(currentState); // Output the current state to the serial monitor.
  
  // Move the servo based on the current state
  switch (currentState) {
    case 0:
      stopValue = getTimelineValue(state0Rotation);
      notifier.write(stopValue); // Set the servo to the angle for state 0.
      break;
    case 1:
      state1angle = getTimelineValue(state1Rotation);
      notifier.write(state1angle); // Set the servo to the angle for state 1.
      break;
  }
}

// Function to determine the current state based on button input
void determineState() {
    buttonCurrent = digitalRead(buttonPin); // Read the current state of the button.

    if (buttonCurrent == 1 && buttonPrev == 0) 
    {
        if (currentState == 0) 
        {
            currentState = 1; // Toggle to state 1
            resetTimeline(state1Rotation); // Reset the timeline before playing
        }
        else if (currentState == 1) 
        {
            currentState = 0; // Toggle to state 1
            resetTimeline(state0Rotation); // Reset the timeline before playing
        }
        
    }

    if (currentState == 1 && state1Rotation.hasCompleted) {
        // If in state 1 and the timeline has completed, reset to state 0
       resetTimeline(state0Rotation);
        currentState = 0;
    }

    buttonPrev = buttonCurrent; // Update the previous button state for the next loop iteration.
}




