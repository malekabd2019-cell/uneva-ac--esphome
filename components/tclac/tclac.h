#ifndef TCLAC_H
#define TCLAC_H

#include "esphome.h"
#include "esphome/core/defines.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace tclac {

// تعريفات الأوضاع
#define MODE_POS         8
#define MODE_MASK        0x0F
#define MODE_AUTO        0x08
#define MODE_COOL        0x03
#define MODE_DRY         0x02
#define MODE_FAN_ONLY    0x07
#define MODE_HEAT        0x01

#define FAN_SPEED_POS    9
#define FAN_SPEED_MASK   0x07
#define FAN_AUTO         0x00
#define FAN_LOW          0x01
#define FAN_MIDDLE       0x06
#define FAN_MEDIUM       0x03
#define FAN_HIGH         0x07
#define FAN_FOCUS        0x05
#define FAN_QUIET_POS    7
#define FAN_QUIET        0x80
#define FAN_DIFFUSE      0x10

#define SWING_POS        10
#define SWING_MODE_MASK  0x38
#define SWING_OFF        0x00
#define SWING_VERTICAL   0x08
#define SWING_HORIZONTAL 0x10
#define SWING_BOTH       0x18

#define SET_TEMP_MASK    0x0F

// Enums
enum class AirflowVerticalDirection {
  LAST,
  MAX_UP,
  UP,
  CENTER,
  DOWN,
  MAX_DOWN
};

enum class AirflowHorizontalDirection {
  LAST,
  MAX_LEFT,
  LEFT,
  CENTER,
  RIGHT,
  MAX_RIGHT
};

enum class VerticalSwingDirection {
  UP_DOWN,
  UPSIDE,
  DOWNSIDE
};

enum class HorizontalSwingDirection {
  LEFT_RIGHT,
  LEFTSIDE,
  CENTER,
  RIGHTSIDE
};

class tclacClimate : public Component, public uart::UARTDevice, public climate::Climate, public PollingComponent {
 public:
  tclacClimate() : PollingComponent(5000) {}  // استطلاع كل 5 ثوانٍ

  void setup() override;
  void loop() override;
  void update() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;

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

  // وضع المولد
  void set_generator_level(int level);

 protected:
  void readData();
  void takeControl();
  void sendData(byte *message, byte size);
  String getHex(byte *message, byte size);
  byte getChecksum(const byte *message, size_t size);
  void dataShow(bool flow, bool shine);

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

  byte dataTX[38] = {0};
  byte dataRX[62] = {0};
  byte poll[19] = {0xBB, 0x00, 0x01, 0x04, 0x19, 0x03, 0x01, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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
