#include <Enes100.h>

const int RX_PIN = 9;
const int TX_PIN = 8;
const int ARUCO_ID = 211;

void setup() {
  // put your setup code here, to run once:
  Enes100.begin("DATA!!!!", DATA, ARUCO_ID, TX_PIN, RX_PIN);

  Enes100.print("Destination is at (");
  Enes100.print(Enes100.missionSite.x);
  Enes100.print(", ");
  Enes100.print(Enes100.missionSite.y);
  Enes100.println(")");

  Enes100.mission(CYCLE, 7);
  Enes100.mission(MAGNETISM, NOT_MAGNETIC);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!Enes100.updateLocation()) {
      Enes100.println("404 Not Found");
  }
  Enes100.print("OTV is at (");
  Enes100.print(Enes100.location.x);
  Enes100.print(", ");
  Enes100.print(Enes100.location.y);
  Enes100.print(", ");
  Enes100.print(Enes100.location.theta);
  Enes100.println(")");
}
