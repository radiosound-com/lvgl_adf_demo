idf_component_register(
    SRCS main.c play_sdcard_mp3_control_example.c ui.c
    INCLUDE_DIRS .)

idf_component_get_property(LVGL_LIB lvgl__lvgl COMPONENT_LIB)
target_compile_options(
    ${LVGL_LIB} 
    PRIVATE
        -DLV_LVGL_H_INCLUDE_SIMPLE
)
