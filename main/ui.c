#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "bsp_board_extra.h"
#include "esp_log.h"
#include "esp_peripherals.h"
#include "input_key_service.h"
#include "lvgl.h"

/* for sending key events to the audio pipeline */
extern periph_service_handle_t input_ser;

/* Label to display current file name */
static lv_obj_t *file_label = NULL;
/* Button objects for UI controls */
static lv_obj_t *play_btn = NULL;
static lv_obj_t *skip_btn = NULL;
static lv_obj_t *vol_up_btn = NULL;
static lv_obj_t *vol_down_btn = NULL;

/* Function prototypes */
static void play_event_handler(lv_event_t * e);
static void skip_event_handler(lv_event_t * e);
static void vol_up_event_handler(lv_event_t * e);
static void vol_down_event_handler(lv_event_t * e);

void ui_set_file_label(char *file_name)
{
    /* Without taking the display mutex, you get lockups and crashes.
       Always take the mutex when modifying the UI. */
    if (bsp_display_lock(500))
    {
        lv_label_set_text(file_label, file_name);
        bsp_display_unlock();
    }
    else
    {
        ESP_LOGE("UI", "Failed to get display mutex, cannot modify UI");
    }
}

void ui_main(void)
{
    /* Create a label to display current file name */
    file_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(file_label, &lv_font_montserrat_24, 0);
    lv_obj_align(file_label, LV_ALIGN_TOP_MID, 0, 25);
    
    /* Create a label to show "Current File:" */
    lv_obj_t *file_label_title = lv_label_create(lv_scr_act());
    lv_label_set_text(file_label_title, "Current File:");
    lv_obj_align(file_label_title, LV_ALIGN_TOP_MID, 0, 5);
    
    /* Create Play button */
    play_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(play_btn, 100, 50);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_LEFT, 25, -25);
    lv_obj_add_event_cb(play_btn, play_event_handler, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *play_label = lv_label_create(play_btn);
    lv_label_set_text(play_label, "Play");
    lv_obj_center(play_label);

    /* Create Skip button */
    skip_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(skip_btn, 100, 50);
    lv_obj_align(skip_btn, LV_ALIGN_BOTTOM_LEFT, 135, -25);
    lv_obj_add_event_cb(skip_btn, skip_event_handler, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *skip_label = lv_label_create(skip_btn);
    lv_label_set_text(skip_label, "Skip");
    lv_obj_center(skip_label);
        
    /* Create Volume Up button */
    vol_up_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(vol_up_btn, 100, 50);
    lv_obj_align(vol_up_btn, LV_ALIGN_BOTTOM_RIGHT, -135, -25);
    lv_obj_add_event_cb(vol_up_btn, vol_up_event_handler, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *vol_up_label = lv_label_create(vol_up_btn);
    lv_label_set_text(vol_up_label, "Vol+");
    lv_obj_center(vol_up_label);
    
    /* Create Volume Down button */
    vol_down_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(vol_down_btn, 100, 50);
    lv_obj_align(vol_down_btn, LV_ALIGN_BOTTOM_RIGHT, -25, -25);
    lv_obj_add_event_cb(vol_down_btn, vol_down_event_handler, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *vol_down_label = lv_label_create(vol_down_btn);
    lv_label_set_text(vol_down_label, "Vol-");
    lv_obj_center(vol_down_label);
    
    /* Set initial file name */
    lv_label_set_text(file_label, "No file playing");
}

static void play_event_handler(lv_event_t * e)
{
    ESP_LOGI("UI", "Play button pressed");
    periph_service_event_t evt;
    evt.type = INPUT_KEY_SERVICE_ACTION_CLICK_RELEASE;
    evt.data = (void *)INPUT_KEY_USER_ID_PLAY;
    evt.len = 1;
    evt.source = 0;
    periph_service_callback(input_ser, &evt);
}

static void skip_event_handler(lv_event_t * e)
{
    ESP_LOGI("UI", "Skip button pressed");
    periph_service_event_t evt;
    evt.type = INPUT_KEY_SERVICE_ACTION_CLICK_RELEASE;
    evt.data = (void *)INPUT_KEY_USER_ID_SET;
    evt.len = 1;
    evt.source = 0;
    periph_service_callback(input_ser, &evt);
}


static void vol_up_event_handler(lv_event_t * e)
{
    ESP_LOGI("UI", "Volume Up button pressed");
    periph_service_event_t evt;
    evt.type = INPUT_KEY_SERVICE_ACTION_CLICK_RELEASE;
    evt.data = (void *)INPUT_KEY_USER_ID_VOLUP;
    evt.len = 1;
    evt.source = 0;
    periph_service_callback(input_ser, &evt);
}

static void vol_down_event_handler(lv_event_t * e)
{
    ESP_LOGI("UI", "Volume Down button pressed");
    periph_service_event_t evt;
    evt.type = INPUT_KEY_SERVICE_ACTION_CLICK_RELEASE;
    evt.data = (void *)INPUT_KEY_USER_ID_VOLDOWN;
    evt.len = 1;
    evt.source = 0;
    periph_service_callback(input_ser, &evt);
}
