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

  std::shared_ptr<IAdvertisement> advertisement = std::make_shared<Advertisement>(1, 1, 20);
  std::shared_ptr<IScanner> scanner = std::make_shared<Scanner>();
  std::shared_ptr<IBlinker> blinker = std::make_shared<Blinker>(LED_PIN);

  Behaviour app(advertisement, scanner, blinker);
  app.run(); // Blocks here
}

void loop()
{
}
