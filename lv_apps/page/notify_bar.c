/**
 * @todo 
 *      写滚轮亮度调节组件（lv_timer、 queue）
 */

#include "notify_bar.h"


#include "lv_apps/anim/anim.h"
#include "lv_apps/common/lv_common.h"


#define barStillTime 3000   // notify bar的停顿时间，单位ms
#define barTimerPeriod 50   // 定时器周期，单位ms
#define timerCount  (barStillTime/barTimerPeriod)   


static lv_obj_t * bar;
static lv_timer_t * bar_timer;
static int brightness;      // 亮度
static int last_brightness; // 最后一次亮度
static int timer_count;     // timer cb函数调用次数
bool barState = false;  // bar的显示状态，true 打开， false  关闭


static void brightness_add_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(brightness < 100){
            brightness++;
        }
    }
}
static void brightness_cut_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(brightness > 0){
            brightness--;
        }
    }
}

/* 定时器回调函数，更新 bar 状态 */
static void barTimer_cb(lv_timer_t *timer){
    if(brightness == last_brightness){
        timer_count ++;
        if(timer_count == timerCount){
            bar_anim_out(0);
        }
        return;
    }
    if(brightness != last_brightness){
        timer_count = 0;    //清零，重新计数
        if(!barState){
            bar_anim_in(0);
        }
        lv_bar_set_value(lv_obj_get_child(bar, 0), brightness, LV_ANIM_ON);
        lv_label_set_text_fmt(lv_obj_get_child(bar, 1), "%d", brightness);
        last_brightness = brightness;
    }
}


void create_brightnessBar(void){

    /* 亮度条背景 */
    bar = lv_obj_create(bg_screen);
    lv_obj_set_size(bar, 180, 45);      //bar 背景大小
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x2b2e35), 0);  //背景颜色
    lv_obj_set_style_radius(bar, 7, 0); // 圆角
    lv_obj_set_style_border_color(bar, lv_color_hex(0x16191c), 0);
    lv_obj_set_style_border_width(bar, 1, 0);
    lv_obj_set_style_shadow_color(bar, lv_color_hex(0x16191c),0);
    
    /* shadow 可以不要 */
    lv_obj_set_style_shadow_width(bar, 30, 0); 
    lv_obj_set_style_shadow_ofs_x(bar, 3, 0);
    lv_obj_set_style_shadow_ofs_y(bar, 3, 0);

    lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE); // 移除滑动flag
    lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 20);     
    

    /* 亮度条 */
    lv_obj_t * bar_line = lv_bar_create(bar); 
    lv_obj_set_size(bar_line, 120, 6);
    lv_obj_align(bar_line, LV_ALIGN_LEFT_MID, 5, 0);

    lv_obj_set_style_bg_color(bar_line, lv_color_hex(0x9e9fa3),LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_line, lv_color_hex(0x21ffe0),LV_PART_INDICATOR);
    lv_bar_set_range(bar_line, 0, 100);
    lv_bar_set_value(bar_line, brightness, LV_ANIM_ON);

    // lv_obj_add_event_cb(brightness_indicator,)  // 亮度条回调函数，

    /* 亮度数字 */
    lv_obj_t * bar_text = lv_label_create(bar);
    lv_obj_set_style_text_color(bar_text, lv_color_hex(0xffffff), 0);
    lv_obj_align(bar_text, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_label_set_text_fmt(bar_text, "%d", brightness);

    lv_obj_set_y(bar, -100);
}

void bar_anim_in(uint32_t delay){
    barState = true;
    anim_y_liner_in(bar, -100, 20, 0);
}

void bar_anim_out(uint32_t delay){
    barState = false;
    anim_y_liner_out(bar, 20, -100, delay);
}


/**
 * @brief 模拟调节 brightness 加减按钮
 * 
 */
void create_handle_btn(void){
    lv_obj_t * add = bosco_create_btn4(40, 35, brightness_add_cb, "add");
    lv_obj_align(add, LV_ALIGN_BOTTOM_MID, -50, -50);

    lv_obj_t * cut = bosco_create_btn4(40, 35, brightness_cut_cb, "cut");
    lv_obj_align(cut, LV_ALIGN_BOTTOM_MID, 50, -50);
}

void create_notify_timer(void){
    brightness = 50;
    last_brightness = brightness;
    bar_timer = lv_timer_create(barTimer_cb, barTimerPeriod, NULL);
}


/**
 * @todo
 *      重构动画，去掉部分 hide 的flag 和opa
 */