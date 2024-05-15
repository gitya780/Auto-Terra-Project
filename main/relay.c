#include "atp.h"






 void relay_toggle(int gpio,int state)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(gpio, state);
}

 void relay_config(int gpio)
{

    gpio_reset_pin(gpio);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
}


// void app_main(void)
// {

//     /* Configure the peripheral according to the LED type */
//     configure_led();

//     while (1) {
//         ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
//         blink_led();
//         /* Toggle the LED state */
//         s_led_state = !s_led_state;
//          vTaskDelay(pdMS_TO_TICKS(1000) );
// }
// }