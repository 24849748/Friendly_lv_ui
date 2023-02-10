#include "muyu_page.h"
#include "ctrl_page.h"
#include "lv_apps/anim/anim.h"
#include "lv_apps/common/lv_common.h"
#include <stdio.h>


/* #define */
LV_IMG_DECLARE(muyu);
LV_FONT_DECLARE(font_muyu);

/* static lv_obj_t *obj */
static lv_obj_t * muyu_return;      // 返回按钮
static lv_obj_t * muyu_bg;          // 木鱼黑色背景
static lv_obj_t * muyu_clickNum;    // 敲击统计数字
static lv_obj_t * muyu_text;

static uint32_t muyu_totalNum;
static uint8_t doubleClick;     // 记录点击屏幕上方木鱼数字obj次数


// /* 回调函数 */
static void return_ctrlpage_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("return ctrl page\n");
        muyu_page_anim_out(0);
        ctrl_page_anim_in(200);
    }
}


static void muyu_anim_cb(void * var, int32_t v){
    lv_img_set_zoom(var, v);
}

static void muyu_click_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * _muyu_img = lv_event_get_target(e);

    if(code ==  LV_EVENT_CLICKED){
        // 木鱼敲击动画
        lv_anim_t muyu_anim;
        lv_anim_init(&muyu_anim);
        lv_anim_set_var(&muyu_anim, _muyu_img);
        lv_anim_set_values(&muyu_anim, 255, 240);
        lv_anim_set_time(&muyu_anim, 100);
        lv_anim_set_playback_delay(&muyu_anim, 0);
        lv_anim_set_playback_time(&muyu_anim, 100);
        lv_anim_set_path_cb(&muyu_anim, lv_anim_path_linear);
        lv_anim_set_exec_cb(&muyu_anim, muyu_anim_cb);
        lv_anim_start(&muyu_anim);

        lv_anim_t text_anim;
        lv_anim_init(&text_anim);
        lv_anim_set_var(&text_anim, muyu_text);
        lv_anim_set_values(&text_anim, LV_OPA_COVER, LV_OPA_0);
        lv_anim_set_time(&text_anim, 150);
        lv_anim_set_exec_cb(&text_anim, anim_opa_cb);
        // lv_anim_set_ready_cb(&text_anim, lv_obj_del_anim_ready_cb);
        lv_anim_start(&text_anim);

        lv_anim_set_values(&text_anim, 40, 20);
        lv_anim_set_time(&text_anim, 150);
        lv_anim_set_path_cb(&text_anim, lv_anim_path_linear);
        lv_anim_set_exec_cb(&text_anim, anim_y_cb);
        lv_anim_start(&text_anim);

        // lv_anim_set_values(&text_anim, LV_OPA_COVER, LV_OPA_0);
        // lv_anim_set_time(&text_anim, 150);
        // lv_anim_set_exec_cb(&text_anim, anim_opa_cb);
        // lv_anim_set_ready_cb(&text_anim, lv_obj_del_anim_ready_cb);
        // lv_anim_start(&text_anim);

        doubleClick = 0;
        muyu_totalNum += 1;
        lv_label_set_text_fmt(muyu_clickNum, "%d", muyu_totalNum);
    }
}

static void muyu_totalNum_cb(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code ==  LV_EVENT_SHORT_CLICKED){
        doubleClick += 1;
        if(doubleClick == 2){
            muyu_totalNum = 0;
            lv_label_set_text_fmt(muyu_clickNum, "%d", muyu_totalNum);
            doubleClick = 0;
        }
    }
}


/*************************
 *      绘制页面内容
 *************************/

/* 创建木鱼对象 */
void create_muyu_obj(void){
    // 纯黑色背景
    muyu_bg = lv_obj_create(bg_screen);
    lv_obj_set_size(muyu_bg, 240, 320);
    lv_obj_set_style_bg_color(muyu_bg, lv_color_black(), 0);
    lv_obj_set_style_border_width(muyu_bg, 0, 0);
    lv_obj_center(muyu_bg);

    // muyu_return = create_return_button(return_mainpage_cb);
    muyu_return = lv_img_create(muyu_bg);
    lv_obj_set_style_translate_y(muyu_return, 5, LV_STATE_PRESSED);
    lv_obj_align(muyu_return, LV_ALIGN_TOP_LEFT, 10, COORD_RETURN_Y);
    lv_obj_add_flag(muyu_return, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(muyu_return, return_ctrlpage_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_ext_click_area(muyu_return, 20); // 拓展点击区域
    LV_IMG_DECLARE(logo_return);        // 使用返回图片
    lv_img_set_src(muyu_return, &logo_return); 

    // 木鱼图片
    lv_obj_t * muyu_img = lv_img_create(muyu_bg);
    lv_img_set_src(muyu_img, &muyu);
    lv_obj_center(muyu_img);
    lv_obj_add_flag(muyu_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(muyu_img, muyu_click_cb, LV_EVENT_CLICKED, NULL);

    muyu_totalNum = 0;
    doubleClick = 0;
    muyu_clickNum = lv_label_create(muyu_bg);
    lv_obj_set_style_text_color(muyu_clickNum, lv_color_white(), 0);
    lv_obj_set_style_text_font(muyu_clickNum, &font_muyu, 0);
    lv_label_set_text_fmt(muyu_clickNum, "%d", muyu_totalNum);
    lv_obj_align(muyu_clickNum, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_flag(muyu_clickNum, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(muyu_clickNum, muyu_totalNum_cb, LV_EVENT_SHORT_CLICKED, NULL);

    // 木鱼文字
    muyu_text = lv_label_create(muyu_bg);
    lv_obj_set_style_text_color(muyu_text, lv_color_white(), 0);
    lv_obj_set_style_text_font(muyu_text, &font_muyu, 0);
    lv_label_set_text(muyu_text, "功德+1");
    lv_obj_align(muyu_text, LV_ALIGN_TOP_MID, 50, 40);
    lv_obj_set_style_opa(muyu_text, LV_OPA_0, 0);
}

/*************************
 *        页面动画
 *************************/

void muyu_page_anim_in(uint32_t delay){
    anim_y_fade_in(muyu_return,-50, COORD_RETURN_Y, delay);
    anim_step_in(muyu_bg, delay);
    anim_step_in(muyu_clickNum, delay);
}
void muyu_page_anim_out(uint32_t delay){
    anim_y_fade_out(muyu_return, COORD_RETURN_Y, -50, delay);
    anim_step_out(muyu_bg, delay);
    anim_step_out(muyu_clickNum, delay);
}

/*************************
 *        页面API
 *************************/

void muyu_page_hide_obj(void){
    lv_obj_add_flag(muyu_return, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(muyu_bg, LV_OBJ_FLAG_HIDDEN);
}

void muyu_page_create_obj(void){
    create_muyu_obj();

    muyu_page_hide_obj();
}



void muyu_page_anim_in(uint32_t delay);  //进入新页面api
void muyu_page_anim_out(uint32_t delay); //不常调用此函数

void muyu_page_hide_obj(void);           //不常调用此函数
void muyu_page_create_obj(void);         //在程序最开始创建