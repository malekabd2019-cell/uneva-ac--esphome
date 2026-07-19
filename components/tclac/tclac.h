#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace tclac {

enum class AirflowVerticalDirection : uint8_t {
  LAST = 0,
  MAX_UP = 1,
  UP = 2,
  CENTER = 3,
  DOWN = 4,
  MAX_DOWN = 5,
};

enum class AirflowHorizontalDirection : uint8_t {
  LAST = 0,
  MAX_LEFT = 1,
  LEFT = 2,
  CENTER = 3,
  RIGHT = 4,
  MAX_RIGHT = 5,
};

enum class VerticalSwingDirection : uint8_t {
  UP_DOWN = 1,
  UPSIDE = 2,
  DOWNSIDE = 3,
};

enum class HorizontalSwingDirection : uint8_t {
  LEFT_RIGHT = 1,
  LEFTSIDE = 2,
  CENTER = 3,
  RIGHTSIDE = 4,
};

class tclacClimate : public climate::Climate, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void update();
  
  // تعريف الدالة الجديدة لحل مشكلة المترجم
  void set_gen_mode(int mode);

  void set_beeper_state(bool state);
  void set_display_state(bool disp_state);
  void set_force_mode_state(bool f_state);
  void set_module_display_state(bool d_state);
  void set_vertical_airflow(AirflowVerticalDirection v_airflow);
  void set_horizontal_airflow(AirflowHorizontalDirection h_airflow);
  void set_vertical_swing_direction(VerticalSwingDirection vs_direction);
  void set_horizontal_swing_direction(HorizontalSwingDirection hs_direction);
  
  void set_supported_presets(climate::ClimatePresetMask presets);
  void set_supported_modes(climate::ClimateModeMask modes);
  void set_supported_fan_modes(climate::ClimateFanModeMask fan_modes);
  void set_supported_swing_modes(climate::ClimateSwingModeMask swing_modes);

#ifdef CONF_RX_LED
  void set_rx_led_pin(GPIOPin *rx_led_pin);
#endif
#ifdef CONF_TX_LED
  void set_tx_led_pin(GPIOPin *tx_led_pin);
#endif

 protected:
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
  void readData();
  void takeControl();
  void sendData(uint8_t *message, uint8_t size);
  String getHex(uint8_t *message, uint8_t size);
  uint8_t getChecksum(const uint8_t *message, size_t size);
  void dataShow(bool flow, bool shine);

  // Constants
  static const uint8_t MODE_POS = 7;
  static const uint8_t MODE_MASK = 0x0F;
  static const uint8_t MODE_AUTO = 0x00;
  static const uint8_t MODE_COOL = 0x03;
  static const uint8_t MODE_DRY = 0x02;
  static const uint8_t MODE_FAN_ONLY = 0x07;
  static const uint8_t MODE_HEAT = 0x01;
  static const uint8_t FAN_DIFFUSE = 0x40;

  static const uint8_t FAN_SPEED_POS = 8;
  static const uint8_t FAN_SPEED_MASK = 0x0F;
  static const uint8_t SET_TEMP_MASK = 0xF0;
  static const uint8_t FAN_AUTO = 0x00;
  static const uint8_t FAN_LOW = 0x01;
  static const uint8_t FAN_MIDDLE = 0x06;
  static const uint8_t FAN_MEDIUM = 0x03;
  static const uint8_t FAN_HIGH = 0x07;
  static const uint8_t FAN_FOCUS = 0x05;

  static const uint8_t FAN_QUIET_POS = 8;
  static const uint8_t FAN_QUIET = 0x80;

  static const uint8_t SWING_POS = 10;
  static const uint8_t SWING_MODE_MASK = 0x38;
  static const uint8_t SWING_OFF = 0x00;
  static const uint8_t SWING_VERTICAL = 0x38;
  static const uint8_t SWING_HORIZONTAL = 0x08;
  static const uint8_t SWING_BOTH = 0x38;

  // Variables
  uint8_t dataRX[61];
  uint8_t dataTX[38];
  uint8_t poll[6] = {0xBB, 0x00, 0x01, 0x04, 0x02, 0x01};

  bool beeper_status_{true};
  bool display_status_{true};
  bool force_mode_status_{false};
  bool module_display_status_{true};
  bool allow_take_control{false};
  bool is_call_control{false};

  AirflowVerticalDirection vertical_direction_{AirflowVerticalDirection::LAST};
  AirflowHorizontalDirection horizontal_direction_{AirflowHorizontalDirection::LAST};
  VerticalSwingDirection vertical_swing_direction_{VerticalSwingDirection::UP_DOWN};
  HorizontalSwingDirection horizontal_swing_direction_{HorizontalSwingDirection::LEFT_RIGHT};

  climate::ClimatePresetMask supported_presets_;
  climate::ClimateModeMask supported_modes_;
  climate::ClimateFanModeMask supported_fan_modes_;
  climate::ClimateSwingModeMask supported_swing_modes_;

#ifdef CONF_RX_LED
  GPIOPin *rx_led_pin_;
#endif
#ifdef CONF_TX_LED
  GPIOPin *tx_led_pin_;
#endif
};

}  // namespace tclac
}  // namespace esphome
