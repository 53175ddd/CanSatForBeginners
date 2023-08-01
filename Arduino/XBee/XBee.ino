/*
XBEE_DIN  <--> 16
XBEE_DOUT <--> 17
*/

void setup() {
  Serial2.begin(9600);
  while(!Serial2);
}

void loop() {
  static uint8_t count = 0;
  Serial2.print("count = ");
  Serial2.println(count);
  count++;

  delay(1000);
}
