#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x8D, 0x22, 0x23, 0x1C, 0xC1, 0x7F, 0x43, 0x29, 0xAF, 0xF5, 0x87, 0x7F, 0x30, 0xBF, 0x65, 0xA0 }
PBL_APP_INFO(MY_UUID,
             "Quiz Metronome", "Instructure",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

static struct QuizData {
  Window window;
  TextLayer temperature_layer;
  char temperature[16];
  BitmapLayer icon_layer;
  GBitmap icon_bitmap;
  uint8_t bitmap_data[BITMAP_BUFFER_BYTES];
  AppSync sync;
  uint8_t sync_buffer[32];
} s_data;


void handle_init(AppContextRef ctx) {
  (void)ctx;

//  window_init(&window, "Window Name");
  window_init(window, "Metronome");
  window_set_background_color(window, GColorBlack);
	
  text_layer_init(&s_data.temperature_layer, GRect(0, 100, 144, 68));
  text_layer_set_text_color(&s_data.temperature_layer, GColorWhite);
  text_layer_set_background_color(&s_data.temperature_layer, GColorClear);
  text_layer_set_font(&s_data.temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(&s_data.temperature_layer, GTextAlignmentCenter);
  text_layer_set_text(&s_data.temperature_layer, s_data.temperature);
  layer_add_child(&window->layer, &s_data.temperature_layer.layer);

  Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(WEATHER_TEMPERATURE_KEY, "1234\u00B0C"),
  };
	
  window_stack_push(window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
