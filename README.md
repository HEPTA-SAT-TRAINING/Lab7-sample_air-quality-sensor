# Lab7-sample_air-quality-sensor

HEPTA-SAT training sample: **Grove Air Quality Sensor v1.3** (Winsen **MP503**).

## Sensor Specifications

| Item | Value |
|------|-------|
| Module | **Seeed Grove Air Quality Sensor v1.3** |
| Gas sensor | **Winsen MP503** (semiconductor type) |
| Interface | Analog output → **MCP3208 USER1 / USER2 / USER3** (V4.1.1) |
| Supply | 3.3 to 5 V (Grove module) |
| Detects | Alcohol, smoke, isobutane, formaldehyde, etc. |
| Warm-up | approx. 20 s (countdown printed in the sketch) |
| Classification | Seeed `slope()` algorithm (thresholds scaled ×4 for 12-bit) |

## Analog Input Selection

Set `kUserChannel` in the sketch to match the board USER pin:

| `kUserChannel` | Board pin | MCP3208 |
|----------------|-----------|---------|
| `1` | **USER1** | CH5 |
| `2` | **USER2** | CH6 |
| `3` | **USER3** | CH7 |

## Wiring (HEPTA-SAT V4.1.1)

| Sensor | HEPTA-SAT |
|--------|-----------|
| Analog output | USER1 / USER2 / USER3 (see `kUserChannel`) |
| VCC | 3.3 V payload (`eps.switch_3V3_on()`) |
| GND | GND |

## Build and Upload

1. Open `Lab7-sample_air-quality-sensor.ino` in the Arduino IDE
2. Board: **Generic RP2350**
3. Check out the `src/` submodule (`git submodule update --init`)
4. Set `kUserChannel` to match your wiring (`1`, `2`, or `3`)
5. Compile and upload
6. Serial monitor at **9600 baud**

## Example Serial Output

```
Warming up air quality sensor...
Warming up... 1/20 s
Warming up... 2/20 s
...
Warming up... 20/20 s
Air quality sensor ready (USER1 / MCP3208 ch5)
Air quality: 512 (Fresh air)
Air quality: 384 (Low pollution)
```

## Driver

- `src/drv/air_quality_mp503.h` / `.cpp`
- Details: `src/docs/drivers/air-quality-mp503.md`
