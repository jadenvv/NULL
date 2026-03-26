#include "i2c.h"
static enum menu all_menus[] = {MAIN, ARP, RFID, IR};
static enum option_main menu_opt[] = {ARP_OPT, IR_OPT, RFID_OPT, LORA_OPT};
static const char *TAG_I2C = "I2C_OLED";
static bool flush(esp_lcd_panel_io_handle_t panel_io,
                  esp_lcd_panel_io_event_data_t *edata, void *user_ctx) {
  lv_disp_t *disp = (lv_disp_t *)user_ctx;
  lvgl_port_flush_ready(disp);
  return false;
}

struct i2c_handles init_OLED() {
  ESP_LOGI(TAG_I2C, "intializing i2c...");
  i2c_master_bus_config_t bus_config = {.i2c_port = -1,
                                        .sda_io_num = CONFIG_I2C_MASTER_SDA,
                                        .scl_io_num = CONFIG_I2C_MASTER_SCL,
                                        .clk_source = I2C_CLK_SRC_RC_FAST,
                                        .glitch_ignore_cnt = 7, // might change
                                        .intr_priority = 0,
                                        .trans_queue_depth = 7,
                                        .flags.allow_pd = true};
  i2c_master_bus_handle_t bus_handle;
  esp_err_t err = i2c_new_master_bus(&bus_config, &bus_handle);
  ESP_ERROR_CHECK(err);
  i2c_device_config_t device_config = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = CONFIG_I2C_DEVICE_ADDRESS,
      .scl_speed_hz = CONFIG_I2C_MASTER_FREQUENCY,
      .scl_wait_us = 0,
      .flags.disable_ack_check = false};
  i2c_master_dev_handle_t dev_handle;
  ESP_ERROR_CHECK(
      i2c_master_bus_add_device(bus_handle, &device_config, &dev_handle));
  return (struct i2c_handles){bus_handle, dev_handle};
}
// note : i don't know if lv_disp_t goes out of scope and gets cleaned up
lv_disp_t *ssd1306_driver_init(struct i2c_handles handles) {
  esp_lcd_panel_io_i2c_config_t config = {.dev_addr = CONFIG_I2C_DEVICE_ADDRESS,
                                          .scl_speed_hz =
                                              CONFIG_I2C_MASTER_FREQUENCY,
                                          .control_phase_bytes = 1,
                                          .dc_bit_offset = 6,
                                          .lcd_cmd_bits = 8,
                                          .lcd_param_bits = 8

  };
  esp_lcd_panel_io_handle_t IO_handle = NULL;
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_io_i2c_v2(handles.bus_handler, &config, &IO_handle));
  esp_lcd_panel_dev_config_t panel_config = {.bits_per_pixel = 1,
                                             .reset_gpio_num = -1};
  esp_lcd_panel_handle_t panel_handle = NULL;
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ssd1306(IO_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  ESP_LOGI(TAG_I2C, "successfully initialized i2c...");
  const lvgl_port_cfg_t lvgl_config = ESP_LVGL_PORT_INIT_CONFIG();
  lvgl_port_init(&lvgl_config);
  const lvgl_port_display_cfg_t disp_config = {
      .io_handle = IO_handle,
      .panel_handle = panel_handle,
      .buffer_size = CONFIG_HEIGHT_OLED * CONFIG_WIDTH_OLED,
      .hres = CONFIG_HEIGHT_OLED,
      .vres = CONFIG_WIDTH_OLED,
      .monochrome = true,
      .rotation = {.swap_xy = false, .mirror_x = false, .mirror_y = false}};
  lv_disp_t *disp = lvgl_port_add_disp(&disp_config);
  const esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = flush,
  };
  esp_lcd_panel_io_register_event_callbacks(IO_handle, &cbs, disp);
  ESP_LOGI(TAG_I2C, "initialized the LVGL display Handle");
  return disp;
}

