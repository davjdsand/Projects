#include <LiquidCrystal_I2C.h>

// LCD Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed

const int HALL_PIN = 12;           // GPIO12 for Hall sensor
const float WHEEL_CIRCUMFERENCE = 1.81; // Wheel circumference in meters
const unsigned long BAUD_RATE = 115200;

// Detection tuning
const unsigned long DEBOUNCE_US = 5000;    // 5ms debounce
const float MAX_REALISTIC_SPEED = 60.0;    // km/h
const unsigned long LCD_UPDATE_INTERVAL = 200; // Update LCD every 200ms
const float SMOOTHING_FACTOR = 0.3;       // For speed smoothing (0.1-0.5)

volatile unsigned long lastDetectionTime = 0;
volatile unsigned long rotationInterval = 0;
volatile bool newReading = false;
unsigned long lastLcdUpdate = 0;
float currentSpeedKmh = 0.0;
float displayedSpeedKmh = 0.0;

void IRAM_ATTR handleMagnetDetection() {
  unsigned long currentTime = micros();
  unsigned long timeSinceLast = currentTime - lastDetectionTime;
  
  if (timeSinceLast < DEBOUNCE_US) return;
  
  rotationInterval = timeSinceLast;
  lastDetectionTime = currentTime;
  newReading = true;
}

void setup() {
  Serial.begin(BAUD_RATE);
  
  // Initialize LCD (only once)
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Print static labels (only once)
  lcd.setCursor(0, 0);
  lcd.print("Bike Speed:");
  lcd.setCursor(10, 1);
  lcd.print("Km/h");

  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), handleMagnetDetection, FALLING);
  
  Serial.println("\nBike Speedometer Ready");
}

void loop() {
  if (newReading) {
    newReading = false;
    
    if (rotationInterval > 0) {
      float speed_mps = WHEEL_CIRCUMFERENCE / (rotationInterval / 1000000.0);
      float newSpeedKmh = speed_mps * 3.6;
      
      // Apply speed filtering
      if (newSpeedKmh <= MAX_REALISTIC_SPEED) {
        // Smooth the speed value
        currentSpeedKmh = (currentSpeedKmh * (1 - SMOOTHING_FACTOR)) + (newSpeedKmh * SMOOTHING_FACTOR);
        
        Serial.print("Speed: ");
        Serial.print(currentSpeedKmh, 1);
        Serial.println(" km/h");
      }
    }
  }
  
  // Update only the speed number on LCD
  if (millis() - lastLcdUpdate >= LCD_UPDATE_INTERVAL) {
    lastLcdUpdate = millis();
    
    // Only update the speed digits (position 0,1 on second row)
    lcd.setCursor(0, 1);
    lcd.print(currentSpeedKmh, 1);
    lcd.print(" "); // Clear any leftover digits
  }
  
  delay(10);
}