enum SignalState {
  OFF,
  SHORT_SIGNAL,
  LONG_SIGNAL
};

// Left Signal Components
const int buttonPinLeft = 4;     // Left button on GPIO4
const int ledPinLeft = 5;        // Left LED on GPIO5 (active LOW)
unsigned long leftPressTime = 0;
SignalState leftState = OFF;
int leftSignalCount = 0;
unsigned long leftLastBlinkTime = 0;
bool leftLedState = HIGH;        // Start with LED off

// Right Signal Components
const int buttonPinRight = 18;    // Right button on GPI18
const int ledPinRight = 19;      // Right LED on GPIO19 (active LOW)
unsigned long rightPressTime = 0;
SignalState rightState = OFF;
int rightSignalCount = 0;
unsigned long rightLastBlinkTime = 0;
bool rightLedState = HIGH;       // Start with LED off

void setup() {
  // Left signal setup
  pinMode(buttonPinLeft, INPUT_PULLUP);
  pinMode(ledPinLeft, OUTPUT);
  digitalWrite(ledPinLeft, HIGH); // Start with left LED off

  // Right signal setup
  pinMode(buttonPinRight, INPUT_PULLUP);
  pinMode(ledPinRight, OUTPUT);
  digitalWrite(ledPinRight, HIGH); // Start with right LED off
}


void cancelLeftSignal() {
  leftState = OFF;
  digitalWrite(ledPinLeft, HIGH);
  leftPressTime = 0;
  leftSignalCount = 0;
}

void cancelRightSignal() {
  rightState = OFF;
  digitalWrite(ledPinRight, HIGH);
  rightPressTime = 0;
  rightSignalCount = 0;
}




void loop() {
  /* ===== LEFT SIGNAL CODE ===== */
  int leftButtonState = digitalRead(buttonPinLeft);
  
  // Left button press handling
  if (leftButtonState == LOW) {
    if (leftPressTime == 0) {
      leftPressTime = millis();
      if (rightState != OFF) {
        cancelRightSignal();
      }
    }
  } else {
    if (leftPressTime > 0) {
      unsigned long duration = millis() - leftPressTime;
      
      if (duration < 700 && leftState == OFF) {
        leftState = SHORT_SIGNAL;
        leftSignalCount = 0;
        leftLastBlinkTime = millis();
      } else if (duration > 700 && leftState == OFF) {
        leftState = LONG_SIGNAL;
        leftLastBlinkTime = millis();
      } else if (leftState != OFF) {
        cancelLeftSignal();
      }
      leftPressTime = 0;
    }
  }

  // Left signal state management
  switch (leftState) {
    case SHORT_SIGNAL:
      if (leftSignalCount < 7) {
        if (millis() - leftLastBlinkTime >= 500) {
          leftLastBlinkTime = millis();
          leftLedState = !leftLedState;
          digitalWrite(ledPinLeft, leftLedState);
          leftSignalCount++;
        }
      } else {
        cancelLeftSignal();
      }
      break;

    case LONG_SIGNAL:
      if (millis() - leftLastBlinkTime >= 500) {
        leftLastBlinkTime = millis();
        leftLedState = !leftLedState;
        digitalWrite(ledPinLeft, leftLedState);
      }
      break;

    case OFF:
      break;
  }

  /* ===== RIGHT SIGNAL CODE ===== */
  int rightButtonState = digitalRead(buttonPinRight);
  
  // Right button press handling
  if (rightButtonState == LOW) {
    if (rightPressTime == 0) {
      rightPressTime = millis();
      if (leftState != OFF) {
        cancelLeftSignal();
      }
    }
  } else {
    if (rightPressTime > 0) {
      unsigned long duration = millis() - rightPressTime;
      
      if (duration < 700 && rightState == OFF) {
        rightState = SHORT_SIGNAL;
        rightSignalCount = 0;
        rightLastBlinkTime = millis();
      } else if (duration > 700 && rightState == OFF) {
        rightState = LONG_SIGNAL;
        rightLastBlinkTime = millis();
      } else if (rightState != OFF) {
        cancelRightSignal();
      }
      rightPressTime = 0;
    }
  }

  // Right signal state management
  switch (rightState) {
    case SHORT_SIGNAL:
      if (rightSignalCount < 7) {
        if (millis() - rightLastBlinkTime >= 500) {
          rightLastBlinkTime = millis();
          rightLedState = !rightLedState;
          digitalWrite(ledPinRight, rightLedState);
          rightSignalCount++;
        }
      } else {
        cancelRightSignal();
      }
      break;

    case LONG_SIGNAL:
      if (millis() - rightLastBlinkTime >= 500) {
        rightLastBlinkTime = millis();
        rightLedState = !rightLedState;
        digitalWrite(ledPinRight, rightLedState);
      }
      break;

    case OFF:
      break;
  }
}