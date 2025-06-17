#pragma once

#include "SPI.h"
#include "SdFat.h"
#include "Adafruit_TinyUSB.h"
// File system on SD Card
extern SdFat sd;

extern SdFile root;
extern SdFile file;

////////////// USB Mass Storage object////////////////////
extern Adafruit_USBD_MSC usb_msc;


extern bool fs_changed;

/////////////main funstion initialization ///////////////

void msc_flush_cb (void);
int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize);
int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize);

/////////////main SD_CARD setup for work as the MSU device ///////////////
void SD_CARD_MSU_SETUP(void);