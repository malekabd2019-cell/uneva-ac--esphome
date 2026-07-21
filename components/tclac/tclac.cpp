#include "tclac.h"

namespace esphome {
namespace tclac {

static const char *const TAG = "tclac";

void TCLAC::setup() {}

void TCLAC::loop() {
  this->read_data();
}

void TCLAC::dump_config() {
  ESP_LOGCONFIG(TAG, "TCL AC Custom Component with Native Gen Mode");
}

climate::ClimateTraits TCLAC::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_visual_min_temperature(16.0);
  traits.set_visual_max_temperature(31.0);
  traits.set_visual_temperature_step(1.0);
  return traits;
}

void TCLAC::control(const climate::ClimateCall &call) {
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
  }
  this->send_data();
}

void TCLAC::send_data() {
  // بناء الحزمة مع تضمين قيمة وضع المولد المحفوظة في الخانة 34
  uint8_t cmd[38] = {
    0xBB, 0x00, 0x01, 0x03, 0x20, 0x03, 0x01, 0x64,
    0x03, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, (uint8_t)this->target_temperature, this->gen_mode_, 0x00, 0x00, 0x00
  };

  // حساب المجموع التفقدي Checksum
  uint8_t sum = 0;
  for (int i = 0; i < 37; i++) {
    sum += cmd[i];
  }
  cmd[37] = sum;

  this->write_array(cmd, 38);
}

void TCLAC::read_data() {
  while (this->available()) {
    uint8_t b;
    this->read_byte(&b);
  }
}

}  // namespace tclac
}  // namespace esphome
