#include <SD_CONFIG.h>

SdFat sd;
SdFile root;
SdFile file;
Adafruit_USBD_MSC usb_msc;

const int chipSelect = 5;
bool fs_changed;
// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_cb(void)
{
#if SD_FAT_VERSION >= 20000
    sd.card()->syncDevice();
#else
    sd.card()->syncBlocks();
#endif
    // clear file system's cache to force refresh
    sd.cacheClear();
    fs_changed = true; // set flag to indicate file system has changed
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize)
{
    bool rc;

#if SD_FAT_VERSION >= 20000
    rc = sd.card()->readSectors(lba, (uint8_t *)buffer, bufsize / 512);
#else
    rc = sd.card()->readBlocks(lba, (uint8_t *)buffer, bufsize / 512);
#endif

    return rc ? bufsize : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize)
{
    bool rc;

    digitalWrite(LED_BUILTIN, HIGH);

#if SD_FAT_VERSION >= 20000
    rc = sd.card()->writeSectors(lba, buffer, bufsize / 512);
#else
    rc = sd.card()->writeBlocks(lba, buffer, bufsize / 512);
#endif

    return rc ? bufsize : -1;
}

void SD_CARD_MSU_SETUP(void)
{
    // Reinit SD to share with PC
    if (sd.begin(5, SD_SCK_MHZ(50)))
    {
#if SD_FAT_VERSION >= 20000
        uint32_t block_count = sd.card()->sectorCount();
#else
        uint32_t block_count = sd.card()->cardSize();
#endif
        usb_msc.setCapacity(block_count, 512);
        usb_msc.setUnitReady(true); // allow PC to read the file
    }
}