#include "mcp9600_noid.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mcp9600_noid {

static const char *const TAG = "mcp9600_noid";

static const uint8_t REG_HOT_JUNCTION = 0x00;
static const uint8_t REG_COLD_JUNCTION = 0x02;
static const uint8_t REG_SENSOR_CONFIG = 0x05;

void MCP9600NoIdComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up MCP9600 (no device ID check)...");

  // Some MCP9600 chips need time after power-on before accepting writes
  delay(100);

  uint8_t config = static_cast<uint8_t>(this->thermocouple_type_) << 4;
  if (!this->write_byte(REG_SENSOR_CONFIG, config)) {
    // Don't mark failed — chip defaults to K-type anyway and reads may still work
    ESP_LOGW(TAG, "Config register write failed (chip may still work with defaults)");
  } else {
    ESP_LOGI(TAG, "MCP9600 configured OK (device ID check skipped)");
  }

  // Verify we can actually read from the chip
  uint8_t data[2];
  if (this->read_register(REG_HOT_JUNCTION, data, 2) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Cannot read from MCP9600 - check wiring");
    this->mark_failed();
    return;
  }

  ESP_LOGI(TAG, "MCP9600 read test passed");
}

void MCP9600NoIdComponent::update() {
  if (this->hot_junction_sensor_ != nullptr) {
    uint8_t data[2];
    if (this->read_register(REG_HOT_JUNCTION, data, 2) == i2c::ERROR_OK) {
      int16_t raw = (static_cast<int16_t>(data[0]) << 8) | data[1];
      float temp_c = raw * 0.0625f;
      this->hot_junction_sensor_->publish_state(temp_c);
    } else {
      ESP_LOGW(TAG, "Hot junction read failed");
      this->hot_junction_sensor_->publish_state(NAN);
    }
  }

  if (this->cold_junction_sensor_ != nullptr) {
    uint8_t data[2];
    if (this->read_register(REG_COLD_JUNCTION, data, 2) == i2c::ERROR_OK) {
      int16_t raw = (static_cast<int16_t>(data[0]) << 8) | data[1];
      float temp_c = raw * 0.0625f;
      this->cold_junction_sensor_->publish_state(temp_c);
    } else {
      ESP_LOGW(TAG, "Cold junction read failed");
      this->cold_junction_sensor_->publish_state(NAN);
    }
  }
}

void MCP9600NoIdComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "MCP9600 (no ID check):");
  ESP_LOGCONFIG(TAG, "  Thermocouple type: %u", this->thermocouple_type_);
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Hot Junction", this->hot_junction_sensor_);
  LOG_SENSOR("  ", "Cold Junction", this->cold_junction_sensor_);
}

}  // namespace mcp9600_noid
}  // namespace esphome
