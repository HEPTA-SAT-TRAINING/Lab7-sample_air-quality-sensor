#include "src/HeptaSat.h"
#include "src/drv/air_quality_mp503.h"

HeptaCdh cdh;
HeptaEps eps;
AirQualityMp503 air_quality;

// true: MCP3208 channel 6, false: MCU GP28 (ADC2)
constexpr bool kUseMcp3208 = true;
constexpr uint8_t kMcp3208CsPin = 17;
constexpr uint8_t kMcp3208Channel = 6;
constexpr uint8_t kDirectAdcPin = 28;
constexpr uint8_t kWarmupSeconds = 20;

const char *quality_to_string(AirQualityMp503::QualityLevel level) {
  switch (level) {
    case AirQualityMp503::FORCE_SIGNAL:
      return "High pollution (force signal)";
    case AirQualityMp503::HIGH_POLLUTION:
      return "High pollution";
    case AirQualityMp503::LOW_POLLUTION:
      return "Low pollution";
    case AirQualityMp503::FRESH_AIR:
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

  // skip_warmup=true: the countdown loop above already waited kWarmupSeconds
  if (!air_quality.begin(kUseMcp3208, kDirectAdcPin, kMcp3208Channel, kMcp3208CsPin,
                         3.3f, true)) {
    cdh.println("Air quality sensor init failed");
    while (true) {
      delay(1000);
    }
  }

  if (kUseMcp3208) {
    cdh.printf("Air quality sensor ready (MCP3208 ch%u)\r\n", kMcp3208Channel);
  } else {
    cdh.println("Air quality sensor ready (GP28)");
  }
}

void loop() {
  AirQualityMp503::QualityLevel level = air_quality.slope();
  uint16_t raw = air_quality.get_raw();

  cdh.printf("Air quality: %u (%s)\r\n", raw, quality_to_string(level));

  delay(1000);
}
