#define CYCLE_INPUT_PIN 8

void setup() {
  // put your setup code here, to run once:
  pinMode(CYCLE_INPUT_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("Duty Cycle: ");
  Serial.println(readDutyCycle());
}

int readDutyCycle() {
  delay(1000);
  
  int highCount = 0;
  int lowCount = 0;
  int totalCount = 1000;

  for(int i = 0; i < totalCount; i++) {
    if (digitalRead(CYCLE_INPUT_PIN) == HIGH) {
      highCount++;
    } else {
      lowCount++;
    }
    delay(1);
  }

  float percent = (float) highCount / totalCount;

  if (percent <= .2) {
    return 1;
  } else if (percent <= .4) {
    return 3;
  } else if (percent <= .6) {
    return 5;
  } else if (percent <= .8) {
    return 7;
  } else {
    return 9;
  }
}
