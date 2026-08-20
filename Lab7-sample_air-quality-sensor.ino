#include "src/HeptaSat.h"
#include "src/drv/air_quality_mp503.h"

HeptaCdh cdh;
HeptaEps eps;
AirQualityMp503 air_quality;

// V4.1.1 payload ADC: USER1 / USER2 / USER3 -> MCP3208 CH5 / CH6 / CH7
constexpr uint8_t kUserChannel = 1;  // set to 1, 2, or 3
constexpr uint8_t kMcp3208CsPin = 17;
constexpr uint8_t kWarmupSeconds = 20;

constexpr uint8_t mcp_channel_from_user(uint8_t user) {
  return static_cast<uint8_t>(4 + user);
}

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

  if (kUserChannel < 1 || kUserChannel > 3) {
    cdh.println("kUserChannel must be 1, 2, or 3");
    while (true) {
      delay(1000);
    }
  }

  cdh.println("Warming up air quality sensor...");
  for (uint8_t sec = 1; sec <= kWarmupSeconds; sec++) {
    cdh.printf("Warming up... %u/%u s\r\n", sec, kWarmupSeconds);
    delay(1000);
  }

  const uint8_t mcp_channel = mcp_channel_from_user(kUserChannel);
  // skip_warmup=true: the countdown loop above already waited kWarmupSeconds
  if (!air_quality.begin(kMcp3208CsPin, mcp_channel, 3.3f, true)) {
    cdh.println("Air quality sensor init failed");
    while (true) {
      delay(1000);
    }
  }

  cdh.printf("Air quality sensor ready (USER%u / MCP3208 ch%u)\r\n",
             kUserChannel, mcp_channel);
}

void loop() {
  AirQualityMp503::QualityLevel level = air_quality.slope();
  uint16_t raw = air_quality.get_raw();

  cdh.printf("Air quality: %u (%s)\r\n", raw, quality_to_string(level));

  delay(1000);
}
