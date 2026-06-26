#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mcp9600_noid {

enum ThermocoupleType : uint8_t {
  THERMOCOUPLE_TYPE_K = 0b000,
  THERMOCOUPLE_TYPE_J = 0b001,
  THERMOCOUPLE_TYPE_T = 0b010,
  THERMOCOUPLE_TYPE_N = 0b011,
  THERMOCOUPLE_TYPE_S = 0b100,
  THERMOCOUPLE_TYPE_E = 0b101,
  THERMOCOUPLE_TYPE_B = 0b110,
  THERMOCOUPLE_TYPE_R = 0b111,
};

class MCP9600NoIdComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_thermocouple_type(ThermocoupleType type) { thermocouple_type_ = type; }
  void set_hot_junction_sensor(sensor::Sensor *s) { hot_junction_sensor_ = s; }
  void set_cold_junction_sensor(sensor::Sensor *s) { cold_junction_sensor_ = s; }

 protected:
  ThermocoupleType thermocouple_type_{THERMOCOUPLE_TYPE_K};
  sensor::Sensor *hot_junction_sensor_{nullptr};
  sensor::Sensor *cold_junction_sensor_{nullptr};
};

}  // namespace mcp9600_noid
}  // namespace esphome
