#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unistd.h"

#include "esp_err.h"
#include <sys/param.h>
#include <sys/lock.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_attr.h"
#include "esp_check.h"
#include <sys/param.h>
#include <sys/lock.h>
#include "soc/soc_caps.h"
#include "esp_log.h"

#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "driver/i2c_types.h"
#include "driver/i2c.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


#include "driver/gpio.h"
#include "sdkconfig.h"

#include "driver/ledc.h"




//SHT3X_I2C variables
// extern uint8_t data[6]; // Recieve buffer
extern uint16_t d_temp;  // Temperature value
extern uint16_t d_hum;    // Humidity value
// extern uint16_t send_data;  //measurement address in single shot mode with clock stretching , high repeatability
extern int converted_temperature;
extern int converted_humidity;
// extern uint8_t data_wr[2];  // sending buffer
extern i2c_master_bus_handle_t bus_handle;// I2C bus handle for the sht30
extern i2c_master_dev_handle_t dev_handle;// I2C device handle for the sht30 on the bus

//ADC variables
extern adc_oneshot_unit_handle_t adc1_handle;      /* ADC unit handle */
extern adc_oneshot_unit_handle_t adc2_handle;   /// ADC unit handle
extern long  moisture;                     /* Soil Moisture Value*/
extern long  moisture_2; 



// Function to initialize I2C for SHT30
void i2c_init(void); //initialiization of the SHT30 sensor on I2C bus
void sht_add_device(void); //adding a new slave to the I2C
void read_sht(void) ;   // Reading temperature and humidity from

// Function of LCD
void lcd_add_device(void); // Adding a new slave to the I2C for LCD
void lcd_data_display(void) ; // Send data to LCD screen via I2C interface

// function of LCD for processing the data
void lcd_send_cmd(char) ;     // Function for send command to LCD
void lcd_send_data(char) ;     // Function for send data to LCD
void lcd_clear(void);         // Clearing the LCD screen
void lcd_put_cur(int, int) ;     // Set cursor position on LCD
void lcd_start(void);          // Initialize LCD
void lcd_send_string(char*); // Print string to LCD

void ADC_init_moisture(void);       // Initializing the ADC module for soil
void ADC_read_moisture(void) ;        // Read the analog value from the ADC
long map(long , long , long , long, long ); // Map function for converting the analog value to the percentage


// RELAY 
 void relay_config(int gpio);// Configure the relay give gpio number as parameter
 void relay_toggle(int gpio,int state);// Blink the led give gpio number as parameter and state as parameter


//PWM Fan
 void pwm_fan(void);// PWM fan function
extern esp_err_t err;
