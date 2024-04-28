#include <Mouse.h>

void setup() {
  Mouse.begin();

  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {

  if (Serial.available()) {
    String myString;
    int arr[2];

    myString = Serial.readString();

    String valX = getValue(myString, ':', 0);
    arr[0] = valX.toInt();

    String val2 = getValue(myString, ':', 1);
    String valY = getValue(val2, ';', 0);
    arr[1] = valY.toInt();

    String valC = getValue(myString, ';', 1);
    int pclick = valC.toInt();

    if (pclick == 1) {
      Mouse.click(MOUSE_LEFT);
    }
    if (arr[0] > 127 || arr[1] > 127 || arr[0] < -127 || arr[1] < -127) {

    } else {
      Mouse.move(arr[0], arr[1]);
    }
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
