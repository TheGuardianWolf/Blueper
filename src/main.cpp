#include <Arduino.h>

#include "Advertisement.h"
#include "Scanner.h"
#include "Blinker.h"
#include "Behaviour.h"
#include "Defines.h"

void setup()
{
  // Device specific setup
  Serial.begin(115200);
  Serial.println("Initialising...");

  auto &advertisement = Advertisement::create();
  // std::unique_ptr<IScanner> scanner = std::make_unique<Scanner>();
  // std::unique_ptr<IBlinker> blinker = std::make_unique<Blinker>(LED_PIN);

  // Behaviour app(*advertisement, *scanner, *blinker);

  advertisement.start();

  Serial.println("Initialisation complete");
  // app.run(); // Blocks here
}

void loop()
{
}
