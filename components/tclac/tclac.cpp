ClimateTraits tclacClimate::traits() {
	auto traits = climate::ClimateTraits();

	traits.set_supports_action(false);
	traits.set_supports_current_temperature(true);
	traits.set_supports_two_point_target_temperature(false);

	// التحويل من std::set إلى الأقنعة الجديدة (Masks)
	for (auto mode : this->supported_modes_) {
		traits.add_supported_mode(mode);
	}
	for (auto preset : this->supported_presets_) {
		traits.add_supported_preset(preset);
	}
	for (auto fan_mode : this->supported_fan_modes_) {
		traits.add_supported_fan_mode(fan_mode);
	}
	for (auto swing_mode : this->supported_swing_modes_) {
		traits.add_supported_swing_mode(swing_mode);
	}
	
	traits.add_supported_mode(climate::CLIMATE_MODE_OFF);
	traits.add_supported_mode(climate::CLIMATE_MODE_AUTO);
	traits.add_supported_fan_mode(climate::CLIMATE_FAN_AUTO);
	traits.add_supported_swing_mode(climate::CLIMATE_SWING_OFF);
	traits.add_supported_preset(ClimatePreset::CLIMATE_PRESET_NONE);

	return traits;
}
