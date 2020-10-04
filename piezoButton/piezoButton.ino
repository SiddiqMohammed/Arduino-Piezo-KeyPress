uint8_t buf[8] = { 0 };   //Keyboard report buffer

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int values = 0;
int Index = 0;

int valState = 0;
int lastValState = 0;

int valReadings[10];

int inputPin = A0;


void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  if (Index != 11) {
    Index = Index + 1;
  }
  else {
    values = average;
    Index = 0;
    valState = values;
  }

  if (average < 60) {
    values = 0;
    releaseKey();
  }
  else if (average > 90) {
    values = average;

    if (valState != lastValState) {
      buf[2] = 81;   // W keycode
      Serial.write(buf, 8); // Send keypress
      releaseKey();
    }
  }
  lastValState = valState;

  //  Serial.print(values);
  //  Serial.print(" ");
  //  Serial.println(average);
  delay(100);        // delay in between reads for stability
}

// Function for Key Release
void releaseKey()
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Send Release key
}
