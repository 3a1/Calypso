#include <Mouse.h>

void setup() 
{
  Mouse.begin();
  Serial.begin(9600);
}

void loop() 
{
  while(Serial.available())
  {
    int x = 10;
    String input = Serial.readStringUntil('\0');
    x = input.toInt();
    if(x < 0)
    {
      Mouse.press(MOUSE_LEFT);
      delay(3);
      Mouse.release(MOUSE_LEFT);
      delay(1);
    }
  }
}
