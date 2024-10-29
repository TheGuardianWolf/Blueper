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
  // std::unique_ptr<IScanner> scanner = std::make_unique<Scanner>();
  // std::unique_ptr<IBlinker> blinker = std::make_unique<Blinker>(LED_PIN);

  // Behaviour app(*advertisement, *scanner, *blinker);

  advertisement.start(&timing);
  scanner.start();
  blinker.setBlinkDelay(2000);

  Serial.println("Initialisation complete");
  // app.run(); // Blocks here

  while (true)
  {
    timing.loop();
    delay(50);
  }
}

void loop()
{
}
