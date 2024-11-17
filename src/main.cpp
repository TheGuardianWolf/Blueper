#include <Arduino.h>

#include "Advertisement.h"
#include "Scanner.h"
#include "Blinker.h"
#include "Timing.h"
#include "Behaviour.h"
#include "Defines.h"

void setup()
{
  // Device specific setup
  Serial.begin(115200);
  Serial.println("Initialising...");

  auto &timing = Timing::create();
  auto &advertisement = Advertisement::create();
  auto &scanner = Scanner::create();
  Blinker blinker(timing, LED_PIN);

  Behaviour app(timing, advertisement, scanner, blinker);

  Serial.println("Initialisation complete");
  app.start();

  while (true)
  {
    timing.loop();
    delay(LOOP_TIME);
  }
}

void loop()
{
}
