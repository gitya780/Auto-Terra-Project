#include "atp.h"

      


void app_main(void)
{
 i2c_init();// initialize the i2c for sht30
 sht_add_device(); // add device to the list of connected devices on i2c bus
 lcd_add_device(); // add device to lcd list 
 ADC_init_moisture(); // initalize adc for moisture
 lcd_start();// initialize the lcd screen
 lcd_clear();// clear the lcd screen
 relay_config(1);// configure the relay(peltier)
 relay_config(19);// configure the fan relay
 relay_config(20);// configure the pump relay
 pwm_fan();// initialize the ledc for fan

   // lcd_init(); //initialize LCD screen
    while(1){
        read_sht();// measure and read the  temperature and humidity from SHT30
        ADC_read_moisture();// 
        vTaskDelay(pdMS_TO_TICKS(100));
        lcd_data_display();// send data to lcd screen

   if(converted_temperature<50){
        relay_toggle(1,1);// turn on the relay (peltier) if temperature is below 50
    }
    if(converted_temperature>75||converted_humidity>75){
        relay_toggle(19,1); // turn on the fan relay if temperature is above 75
        relay_toggle(20,1); // turn on the pump relay if temperature is above 75
        relay_toggle(1,1);// turn on the relay (peltier) if temperature is above 75

    }
    else{
        relay_toggle(19,0); // turn on the fan relay if temperature is above 75
        relay_toggle(20,0); // turn on the pump relay if temperature is above 75
        relay_toggle(1,0);// turn on the relay (peltier) if temperature is above 75
    }

    }
}
