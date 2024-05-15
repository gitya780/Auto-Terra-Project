#include "atp.h"



// int counter = 0;
// TaskHandle_t xHandle=NULL;
// static void intr_handler(void *args)
// {
//     counter++;
// }

// void countTask()
// {
//     while (1)
//     {
//         vTaskDelay(1000 / portTICK_PERIOD_MS);     // 1 sec delay
//         printf("Current RPM: %d\n", counter * 30); // counter/2 * 60
//         counter = 0;
//     }
//     vTaskDelete(NULL);
// }

#define LEDC_TIMER LEDC_TIMER_0 // timer0
#define LEDC_MODE LEDC_LOW_SPEED_MODE 
#define LEDC_OUTPUT_IO (46) // Define the output GPIO
#define LEDC_CHANNEL LEDC_CHANNEL_0 // Define the pwm channel
#define LEDC_DUTY_RES LEDC_TIMER_10_BIT // Set duty resolution to 10 bits
#define LEDC_DUTY (1023)                 // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY (25000)          // Frequency in Hertz. Set frequency at 5 kHz

static void ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    esp_err_t err = ledc_timer_config(&ledc_timer);
    if (err != ESP_OK)
    {
        printf("LEDC timer config failed with error 0x%x\n", err);
        return;
    }

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    err = ledc_channel_config(&ledc_channel);
    if (err != ESP_OK)
    {
        printf("LEDC channel config failed with error 0x%x\n", err);
        return;
    }
}

void pwm_fan(void)
{
//     esp_err_t err = gpio_set_intr_type(GPIO_NUM_15, GPIO_INTR_POSEDGE);// GPIO15 interrupt trigger in rising edge
// if (err != ESP_OK) {
//     printf("Failed to set GPIO interrupt type: %d\n", err);
    
// }
//     err = gpio_install_isr_service(0); //install the isr service (interrupt)
//     if (err != ESP_OK) {
//         printf("Failed to install GPIO ISR service: %d\n", err);
    
// }
//     err = gpio_isr_handler_add(15, intr_handler, NULL); //add the isr handler (interrupt) to the GPIO15 interrupt
// if (err != ESP_OK) {
//     printf("Failed to add GPIO ISR handler: %d\n", err);
    
// }

    /* Configure LED C */
    // Set the LEDC peripheral configuration
    ledc_init();

        // Set duty to 50%
     err = ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY); //
    if (err != ESP_OK)
    {
        printf("LEDC set duty failed with error 0x%x\n", err);
        return;
    }

    // Update duty to apply the new value
    err = ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    if (err != ESP_OK)
    {
        printf("LEDC update duty failed with error 0x%x\n", err);
        return;
    }

    printf("LEDC configuration successful\n");
    // xTaskCreate(countTask, "counter", 2000, NULL, 20, &xHandle);
}
