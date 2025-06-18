#pragma once

#include <ARDUINO.h>
#include "SD_CONFIG.h"


extern bool led_state;

extern bool last_mounted;
extern bool sd_msc_ready;

extern int counter;
extern bool is_logging;
extern unsigned long last_log_time;

// LED blink timing
extern unsigned long last_blink;
extern bool led_state;
extern int led_pin;

void Disconnect_MSU();
void initial_CSV_SETUP();