#include "info_page.h"
#include "main_page.h"
#include "lv_apps/anim/anim.h"
#include "lv_apps/common/lv_common.h"
#include <stdio.h>

/* 坐标信息,对齐左上角LV_ALIGN_TOP_LEFT */
#define INFO_RETURN_Y       15   

#define INFO_LINE_X         25     //info line x坐标
#define FirstLineINFO_Y     60     //第一行 info y坐标
#define LastLineINFO_Y      FirstLineINFO_Y + 130 + 20    //最后一行 info y坐标


static lv_obj_t * info_return;      //左上角返回键
static lv_obj_t * info_line;        //信息栏左边
static lv_obj_t * axp_info;         //axp173相关信息
static lv_obj_t * axp_inter_temp;       
static lv_obj_t * axp_info_percentage;         //axp173相关信息
static lv_obj_t * axp_info_volt;         //axp173相关信息

static lv_obj_t * sys_info;         //系统相关信息
static lv_obj_t * sys_info_flash;   //flash
static lv_obj_t * sys_info_heap;    //ram

// static lv_obj_t * bat_bg_img;
// static lv_obj_t * bat_label;


static lv_timer_t * info_timer;

int32_t data_bat,data_flash,data_heap;
float data_volt, data_inter_temp;


/* 按钮回调函数 */
static void return_mainpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("return main page\n");
        info_page_anim_out(200);
        main_page_anim_in(200);
    }
}


/*************************
 *      绘制页面内容
 *************************/

/* info 页面返回按键 */
void create_info_return_btn(void){
    /* 返回键 */
    info_return = create_return_button(return_mainpage_cb);
}


/* info左边线条 */
void create_info_line(void){
    static lv_point_t points[] = { 
        {INFO_LINE_X, FirstLineINFO_Y},     //line起点
        {INFO_LINE_X, LastLineINFO_Y} };    //line终点
    static lv_style_t styleline;
    lv_style_init(&styleline);
    lv_style_set_line_color(&styleline, lv_color_white());
    lv_style_set_line_width(&styleline, 3);

    info_line = lv_line_create(bg_screen);
    lv_line_set_points(info_line, points, 2);
    lv_obj_add_style(info_line, &styleline, 0);
    lv_obj_align(info_line, LV_ALIGN_TOP_LEFT, 0, 0);
}


/**
 * @brief 快速创建一个info label对象，并设置文字内容、对齐左上角，内容需要重新设定
 */
lv_obj_t * lv_create_info_label(lv_obj_t *parent, lv_coord_t y){
    lv_obj_t * obj = lv_label_create(parent);
    lv_label_set_recolor(obj,true);
    lv_obj_align(obj, LV_ALIGN_TOP_LEFT, INFO_LINE_X+10, y);
    lv_obj_set_style_text_color(obj, lv_color_white(), 0);
    return obj;
}



/* 更新info */
void info_axp_update(void){
    data_inter_temp = data_inter_temp + 0.1;
    data_bat = data_bat + 1;
    data_volt = data_volt + 0.1;

    lv_label_set_text_fmt(axp_inter_temp, "Temp: %.1f%%", data_inter_temp);
    lv_label_set_text_fmt(axp_info_percentage, "Bat:%d%%", data_bat);
    lv_label_set_text_fmt(axp_info_volt, "Volt:%.2f V", data_volt);
}
void info_sys_update(void){
    data_flash = data_flash + 16;
    data_heap = data_heap + 21;

    lv_label_set_text_fmt(sys_info_flash, "flash: %d mb", data_flash);
    lv_label_set_text_fmt(sys_info_heap, "free heap: %d bytes", data_heap);
}

/**
 * @brief 创建 axp info 文字对象
 *      包括： 充电状态 电池电压/百分比 当前功率
 */
void create_axp_info(void){
    // axp_info = lv_label_create(bg_screen);
    // lv_label_set_recolor(axp_info, true);
    // lv_label_set_text(axp_info, "axp173 : charging\nVolt: 3.7V\nPercent: 40%\nPower: 0.14w ");
    // lv_label_set_text(axp_info, "axp173 : charging");
    // lv_obj_align(axp_info, LV_ALIGN_TOP_LEFT, INFO_LINE_X + 10, FirstLineINFO_Y);

    data_bat = 2;
    data_volt = 3.2;
    data_inter_temp = 23.1;

    axp_info = lv_create_info_label(bg_screen, FirstLineINFO_Y);
    axp_inter_temp = lv_create_info_label(bg_screen, FirstLineINFO_Y+20);
    axp_info_percentage = lv_create_info_label(bg_screen, FirstLineINFO_Y+40);
    axp_info_volt = lv_create_info_label(bg_screen, FirstLineINFO_Y+60);

    lv_label_set_text(axp_info, LV_SYMBOL_BATTERY_2 "AXP173");
    info_axp_update();

    // lv_label_set_text_fmt(axp_inter_temp, "Power:%d w", data_power);
    // lv_label_set_text_fmt(axp_info_percentage, "Bat:%d%%",data_bat);
    // lv_label_set_text_fmt(axp_info_volt, "Volt:%.2f V", data_volt);

}

