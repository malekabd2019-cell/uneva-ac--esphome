#pragma once
#include "esphome.h"

class TCLACCustom : public esphome::climate::Climate, public esphome::uart::UARTDevice, public esphome::Component {
 public:
  uint8_t gen_mode = 0x00;

  void setup() override {
    this->target_temperature = 24.0;
    this->mode = esphome::climate::CLIMATE_MODE_COOL;
  }

  esphome::climate::ClimateTraits traits() override {
    auto traits = esphome::climate::ClimateTraits();
    traits.set_supports_current_temperature(false);
    traits.set_visual_min_temperature(16.0);
    traits.set_visual_max_temperature(31.0);
    traits.set_visual_temperature_step(1.0);
    traits.set_supported_modes({
      esphome::climate::CLIMATE_MODE_OFF,
      esphome::climate::CLIMATE_MODE_COOL,
      esphome::climate::CLIMATE_MODE_HEAT,
      esphome::climate::CLIMATE_MODE_FAN_ONLY,
      esphome::climate::CLIMATE_MODE_DRY
    });
    return traits;
  }

  void control(const esphome::climate::ClimateCall &call) override {
    if (call.get_mode().has_value()) {
      this->mode = *call.get_mode();
    }
    if (call.get_target_temperature().has_value()) {
      this->target_temperature = *call.get_target_temperature();
    }
    this->send_data();
    this->publish_state();
  }

  void set_gen_mode(uint8_t mode_val) {
    this->gen_mode = mode_val;
    this->send_data();
  }

  void send_data() {
    uint8_t mode_byte = 0x03;
    if (this->mode == esphome::climate::CLIMATE_MODE_OFF) mode_byte = 0x00;
    else if (this->mode == esphome::climate::CLIMATE_MODE_COOL) mode_byte = 0x03;
    else if (this->mode == esphome::climate::CLIMATE_MODE_HEAT) mode_byte = 0x01;
    else if (this->mode == esphome::climate::CLIMATE_MODE_FAN_ONLY) mode_byte = 0x07;
    else if (this->mode == esphome::climate::CLIMATE_MODE_DRY) mode_byte = 0x02;

    uint8_t target_temp = (uint8_t)this->target_temperature;
    if (target_temp < 16) target_temp = 16;
    if (target_temp > 31) target_temp = 31;

    uint8_t cmd[38] = {
      0xBB, 0x00, 0x01, 0x03, 0x20, 0x03, 0x01, 0x64,
      0x03, mode_byte, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x0D, target_temp, this->gen_mode, 0x00, 0x00, 0x00
    };

    uint8_t sum = 0;
    for (int i = 3; i < 37; i++) {
      sum += cmd[i];
    }
    cmd[37] = sum;

    this->write_array(cmd, 38);
  }

  void loop() override {
    while (this->available()) {
      uint8_t b;
      this->read_byte(&b);
    }
  }
};
