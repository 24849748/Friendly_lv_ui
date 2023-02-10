#include "start_page.h"
#include "main_page.h"

#include "lv_apps/common/lv_common.h"
#include "lv_apps/anim/anim.h"

/* #define */

#define COORD_GIF_X 15
#define COORD_GIF_Y -35

#define START_TIME  1000     // load界面执行时间

LV_IMG_DECLARE(loadimg1);
LV_IMG_DECLARE(loadimg2);
LV_IMG_DECLARE(loadimg3);
LV_IMG_DECLARE(loadimg4);
const lv_img_dsc_t *dsc_imgs_load[4] = {
    &loadimg4,
    &loadimg3,
    &loadimg2,
    &loadimg1,
};


/* static lv_obj_t *obj */
// static lv_obj_t * startPageBG;  //加载页面背景（主对象）
// lv_obj_t * loadBar;
// lv_obj_t * loadLabel; 
// lv_obj_t * animing_load; 


/* 回调函数 */
static void anim_barValue_cb(void * obj, int32_t value){
    lv_bar_set_value(obj, value, LV_ANIM_ON);
}

void obj_del_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, NULL);
    lv_anim_set_time(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_ready_cb(&a, lv_obj_del_anim_ready_cb);
    lv_anim_start(&a);
}

void main_page_in_ready_cb(lv_anim_t * a){
    main_page_anim_in(200);
}

/* static void bar_event_cb(lv_event_t * e){
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
    if(dsc->part != LV_PART_INDICATOR) return;

    lv_obj_t * obj=lv_event_get_target(e);
    
    lv_coord_t coord_x = (lv_bar_get_value(obj)*2);
    lv_obj_align(animing_load, LV_ALIGN_LEFT_MID, coord_x, COORD_GIF_Y);
    // lv_obj_align(lv_obj_get_child(startPageBG,0), LV_ALIGN_LEFT_MID, coord_x, COORD_GIF_Y);
    if(lv_bar_get_value(obj) == 95){
        // lv_obj_add_flag(loadBar, LV_OBJ_FLAG_HIDDEN);
        // lv_obj_add_flag(loadLabel, LV_OBJ_FLAG_HIDDEN);
        // lv_obj_add_flag(animing_load, LV_OBJ_FLAG_HIDDEN);
        
        lv_obj_del(loadBar);
        lv_obj_del(loadLabel);
        lv_obj_del(animing_load);
        main_page_anim_in(200);
    }
} */


/*************************
 *      绘制页面内容
 *************************/

void start_page(void){
    
    lv_obj_t * loadBar;
    lv_obj_t * loadLabel; 
    lv_obj_t * loadGif; 

    /* gif 动图 */
    loadGif = lv_animimg_create(bg_screen);
    // lv_obj_t * animing_load = lv_animimg_create(startPageBG);
    lv_obj_align(loadGif, LV_ALIGN_LEFT_MID, COORD_GIF_X, COORD_GIF_Y);
    lv_animimg_set_src(loadGif,  (lv_img_dsc_t**)dsc_imgs_load, 4);
    lv_animimg_set_duration(loadGif, 250);
    lv_animimg_set_repeat_count(loadGif, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(loadGif);

    /* Loading文本 */
    loadLabel = lv_label_create(bg_screen);
    // lv_obj_t * loadLabel = lv_label_create(startPageBG);
    lv_label_set_text(loadLabel, "Loading...");
    lv_obj_align(loadLabel, LV_ALIGN_CENTER, 0, 20);

    /* 进度条 */
    // lv_obj_t * loadBar = lv_bar_create(startPageBG);
    loadBar = lv_bar_create(bg_screen);
    lv_obj_set_size(loadBar, 200, 7);       // 进度条大小
    lv_obj_center(loadBar);                 // 进度条位置
    lv_bar_set_range(loadBar, 5, 95);      // 进度条范围    
    lv_obj_set_style_bg_color(loadBar, lv_color_hex(0xfdd86d), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(loadBar, lv_color_hex(0xa9edb9), LV_PART_MAIN);
    // lv_obj_add_event_cb(loadBar, bar_event_cb, LV_EVENT_DRAW_PART_END, NULL);

    /* 进度条动画 */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, loadBar);
    lv_anim_set_exec_cb(&a, anim_barValue_cb);
    lv_anim_set_time(&a, START_TIME);
    lv_anim_set_values(&a, 5, 95);
    lv_anim_set_delay(&a, 500);
    lv_anim_start(&a);

    lv_anim_set_var(&a, loadGif);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_time(&a, START_TIME);
    lv_anim_set_values(&a, COORD_GIF_X, 185);
    lv_anim_set_delay(&a, 500);
    lv_anim_start(&a);

    lv_anim_set_var(&a, loadLabel);
    lv_anim_set_exec_cb(&a, NULL);
    lv_anim_set_time(&a, 1);
    lv_anim_set_delay(&a, START_TIME+300);
    lv_anim_set_ready_cb(&a, main_page_in_ready_cb);
    lv_anim_start(&a);

    obj_del_delayed(loadGif, START_TIME+500);
    obj_del_delayed(loadBar, START_TIME+500);
    obj_del_delayed(loadLabel, START_TIME+500);
}
