#include <TEA5767.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// Initialize the TEA5767 radio
TEA5767 radio;

int delay_ms = 100;
float minFreq = 78.0;
float maxFreq = 108.0;
bool radioOn = true;

// LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

void setup()
{
  // Start the I2C communication
  Wire.begin();

  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set the initial frequency to 88.0 MHz
  radio.setFrequency(78.0);

  // Init the LCD screen
  lcd.begin(16, 2);
}

char pressButton(int pin)
{
  int x;
  x = analogRead(0);

  if (x < 60)
  {
    return 'U';
  }
  else if (x < 200)
  {
    return 'D';
  }
  else if (x < 400)
  {
    return 'L';
  }
  else if (x < 600)
  {
    return 'R';
  }
  else if (x < 800)
  {
    return 'S';
  }
}

void circleDelay()
{
  if (delay_ms < 450)
  {
    delay_ms = 50;
  }

  delay_ms += 50;
}

void controlPanel()
{
  char button = pressButton(0);

  if (button == 'U')
  {
    circleDelay();
  }
  else if (button == 'S')
  {
    radioOn = !radioOn;
    radio.setMuted(radioOn);
    if (!radioOn)
    {
      radio.setFrequency(minFreq);
    }
  }
}

void circleFrequency()
{
  if (radioOn)
  {
    // Set the radio to the current frequency
    radio.setFrequency(freq);

    // Print the current frequency to the serial monitor
    Serial.print("Current Frequency: ");
    Serial.println(freq);

    delay(delay_ms);
  }
}

void displayFrequency()
{
  // Clear the LCD screen
  lcd.clear();

  // Set the cursor to the first column of the first row
  lcd.setCursor(0, 0);

  // Print the current frequency to the LCD screen
  lcd.print("Frequency: ");
  lcd.print(freq);
}

void displayRadioStatus()
{
  // Set the cursor to the first column of the second row
  lcd.setCursor(0, 1);

  // Print the radio status to the LCD screen
  lcd.print(radioOn ? "Muted" : "UnMuted");
}

void loop()
{
  // Loop through the frequency range
  for (float freq = minFreq; freq <= maxFreq; freq += 0.1)
  {
    // Control the radio with the buttons
    controlPanel();

    circleFrequency();
  }
}
