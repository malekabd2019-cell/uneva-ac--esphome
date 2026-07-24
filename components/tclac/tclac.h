/**
* Create by Miguel Ángel López on 20/07/19
* and modify by xaxexa
* Refactoring & component making:
* Соловей с паяльником 15.03.2024
* Updated for ESPHome 2026+ API & Safe Pointers
**/

#ifndef TCL_ESP_TCL_H
#define TCL_ESP_TCL_H

#include "esphome.h"
#include "esphome/core/defines.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"

namespace esphome {
namespace tclac {

#define SET_TEMP_MASK	0b00001111

#define MODE_POS		7
#define MODE_MASK		0b00111111

#define MODE_AUTO		0b00110101
#define MODE_COOL		0b00110001
#define MODE_DRY		0b00110011
#define MODE_FAN_ONLY	0b00110010
#define MODE_HEAT		0b00110100

#define FAN_SPEED_POS	8
#define FAN_QUIET_POS	33

#define FAN_AUTO		0b10000000
#define FAN_QUIET		0x80
#define FAN_LOW			0b10010000
#define FAN_MIDDLE		0b11000000
#define FAN_MEDIUM		0b10100000
#define FAN_HIGH		0b11010000
#define FAN_FOCUS		0b10110000
#define FAN_DIFFUSE		0b10000000
#define FAN_SPEED_MASK	0b11110000

#define SWING_POS			10
#define SWING_OFF			0b00000000
#define SWING_HORIZONTAL	0b00100000
#define SWING_VERTICAL		0b01000000
#define SWING_BOTH			0b01100000
#define SWING_MODE_MASK		0b01100000

using climate::ClimateCall;
using climate::ClimateMode;
using climate::ClimatePreset;
using climate::ClimateTraits;
using climate::ClimateFanMode;
using climate::ClimateSwingMode;

enum class VerticalSwingDirection : uint8_t {
	UP_DOWN = 0,
	UPSIDE = 1,
	DOWNSIDE = 2,
};
enum class HorizontalSwingDirection : uint8_t {
	LEFT_RIGHT = 0,
	LEFTSIDE = 1,
	CENTER = 2,
	RIGHTSIDE = 3,
};
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

class tclacClimate : public climate::Climate, public esphome::uart::UARTDevice, public PollingComponent {

	private:
		byte checksum{0};
		byte dataTX[38]{0};
		byte dataRX[61]{0};
		byte poll[8] = {0xBB,0x00,0x01,0x04,0x02,0x01,0x00,0xBD};
		bool beeper_status_{false};
		bool display_status_{false};
		bool force_mode_status_{false};
		uint8_t switch_preset{0};
		bool module_display_status_{false};
		uint8_t switch_fan_mode{0};
		bool is_call_control{false};
		uint8_t switch_swing_mode{0};
		int target_temperature_set{0};
		uint8_t switch_climate_mode{0};
		bool allow_take_control{false};

		uint8_t gen_mode_{0x00};
		
		esphome::climate::ClimateTraits traits_;
		
	public:

		tclacClimate() : PollingComponent(5 * 1000) {
			checksum = 0;
		}

		void readData();
		void takeControl();
		void loop() override;
		void setup() override;
		void update() override;
		void set_beeper_state(bool state);
		void set_display_state(bool state);
		void dataShow(bool flow, bool shine);
		void set_force_mode_state(bool state);
		void set_rx_led_pin(GPIOPin *rx_led_pin);
		void set_tx_led_pin(GPIOPin *tx_led_pin);
		void sendData(byte * message, byte size);
		void set_module_display_state(bool state);
		static String getHex(byte *message, byte size);
		void control(const ClimateCall &call) override;
		static byte getChecksum(const byte * message, size_t size);
		void set_vertical_airflow(AirflowVerticalDirection direction);
		void set_horizontal_airflow(AirflowHorizontalDirection direction);
		void set_vertical_swing_direction(VerticalSwingDirection direction);
		void set_horizontal_swing_direction(HorizontalSwingDirection direction);

		template<typename T> void set_supported_presets(const T &presets) {
			for (auto p : presets) this->supported_presets_.insert(p);
		}
		template<typename T> void set_supported_modes(const T &modes) {
			for (auto m : modes) this->supported_modes_.insert(m);
		}
		template<typename T> void set_supported_fan_modes(const T &modes) {
			for (auto f : modes) this->supported_fan_modes_.insert(f);
		}
		template<typename T> void set_supported_swing_modes(const T &modes) {
			for (auto s : modes) this->supported_swing_modes_.insert(s);
		}

		void set_gen_mode(uint8_t mode) {
			this->gen_mode_ = mode;
			this->takeControl();
		}

		uint8_t get_gen_mode() const {
			return this->gen_mode_;
		}
		
	protected:
		GPIOPin *rx_led_pin_{nullptr};
		GPIOPin *tx_led_pin_{nullptr};
		ClimateTraits traits() override;

		std::set<ClimateMode> supported_modes_{};
		std::set<ClimatePreset> supported_presets_{};
		AirflowVerticalDirection vertical_direction_{AirflowVerticalDirection::LAST};
		std::set<ClimateFanMode> supported_fan_modes_{};
		AirflowHorizontalDirection horizontal_direction_{AirflowHorizontalDirection::LAST};
		VerticalSwingDirection vertical_swing_direction_{VerticalSwingDirection::UP_DOWN};
		std::set<ClimateSwingMode> supported_swing_modes_{};
		HorizontalSwingDirection horizontal_swing_direction_{HorizontalSwingDirection::LEFT_RIGHT};
};

}  // namespace tclac
}  // namespace esphome

#endif  // TCL_ESP_TCL_H
			
