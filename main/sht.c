#include "atp.h"

uint16_t send_data = 0x2c06;

// Define variables used in sht.c
i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;
// uint8_t data_wr[2];
// uint8_t data[6];
uint16_t d_temp;
uint16_t d_hum;
int converted_temperature;
int converted_humidity;


void i2c_init(void){
i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = GPIO_NUM_6,
        .sda_io_num = GPIO_NUM_7,
        .glitch_ignore_cnt = 7,
        .flags = {
            .enable_internal_pullup = 1       // Enable internal pullups
        }    // enable pull-up on SDA pin
    };
    

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
     vTaskDelay(pdMS_TO_TICKS(10));

    
    
}

void sht_add_device(void)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x44,
        .scl_speed_hz = 400000,
    };

 

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
         vTaskDelay(pdMS_TO_TICKS(10));
    ESP_ERROR_CHECK(i2c_master_bus_reset(bus_handle));
    
}

void read_sht(void){
    uint16_t send_data = 0x2c06;
    uint8_t data_wr[2];
uint8_t data[6];
 // Array to hold the 16-bit value as two 8-bit bytes
    data_wr[0] = (send_data >> 8) & 0xFF; // MSB
    data_wr[1] = send_data & 0xFF; 
     // Transmit data to the sensor
        esp_err_t err = i2c_master_transmit(dev_handle, data_wr, 2, pdMS_TO_TICKS(500)); // Increase delay
                
        if (err != ESP_OK)
        {
            printf("Error transmitting data to the sensor\n");
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for error recovery
            return;
        }
     vTaskDelay(pdMS_TO_TICKS(100));
        // Receive data from the sensor
        err = i2c_master_receive(dev_handle, data, sizeof(data), pdMS_TO_TICKS(500)); // Increase delay
              
        if (err != ESP_OK)
        {
            printf("Error receiving data from the sensor\n");
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for error recovery
            return;
        }
         vTaskDelay(pdMS_TO_TICKS(100));
         d_temp = (data[0] << 8) | data[1];
       d_hum = (data[3] << 8) | data[4];
        converted_temperature = (-49 + 0.0048065919 * d_temp);
      converted_humidity = (0.0015259022 * d_hum);
      
      
        // Output data to screen
            printf("Relative Humidity : %d RH \n", converted_humidity);
        printf("Temperature in Fahrenheit : %d F \n", converted_temperature);
        vTaskDelay(pdMS_TO_TICKS(1000));
}