state *create_screens() {
  bool ran = false;
  static state *screens = NULL;
  if (ran)
    goto ret;
  screens = heap_caps_malloc(sizeof(state) * NUM_OF_SCR, MALLOC_CAP_INTERNAL);

  for (size_t x = 0; x < NUM_OF_SCR; x++) {
    if (x == 0) {
      screens[x].cur = all_menus[x];
      screens[x].track.obj = lv_scr_act();
      screens[x].track.active = true;
    } else {
      screens[x].cur = all_menus[x];
      screens[x].track.obj = lv_obj_create(NULL);
      screens[x].track.active = false;
    }
  }
  ran = true;
ret:
  return screens;
}
esp_err_t change_screen(enum menu change_to) {
  state *states = create_screens();

  for (size_t i = 0; i < NUM_OF_SCR; i++) {
    if (states[i].cur == change_to)
      lv_scr_load(states[i].track.obj);
    else
      return ESP_ERR_INVALID_ARG;
  }
  return ESP_OK;
}

state_opt *menu_init_arr() {
  static bool ran = false;
  static state_opt *ptrs = NULL;
  if (ran)
    goto ret;
  ptrs = heap_caps_malloc(sizeof(state_opt) * MAIN_OPT, MALLOC_CAP_INTERNAL);
  for (size_t i = 0; i < MAIN_OPT; i++) {
    ptrs[i].cur = menu_opt[i];
    ptrs[i].track.obj = lv_label_create(create_screens()[0].track.obj);
    if (i == 0)
      ptrs[i].track.active = true;
    else
      ptrs[i].track.active = false;
  }
  ran = true;
ret:
  return ptrs;
}
esp_err_t update_opt(struct current_state *current) {
  state_opt *checking = menu_init_arr();
  for (size_t i = 0; i < MAIN_OPT; i++) {
    state_opt check = checking[i];
    if (current->c_state == check.cur) {
      lv_obj_set_style_text_decor(checking[i].track.obj,
                                  LV_TEXT_DECOR_UNDERLINE, 0);
      checking[i].track.active = true;
    } else {
      // honestly this is for the previous as well as checking each one
      lv_obj_set_style_text_decor(checking[i].track.obj, LV_TEXT_DECOR_NONE, 0);
      checking[i].track.active = false;
    }
  }
  return ESP_OK;
}
esp_err_t start_up_menu() {
  if (create_screens()[0].track.obj != lv_scr_act())
    return ESP_ERR_WRONG_SCREEN;
  // based off of the menu_opt array
  state_opt *options = menu_init_arr();
  // logo
  lv_obj_t *logo = lv_label_create(lv_scr_act());
  lv_label_set_text(logo, "NULL by Jaden V.");
  lv_obj_set_style_text_font(logo, &lv_font_montserrat_14, 0);
  lv_obj_align(logo, LV_ALIGN_TOP_LEFT, 5, 5);
  // ARP label
  lv_obj_t *arp_label = options[0].track.obj;
  lv_label_set_text(arp_label, "ARP Spoofing");
  lv_obj_set_style_text_font(arp_label, &lv_font_montserrat_14, 0);
  lv_obj_align(arp_label, LV_ALIGN_LEFT_MID, 0, -10);
  // Lora label
  lv_obj_t *lora_label = options[3].track.obj;
  lv_label_set_text(lora_label, "lora emulation");
  lv_obj_set_style_text_font(lora_label, &lv_font_montserrat_14, 0);
  lv_obj_align_to(lora_label, arp_label, LV_ALIGN_OUT_BOTTOM_MID, 0, -0);
  // Lora label
  lv_obj_t *rfid_label = options[2].track.obj;
  lv_label_set_text(rfid_label, "RFID Transieving");
  lv_obj_set_style_text_font(rfid_label, &lv_font_montserrat_14, 0);
  lv_obj_align_to(rfid_label, lora_label, LV_ALIGN_OUT_BOTTOM_MID, 0, -0);
  // label
  lv_obj_t *ir_label = options[1].track.obj;
  lv_label_set_text(ir_label, "IR emulation");
  lv_obj_set_style_text_font(ir_label, &lv_font_montserrat_14, 0);
  lv_obj_align_to(ir_label, rfid_label, LV_ALIGN_OUT_BOTTOM_MID, 0, -0);

  return ESP_OK;
}