/**
 * @brief 创建 sys info 文字对象
 *      包括： cpu型号  flash大小  剩余堆容量  mac地址
 */
void create_sys_info(void){
    // sys_info = lv_label_create(bg_screen);
    // lv_label_set_recolor(sys_info, true);
    // lv_label_set_text(sys_info, "system : esp32S3\nflash: 123 Mb\nfree heap size: 51 bytes");
    // lv_obj_align(sys_info, LV_ALIGN_TOP_LEFT, INFO_LINE_X + 10, 160);

    data_flash = 2;
    data_heap = 213;

    sys_info = lv_create_info_label(bg_screen, FirstLineINFO_Y+90);
    sys_info_flash = lv_create_info_label(bg_screen, FirstLineINFO_Y+110);
    sys_info_heap = lv_create_info_label(bg_screen, FirstLineINFO_Y+130);
    lv_label_set_text(sys_info, LV_SYMBOL_HOME " ESP32 S3");
    
    // // lv_label_set_text(sys_info_flash, "sys_info_flash");
    // // lv_label_set_text(sys_info_heap, "sys_info_heap");
    // lv_label_set_text_fmt(sys_info_flash, "flash: %d mb", data_flash);
    // lv_label_set_text_fmt(sys_info_heap, "free heap: %d bytes", data_heap);

    info_sys_update();
}


/* axp173、系统资源 定时器更新函数 */
static void axp_sys_timer_update(lv_timer_t *timer){
    // /* get data */
    // data_power = data_power + 2;
    // data_bat = data_bat + 1;
    // data_volt = data_volt + 0.1;
    // data_flash = data_flash + 16;
    // data_heap = data_heap + 21;

    // // lv_label_set_text_fmt(axp_info, "");
    // lv_label_set_text_fmt(axp_inter_temp, "Power:%d w", data_power);
    // lv_label_set_text_fmt(axp_info_percentage, "Bat:%d%%", data_bat);
    // lv_label_set_text_fmt(axp_info_volt, "Volt:%.2f V", data_volt);

    // // lv_label_set_text_fmt(sys_info, "ESP32 S3");
    // lv_label_set_text_fmt(sys_info_flash, "flash: %d mb", data_flash);
    // lv_label_set_text_fmt(sys_info_heap, "free heap: %d bytes", data_heap);

    info_axp_update();
    info_sys_update();
}

/* 创建label 内容刷新定时器 */
void create_axp_sys_timer(void){
    info_timer = lv_timer_create(axp_sys_timer_update, 1000, NULL);
}


/*************************
 *        页面动画
 *************************/

/* info页面动画 */
void info_page_anim_in(uint32_t delay){
    anim_y_fade_in(info_return, -50, INFO_RETURN_Y, delay);
    anim_y_fade_in(info_line, -100, 0, delay);

    anim_x_fade_in(axp_info, -50, 40, delay+50);
    anim_x_fade_in(axp_inter_temp, -50, 40, delay+100);
    anim_x_fade_in(axp_info_percentage, -50, 40, delay+150);
    anim_x_fade_in(axp_info_volt, -50, 40, delay+200);
    
    anim_x_fade_in(sys_info, -50, 40, delay+250);
    anim_x_fade_in(sys_info_flash, -50, 40, delay+300);
    anim_x_fade_in(sys_info_heap, -50, 40, delay+350);
    
    create_axp_sys_timer();
}
void info_page_anim_out(uint32_t delay){
    anim_y_fade_out(info_return, INFO_RETURN_Y, -50, delay);
    anim_y_fade_out(info_line, 0, 100, delay);

    anim_x_fade_out(axp_info, 40, 100, delay+10);
    anim_x_fade_out(axp_inter_temp, 40, 100, delay+20);
    anim_x_fade_out(axp_info_percentage, 40, 100, delay+30);
    anim_x_fade_out(axp_info_volt, 40, 100, delay+40);

    anim_x_fade_out(sys_info, 40, 100, delay+50);
    anim_x_fade_out(sys_info_flash, 40, 100, delay+60);
    anim_x_fade_out(sys_info_heap, 40, 100, delay+70);

    lv_timer_del(info_timer);
}

/*************************
 *        页面API
 *************************/

void info_page_hide_obj(void){
    lv_obj_add_flag(info_return, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(info_line, LV_OBJ_FLAG_HIDDEN);
    
    lv_obj_add_flag(axp_info, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(axp_info_volt, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(axp_inter_temp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(axp_info_percentage, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(sys_info, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(sys_info_flash, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(sys_info_heap, LV_OBJ_FLAG_HIDDEN);
}

void info_page_create_obj(void){
    create_info_return_btn();
    create_info_line();
    create_axp_info();
    create_sys_info();


    info_page_hide_obj();
}


/**
 * 新文字信息顺序：
 * 1.create_xxx_info();
 * 2.info_xxx_update();
 * 3.info_page_hide_obj();
 * 4.anim_in/out();
 * 5.修改create_info_line();
 */