#include <Arduino.h>
#include <SD_CONFIG.h>
#include <LOG_HANDLE.h>

bool MSU_ENABLED = false; // Set to false to disable MSU
void setup()
{
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.begin(115200);

  // Initial USB setup, MSC not ready yet
  usb_msc.setID("MSU_TEST", "SD Card", "1.0");
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  usb_msc.setUnitReady(false);
  usb_msc.begin();

  // Try to init SD at startup
  // This will be reinitialized when USB is connected
  initial_CSV_SETUP();
}

void loop()
{
  bool now_mounted = TinyUSBDevice.mounted();

  // LED blink while disconnected
  if (!now_mounted)
  {
   Disconnect_MSU();
  }

  // When connected to PC
  if (now_mounted && !last_mounted)
  {

    MSU_ENABLED = true; // Enable MSU functionality
    digitalWrite(led_pin, HIGH);
    SD_CARD_MSU_SETUP(); // Reinitialize SD for USB MSC
    // Stop writing to SD while USB is connected
    is_logging = false;
  }

  last_mounted = now_mounted;
}
