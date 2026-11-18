#include "src/HeptaSat.h"

HeptaCdh cdh;
HeptaEps eps;
HeptaAirQualityMp503 air_quality;

constexpr uint8_t kWarmupSeconds = 20;

const char *quality_to_string(HeptaAirQualityMp503::QualityLevel level) {
  switch (level) {
    case HeptaAirQualityMp503::FORCE_SIGNAL:
      return "High pollution (force signal)";
    case HeptaAirQualityMp503::HIGH_POLLUTION:
      return "High pollution";
    case HeptaAirQualityMp503::LOW_POLLUTION:
      return "Low pollution";
    case HeptaAirQualityMp503::FRESH_AIR:
    default:
      return "Fresh air";
  }
}

void setup() {
  cdh.begin();
  eps.init();
  eps.switch_3V3_on();

  cdh.println("Warming up air quality sensor...");
  for (uint8_t sec = 1; sec <= kWarmupSeconds; sec++) {
    cdh.printf("Warming up... %u/%u s\r\n", sec, kWarmupSeconds);
    delay(1000);
  }

  if (!air_quality.begin_after_warmup(USER1)) {
    cdh.println("Air quality sensor init failed");
    while (true) {
      delay(1000);
    }
  }

  cdh.println("Air quality sensor ready");
}

void loop() {
  HeptaAirQualityMp503::QualityLevel level = air_quality.slope();
  uint16_t raw = air_quality.get_raw();

  cdh.printf("Air quality: %u (%s)\r\n", raw, quality_to_string(level));

  delay(1000);
}
