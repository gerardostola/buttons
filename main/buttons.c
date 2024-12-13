#define BUTTONS_DEBUG 0
#include "esp_log.h"
#include "iot_button.h"
#include "buttons.h"

#if BUTTONS_DEBUG
  static const char *TAG = "buttons-debug";
  #define debug(tag, format, ...) ESP_LOGI( tag, format, ##__VA_ARGS__)
#else
  #define debug(tag, format, ...)
#endif

buttons_callback _cb=NULL;
buttons_type * _buttons_definition;
size_t _buttons_quantity=0;

static size_t buttons_get_index(button_handle_t btn)
{
    for (size_t i = 0; i < _buttons_quantity; i++) {
        if (btn == _buttons_definition[i].handle) {
            return i;
        }
    }
    return _buttons_quantity;
}

void buttons_cb(void *arg, void *data)
{
    if (NULL==_cb) {
      return;
    }
    button_handle_t button_handle = (button_handle_t)arg;
        
    button_event_t event =iot_button_get_event(button_handle);
#if BUTTONS_DEBUG
    uint32_t ticks = iot_button_get_ticks_time(button_handle);    
    int16_t repeat = iot_button_get_repeat(button_handle);
    int16_t long_press_hold_cnt = iot_button_get_long_press_hold_cnt(button_handle);
    const char * event_name = iot_button_get_event_str(event);
#endif   
    size_t index = buttons_get_index(button_handle);
       
    debug(TAG, "%s %d %s", event_name, index, (char*)data);
    switch (event) {
    case BUTTON_PRESS_DOWN:      
      break;
    case BUTTON_PRESS_UP:    
      debug(TAG, "[%lu]", ticks);
      break;
    case BUTTON_PRESS_REPEAT:
      debug(TAG, "[%d]", repeat);
      break;
    case BUTTON_PRESS_REPEAT_DONE:   
      debug(TAG, "[%d]", repeat);    
      break;
    case BUTTON_SINGLE_CLICK:
      _cb(index, event, (char*)data);
      break;
    case BUTTON_DOUBLE_CLICK:      
      break;
    case BUTTON_MULTIPLE_CLICK:
      break;
    case BUTTON_LONG_PRESS_START:      
      break;
    case BUTTON_LONG_PRESS_HOLD:
      debug(TAG, "[%lu],count is [%d]", ticks, long_press_hold_cnt);
      break;
    case BUTTON_LONG_PRESS_UP:
      debug(TAG, "[%lu]", ticks);
      _cb(index, event, (char*)data);
      break;
    case BUTTON_PRESS_END:
      break;
    case BUTTON_EVENT_MAX:
      break;
    case BUTTON_NONE_PRESS:
      break;
    }
}

void buttons_init(buttons_type *buttons_definition,
                  size_t buttons_quantity, 
                  buttons_callback callback,
                  size_t short_press_ms,
                  size_t long_press_ms) {
    
    _cb = callback;
    _buttons_quantity = buttons_quantity;
    _buttons_definition=buttons_definition;
    button_config_t cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = long_press_ms,
        .short_press_time = short_press_ms,        
    };
        
    for (int i = 0; i < _buttons_quantity; i++) {
        cfg.gpio_button_config.gpio_num = buttons_definition[i].gpio;
        cfg.gpio_button_config.active_level = 0;
        
        _buttons_definition[i].handle = iot_button_create(&cfg);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_PRESS_DOWN, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_PRESS_UP, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_PRESS_REPEAT, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_SINGLE_CLICK, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_DOUBLE_CLICK, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_LONG_PRESS_START, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_LONG_PRESS_HOLD, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_LONG_PRESS_UP, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_PRESS_REPEAT_DONE, 
                                buttons_cb, (void*)buttons_definition[i].name);
        iot_button_register_cb(_buttons_definition[i].handle, BUTTON_PRESS_END, 
                                buttons_cb, (void*)buttons_definition[i].name);
    }
}
