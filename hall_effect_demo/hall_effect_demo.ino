#define HALL_SENSOR_A A0
#define HALL_SENSOR_D 4


void setup() {
  // put your setup code here, to run once:
  pinMode(HALL_SENSOR_D, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int analogReading = analogRead(HALL_SENSOR_A);
  bool digitalReading = digitalRead(HALL_SENSOR_D);
  Serial.print("Hall Sensor Digital: ");
  Serial.print(digitalReading);
  Serial.print(", Analog Reading: ");
  Serial.print(analogReading);
  if (analogReading >= 533 || digitalReading == HIGH) {
    Serial.print(", MAGNETIC");
  } else {
    Serial.print(", NOT MAGNETIC");
  }
  Serial.println();
  delay(1000);
}
