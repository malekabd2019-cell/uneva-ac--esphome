#pragma once
#include "esphome.h"

class TCLGeneratorManager : public Component {
 public:
  uint8_t current_generator_level = 0;
  bool is_updating_from_remote = false;

  void set_generator(uint8_t level) {
    this->current_generator_level = level;
  }
};

// كائن عام للتحكم بوضع المولد في الذاكرة
extern TCLGeneratorManager *global_tcl_gen;
