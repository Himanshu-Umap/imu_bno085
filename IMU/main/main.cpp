#include <stdio.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define TX GPIO_NUM_19
#define RX GPIO_NUM_18

int length = 128;
uint8_t data[19];
int16_t yaw;
int16_t pitch;
int16_t roll;
int16_t accell_x;
int16_t accell_y;
int16_t accell_z;
int8_t csum = 0;
int8_t check = 0;

extern "C" void app_main(void)
{   

    uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, TX, RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_2, 1024, 0, 0, NULL, 0);
    uart_read_bytes(UART_NUM_2, (uint8_t *)data, length, portMAX_DELAY);
    // uart_write_bytes(UART_NUM_0, "Hello World!", 12);
    // uart_write_bytes(UART_NUM_0, "\n", 1);
    printf("Hello World!\n");
    while(1) {
        int len = uart_read_bytes(UART_NUM_2, (uint8_t *)data, sizeof(data), 10);
        // printf("hi");
        if(len == 0) {
            printf("No data\n");
        }else {
            // len;
            printf("Data: ");
            
            for(int x = 0; x < len; x++) {
                if(data[x] == 0xAA && data[x+1] == 0xAA){ {
                    for(int i = x+2; i < x+15; i++) {
                        csum = csum + data[i];
                    }
                    csum = csum + data[x+18];
                    printf("practical csum = %x\n",csum);
                    printf("actual csum: %x\n", data[x+20]);
                    //printf("%02X ",data[i]);
                    // buffer[i] = data[x+i];

                    //if(csum == data[x+18]){
                        yaw = ((data[x+4] << 8) | (data[x+3]));
                        yaw /= 100;
                        printf("Yaw: %02d\n",yaw);

                        pitch = ((data[x+6] << 8) | data[x+5] );
                        pitch /= 100;
                        printf("Pitch: %02d\n",pitch);

                        roll = ((data[x+8] << 8) | data[x+7] );
                        roll /= 100;
                        printf("Roll: %d\n",roll);

                        accell_x = ((data[x+10] << 8) | data[x+9]);
                        accell_x = ((accell_x/1000)*9.80665);
                        printf("Accell_x: %d\n",accell_x);

                        accell_y = ((data[x+12] << 8) | data[x+11]);
                        accell_y = ((accell_y/1000)*9.80665);
                        printf("Accell_y: %d\n",accell_y);

                        accell_z = ((data[x+14] << 8) | data[x+13]);
                        accell_z = ((accell_z/1000)*9.80665);
                        printf("Accell_z: %d\n",accell_z);
                    //}

                    check = yaw + pitch + roll + accell_x + accell_y + accell_z;
                    printf("Check: %d\n",check);

                    // if(i == 3){
                    //     yaw = ((buffer[i]) | (buffer[i+1] << 4));
                    //     printf("Yaw: %02X\n",yaw);
                    // }else if(i == 5){
                    //     pitch = (buffer[i] | (buffer[i+1] << 4));
                    //     printf("Pitch: %d\n",pitch);
                    // }else if(i == 7){
                    //     roll = (buffer[i] | (buffer[i+1] << 4));
                    //     printf("Roll: %d\n",roll);
                    // }else if(i == 9){
                    //     accell_x = (buffer[i] | (buffer[i+1] << 4));
                    //     printf("Accell_x: %d\n",accell_x);
                    // }else if(i == 11){
                    //     accell_y = (buffer[i] | (buffer[i+1] << 4));
                    //     printf("Accell_y: %d\n",accell_y);
                    // }else if(i == 13){
                    //     accell_z = (buffer[i] | (buffer[i+1] << 4));
                    //     printf("Accell_z: %d\n",accell_z);
                    // }
                    vTaskDelay(2000 / portTICK_PERIOD_MS);
                
                }
            }
            // printf("Data: %02X\n",data[1]);
            printf("\n");
        }
        // printf("Data: %s\n",data);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    }
}