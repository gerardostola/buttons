#pragma once
#include "iot_button.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int gpio;
  const char* name; 
  button_handle_t handle;
} buttons_type;

typedef int (*buttons_callback) (int button_index, button_event_t event, char *str);
void buttons_init(buttons_type *buttons_definition, 
                  size_t buttons_quantity, 
                  buttons_callback callback,                 
                  size_t long_press_ms,
                  size_t short_press_ms);

#ifdef __cplusplus
}
#endif
