#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "utils.h"

namespace esphome {
namespace daikin_s21 {

enum class DaikinClimateMode : uint8_t {
  Disabled = '1',
  Auto = '0',
  Dry = '2',
  Cool = '3',
  Heat = '4',
  Fan = '6',
};

enum class DaikinFanMode : uint8_t {
  Auto = 'A',
  Silent = 'B',
  Speed1 = '3',
  Speed2 = '4',
  Speed3 = '5',
  Speed4 = '6',
  Speed5 = '7',
};

namespace StateQuery {
  inline constexpr const char* Basic = "F1";
  inline constexpr const char* Swing = "F5";
  inline constexpr const char* Powerful = "F6";
  inline constexpr const char* Econo = "F7";
  inline constexpr const char* OldProtocol = "F8";
  inline constexpr const char* NewProtocol = "FY00";
  inline constexpr const char* InsideOutsideTemperatures = "F9";
}  // namespace StateQuery

namespace EnvironmentQuery {
  inline constexpr const char* Inside = "RH";
  inline constexpr const char* Coil = "RI";
  inline constexpr const char* Outside = "Ra";
  inline constexpr const char* FanSpeed = "RL";
  inline constexpr const char* Compressor = "Rd";
} // namespace EnvironmentQuery

namespace StateResponse {
  inline constexpr const char* Basic = "G1";
  inline constexpr const char* Swing = "G5";
  inline constexpr const char* Powerful = "G6";
  inline constexpr const char* Econo = "G7";
  inline constexpr const char* OldProtocol = "G8";
  inline constexpr const char* NewProtocol = "GY00";
  inline constexpr const char* InsideOutsideTemperatures = "G9";
}  // namespace StateResponse

namespace EnvironmentResponse {
  inline constexpr const char* Inside = "SH";
  inline constexpr const char* Coil = "SI";
  inline constexpr const char* Outside = "Sa";
  inline constexpr const char* FanSpeed = "SL";
  inline constexpr const char* Compressor = "Sd";
} // namespace EnvironmentResponse

namespace OldProtocol {
  inline constexpr const char* Protocol0 = "\x30\x00\x00\x00";
  inline constexpr const char* Protocol2_1 = "\x30\x32\x00\x00";
  inline constexpr const char* Protocol2_2 = "\x30\x32\x30\x30";
}  // namespace OldProtocol

namespace NewProtocol {
  inline constexpr const char* Protocol3_00_or_3_10 = "0030";
  inline constexpr const char* Protocol3_20 = "0230";
}  // namespace NewProtocol

std::string daikin_climate_mode_to_string(DaikinClimateMode mode);
std::string daikin_fan_mode_to_string(DaikinFanMode mode);

inline float c10_c(int16_t c10) { return c10 / 10.0; }
inline float c10_f(int16_t c10) { return c10_c(c10) * 1.8 + 32.0; }

class DaikinS21 : public PollingComponent {
 public:
  void update() override;
  void dump_config() override;
  void set_uarts(uart::UARTComponent *tx, uart::UARTComponent *rx);
  void set_debug_protocol(bool set) { this->debug_protocol = set; }
  bool is_ready() { return this->ready; }

  bool is_power_on() { return this->power_on; }
  DaikinClimateMode get_climate_mode() { return this->mode; }
  DaikinFanMode get_fan_mode() { return this->fan; }
  float get_setpoint() { return this->setpoint / 10.0; }
  void set_daikin_climate_settings(bool power_on, DaikinClimateMode mode,
                                   float setpoint, DaikinFanMode fan_mode);
  void set_swing_settings(bool swing_v, bool swing_h);
  bool send_cmd(std::vector<uint8_t> code, std::vector<uint8_t> payload);

  float get_temp_inside() { return this->temp_inside / 10.0; }
  float get_temp_outside() { return this->temp_outside / 10.0; }
  float get_temp_coil() { return this->temp_coil / 10.0; }
  uint16_t get_fan_rpm() { return this->fan_rpm; }
  bool is_idle() { return this->idle; }
  bool get_swing_h() { return this->swing_h; }
  bool get_swing_v() { return this->swing_v; }
  bool get_powerful() { return this->powerful; }
  bool get_econo() { return this->econo; }
  void set_powerful_settings(bool value);
  void set_econo_settings(bool value);
  void set_has_presets(bool value) {
    this->has_presets = value;
  };

 protected:
  bool read_frame(std::vector<uint8_t> &payload);
  void write_frame(std::vector<uint8_t> payload);
  bool s21_query(std::vector<uint8_t> code);
  bool parse_response(std::vector<uint8_t> rcode, std::vector<uint8_t> payload);
  bool run_queries(std::vector<std::string> queries);
  void dump_state();
  void check_uart_settings();
  bool wait_byte_available(uint32_t  timeout);

  uart::UARTComponent *tx_uart{nullptr};
  uart::UARTComponent *rx_uart{nullptr};
  bool ready = false;
  bool debug_protocol = false;

  bool power_on = false;
  DaikinClimateMode mode = DaikinClimateMode::Disabled;
  DaikinFanMode fan = DaikinFanMode::Auto;
  int16_t setpoint = 21;
  bool swing_v = false;
  bool swing_h = false;
  bool powerful = false;
  bool econo = false;
  int16_t temp_inside = 0;
  int16_t temp_outside = 0;
  int16_t temp_coil = 0;
  uint16_t fan_rpm = 0;
  bool idle = true;
  bool has_presets = true;
  bool protocol_checked = false;
  uint8_t f8_protocol = -1;
  uint8_t f8_protocol_variant = -1;
  uint8_t fy00_protocol_major = -1;
  uint8_t fy00_protocol_minor = -1;
};

class DaikinS21Client {
 public:
  void set_s21(DaikinS21 *s21) { this->s21 = s21; }

 protected:
  DaikinS21 *s21;
};

}  // namespace daikin_s21
}  // namespace esphome
