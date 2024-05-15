#include "atp.h"
#define LCD_ADDR 0x27 // I2C address of the LCD

char buffer_temperature[10];
char buffer_humidity[10];
char buffer_moisture[10];
char buffer_moisture_2[10];

i2c_master_bus_handle_t bus_handle_lcd;
i2c_master_dev_handle_t dev_handle_lcd;
esp_err_t err;


void lcd_add_device(void)
{
    
i2c_device_config_t dev_cfg_lcd = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = LCD_ADDR,
    .scl_speed_hz = 100000,
};

 ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg_lcd, &dev_handle_lcd));
    vTaskDelay(pdMS_TO_TICKS(10));

    ESP_ERROR_CHECK(i2c_master_bus_reset(bus_handle));

}

void lcd_delete(void){
     ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle_lcd));
     vTaskDelay(pdMS_TO_TICKS(10));
}

void lcd_data_display(void){

vTaskDelay(pdMS_TO_TICKS(10));
    // lcd_start();
    //lcd_clear();

    // lcd_put_cur(0, 0);
    // lcd_send_string("Hello world!");
    
    //     lcd_put_cur(1, 0);
        //    lcd_send_string("from ESP32");

sprintf(buffer_temperature, "T=%dF", converted_temperature);
    lcd_put_cur(0, 0);
    lcd_send_string(buffer_temperature);
    vTaskDelay(pdMS_TO_TICKS(100) );
sprintf(buffer_humidity, "H=%d%%", converted_humidity);
    lcd_put_cur(1, 0);
    lcd_send_string(buffer_humidity);
    vTaskDelay(pdMS_TO_TICKS(100) );
sprintf(buffer_moisture, "M=%ld""%%", moisture);
    lcd_put_cur(0, 6);
    lcd_send_string(buffer_moisture);
    vTaskDelay(pdMS_TO_TICKS(1000) ); 
 sprintf(buffer_moisture_2, "S=%ld""%%", moisture_2);
    lcd_put_cur(1, 6);
    lcd_send_string(buffer_moisture_2);
   
           
    }
void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
     
    data_t[0] = data_u | 0x0C;                                                // en=1, rs=0
    data_t[1] = data_u | 0x08;                                                // en=0, rs=0
    data_t[2] = data_l | 0x0C;                                                // en=1, rs=0
    data_t[3] = data_l | 0x08;                                                // en=0, rs=0
    err = i2c_master_transmit(dev_handle_lcd, data_t, 4, pdMS_TO_TICKS(500)); // Increase delay
    if (err != 0)
        ESP_LOGE("lcd", "Error in sending command lcd");
}

void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;                                                // en=1, rs=0
    data_t[1] = data_u | 0x09;                                                // en=0, rs=0
    data_t[2] = data_l | 0x0D;                                                // en=1, rs=0
    data_t[3] = data_l | 0x09;                                                // en=0, rs=0
    err = i2c_master_transmit(dev_handle_lcd, data_t, 4, pdMS_TO_TICKS(500)); // Increase delayL
    if (err != 0)
        ESP_LOGE("lcd", "Error in sending data lcd");
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    usleep(5000);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
    case 0:
        col |= 0x80;
        break;
    case 1:
        col |= 0xC0;
        break;
    }

    lcd_send_cmd(col);
}

void lcd_start(void)
{
    // 4 bit initialisation
    usleep(50000); // wait for >40ms
    lcd_send_cmd(0x30);
    usleep(5000); // wait for >4.1ms
    lcd_send_cmd(0x30);
    usleep(200); // wait for >100us
    lcd_send_cmd(0x30);
    usleep(10000);
    lcd_send_cmd(0x20); // 4bit mode
    usleep(10000);

    // dislay initialisation
    lcd_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    usleep(1000);
    lcd_send_cmd(0x08); // Display on/off control --> D=0,C=0, B=0  ---> display off
    usleep(1000);
    lcd_send_cmd(0x01); // clear display
    usleep(1000);
    usleep(1000);
    lcd_send_cmd(0x06); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    usleep(1000);
    lcd_send_cmd(0x0C); // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
    usleep(1000);
}

void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}


