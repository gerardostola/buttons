#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "buttons.h"

#define SHORT_PRESS_DURATION_MS 100
#define LONG_PRESS_DURATION_MS 1000

#define BUTTONS_QUANTITY 2
static buttons_type buttons_definition[BUTTONS_QUANTITY] =
{  /*pin number, familiar name, always 0*/
  {12,"up", 0},
  {14,"down", 0}
};


/* Callback */
int on_buttons_news (int button_index, button_event_t event, char *str) {
  ESP_LOGI(__FUNCTION__, "Button #%d(%s) event: %s", button_index, 
          str, iot_button_get_event_str(event));
  return 0;
}


void app_main(void) {
      
    
    buttons_init( buttons_definition,
                  BUTTONS_QUANTITY,   /* Buttons in system */
                  on_buttons_news,    /* Callback function */
                  SHORT_PRESS_DURATION_MS, /* Milliseconds for short press detection */
                  LONG_PRESS_DURATION_MS /* Milliseconds for long press detection */
                );
                
               
    while (1) {
      vTaskDelay(20 / portTICK_PERIOD_MS);		
    }
    
}