#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "resource_ids.auto.h"
#include <stdint.h>
#include <string.h>

#define BITMAP_BUFFER_BYTES 1024


#define MY_UUID { 0x8D, 0x22, 0x23, 0x1C, 0xC1, 0x7F, 0x43, 0x29, 0xAF, 0xF5, 0x87, 0x7F, 0x30, 0xBF, 0x65, 0xA0 }
PBL_APP_INFO(MY_UUID,
             "Quiz Metronome", "Instructure",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

static struct QuizData {
  Window window;
  TextLayer timer_layer;
  char timerValue[16];
  BitmapLayer icon_layer;
  GBitmap icon_bitmap;
  uint8_t bitmap_data[BITMAP_BUFFER_BYTES];
  AppSync sync;
  uint8_t sync_buffer[32];
} s_data;

enum {
  TIMER_ICON_KEY = 0x0,         // TUPLE_INT
  TIMER_VALUE_KEY = 0x1,  // TUPLE_CSTRING
};

// TODO: Error handling
static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  (void) dict_error;
  (void) app_message_error;
  (void) context;
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  (void) old_tuple;

  switch (key) {
  // case TIMER_ICON_KEY:
  //   load_bitmap(WEATHER_ICONS[new_tuple->value->uint8]);
  //   layer_mark_dirty(&s_data.icon_layer.layer);
  //   break;
  case TIMER_VALUE_KEY:
    strncpy(s_data.timerValue, new_tuple->value->cstring, 16);
    layer_mark_dirty(&s_data.timer_layer.layer);
    break;
  default:
    return;
  }
}

void quiz_metronome_init(AppContextRef ctx) {
  (void)ctx;

  Window* window = &s_data.window;
  window_init(window, "Metronome");
  window_set_background_color(window, GColorBlack);
	
  text_layer_init(&s_data.timer_layer, GRect(0, 100, 144, 68));
  text_layer_set_text_color(&s_data.timer_layer, GColorWhite);
  text_layer_set_background_color(&s_data.timer_layer, GColorClear);
  text_layer_set_font(&s_data.timer_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(&s_data.timer_layer, GTextAlignmentCenter);
  text_layer_set_text(&s_data.timer_layer, s_data.timerValue);
  layer_add_child(&window->layer, &s_data.timer_layer.layer);

  Tuplet initial_values[] = {
    //TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(TIMER_VALUE_KEY, "12:34:56"),
  };

  app_sync_init(&s_data.sync, s_data.sync_buffer, sizeof(s_data.sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
                sync_tuple_changed_callback, sync_error_callback, NULL);
	
  window_stack_push(window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &quiz_metronome_init
  };
  app_event_loop(params, &handlers);
}
