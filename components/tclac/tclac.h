#pragma once

#include "esphome.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace tclac {

class TCLAC : public climate::Climate, public uart::UARTDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

  // دالة ضبط وضع المولد وتثبيته في ذاكرة ESP32 مع الإرسال الفوري
  void set_gen_mode(uint8_t mode) {
    this->gen_mode_ = mode;
    this->send_data();
  }

  uint8_t get_gen_mode() const { return this->gen_mode_; }

 private:
  void send_data();
  void read_data();
  
  // متغير حفظ وضع المولد الأصلي بداخل الذاكرة
  uint8_t gen_mode_{0x00};
};

}  // namespace tclac
}  // namespace esphome
