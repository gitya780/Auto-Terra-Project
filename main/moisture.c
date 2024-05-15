#include "atp.h"
 long moisture;
 long moisture_2;
static int adc_raw_1=0;
static int adc_raw_2=0;

adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_handle_t adc2_handle;
// Moisture 1 
   void ADC_init_moisture(void){
    adc_oneshot_unit_init_cfg_t init_config1 =
        {
                .unit_id = ADC_UNIT_1,
        };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
        
        adc_oneshot_chan_cfg_t config_1 = {
                .bitwidth = ADC_BITWIDTH_12,
                .atten = ADC_ATTEN_DB_12,
        };
        
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_2, &config_1));

//moisture 2
// adc_oneshot_unit_init_cfg_t init_config2 =
//         {
//                 .unit_id = ADC_UNIT_1,
//         };

       //ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc2_handle));
        
                // adc_oneshot_chan_cfg_t config_2 = {
        //         .bitwidth = ADC_BITWIDTH_12,
        //         .atten = ADC_ATTEN_DB_12,
        // };
         ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_3, &config_1));
   }
    void ADC_read_moisture(void){
        
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_2, &adc_raw_1));
        vTaskDelay(pdMS_TO_TICKS(100));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_3, &adc_raw_2));
        vTaskDelay(pdMS_TO_TICKS(100));
        long moisture_map=map(adc_raw_1,3100,1150,0,100);
        long moisture_map2=map(adc_raw_2,3100,1150,0,100);
        moisture=moisture_map;
        moisture_2=moisture_map2;

         //moisture=(100-((adc_raw_1/4095.00)*100));// mapping the adc data in the percentage
          printf("ADC%d convert Data: %ld\n", ADC_UNIT_1 + 1, moisture);
          printf("ADC%d convert Data: %ld\n", ADC_UNIT_2 + 1, moisture_2);
         
         printf("ADC%d raw Data: %d\n", ADC_UNIT_1 + 1,adc_raw_1);
          printf("ADC%d raw Data: %d\n", ADC_UNIT_2 + 1, adc_raw_2);
         
    }

    long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
    
