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
  traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_DRY,
      climate::CLIMATE_MODE_FAN_ONLY,
  });
  traits.set_visual_min_temperature(16.0);
  traits.set_visual_max_temperature(31.0);
  traits.set_visual_temperature_step(1.0);
  return traits;
}

void TCLAC::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
  }
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
  }
  if (call.get_fan_mode().has_value()) {
    this->fan_mode = *call.get_fan_mode();
  }

  // إرسال الحزمة المحدثة بالحرارة والمولد مع تحديث الواجهة
  this->send_data();
  this->publish_state();
}

void TCLAC::send_data() {
  // التأكد من حدود درجة الحرارة (بين 16 و 31)
  uint8_t temp = (uint8_t) this->target_temperature;
  if (temp < 16) temp = 16;
  if (temp > 31) temp = 31;

  // بناء حزمة البيانات الخاصة بمكيفك (الخانة 33 للحرارة، 34 للمولد)
  uint8_t cmd[38] = {
    0xBB, 0x00, 0x01, 0x03, 0x20, 0x03, 0x01, 0x64,
    0x03, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, temp, this->gen_mode_, 0x00, 0x00, 0x00
  };

  // إذا تم إيقاف المكيف من الهاتف
  if (this->mode == climate::CLIMATE_MODE_OFF) {
    cmd[8] = 0x00;
  }

  // حساب المجموع التفقدي بالجمع المباشر حسب شفرة جهازك
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
