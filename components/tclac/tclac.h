#ifndef TCLAC_H
#define TCLAC_H

#include "esphome.h"
#include "esphome/core/defines.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace tclac {

// تعريفات الأنماط (يجب أن تتطابق مع ملف .cpp)
class tclacClimate : public Component, public uart::UARTDevice, public climate::Climate {
 public:
  void setup() override;
  void loop() override;
  void update() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;

  // دوال إعدادات خارجية
  void set_beeper_state(bool state);
  void set_display_state(bool state);
  void set_force_mode_state(bool state);
#ifdef CONF_RX_LED
  void set_rx_led_pin(GPIOPin *rx_led_pin);
#endif
#ifdef CONF_TX_LED
  void set_tx_led_pin(GPIOPin *tx_led_pin);
#endif
  void set_module_display_state(bool state);
  void set_vertical_airflow(AirflowVerticalDirection direction);
  void set_horizontal_airflow(AirflowHorizontalDirection direction);
  void set_vertical_swing_direction(VerticalSwingDirection direction);
  void set_horizontal_swing_direction(HorizontalSwingDirection direction);

  // ====== إضافة وضع المولد والأمبير ======
  void set_generator_level(int level);
  void set_ampere_sensor(sensor::Sensor *sensor); // إن أردت ربط مستشعر خارجي (اختياري)

 protected:
  void readData();
  void takeControl();
  void sendData(byte *message, byte size);
  String getHex(byte *message, byte size);
  byte getChecksum(const byte *message, size_t size);
  void dataShow(bool flow, bool shine);

  // متغيرات الحالة
  bool beeper_status_{true};
  bool display_status_{true};
  bool force_mode_status_{false};
  bool module_display_status_{true};
  bool allow_take_control{true};
  bool is_call_control{false};

  AirflowVerticalDirection vertical_direction_{AirflowVerticalDirection::LAST};
  AirflowHorizontalDirection horizontal_direction_{AirflowHorizontalDirection::LAST};
  VerticalSwingDirection vertical_swing_direction_{VerticalSwingDirection::UP_DOWN};
  HorizontalSwingDirection horizontal_swing_direction_{HorizontalSwingDirection::LEFT_RIGHT};

  climate::ClimateMode switch_climate_mode{climate::CLIMATE_MODE_OFF};
  climate::ClimateFanMode switch_fan_mode{climate::CLIMATE_FAN_AUTO};
  climate::ClimateSwingMode switch_swing_mode{climate::CLIMATE_SWING_OFF};
  climate::ClimatePreset switch_preset{climate::CLIMATE_PRESET_NONE};
  int target_temperature_set{0};

  // مصفوفات الإرسال والاستقبال
  byte dataTX[38] = {0};
  byte dataRX[62] = {0};
  byte poll[19] = {0xBB, 0x00, 0x01, 0x04, 0x19, 0x03, 0x01, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  // ====== إضافة متغيرات المولد والأمبير ======
  int generator_level_{0};
  sensor::Sensor *ampere_sensor_{nullptr}; // مؤشر لمستشعر التيار

#ifdef CONF_RX_LED
  GPIOPin *rx_led_pin_{nullptr};
#endif
#ifdef CONF_TX_LED
  GPIOPin *tx_led_pin_{nullptr};
#endif
};

}  // namespace tclac
}  // namespace esphome
#endif
