#include <LOG_HANDLE.h>

int led_pin = 7;
bool led_state = false;
bool last_mounted = false;
bool sd_msc_ready = false;

int counter = 0;
bool is_logging = false;
unsigned long last_log_time = 0;

// LED blink timing
unsigned long last_blink = 0;


void initial_CSV_SETUP()
{
    if (sd.begin(5, SD_SCK_MHZ(50)))
    {
        if (!sd.exists("offline_log.csv"))
        {
            File f = sd.open("offline_log.csv", FILE_WRITE);
            if (f)
            {
                f.println("Time_Duration");
                f.close();
            }
        }
        is_logging = true;
    }
}
void Disconnect_MSU()
{
    if (millis() - last_blink > 500)
    {
        led_state = !led_state;
        digitalWrite(led_pin, led_state);
        last_blink = millis();
    }

    // If SD is available and we're not in USB mode, log every second
    if (is_logging && millis() - last_log_time >= 1000)
    {
        last_log_time = millis();
        counter++;

        File f = sd.open("offline_log.csv", FILE_WRITE);
        if (f)
        {
            f.println(counter);
            f.close();
        }
    }
}