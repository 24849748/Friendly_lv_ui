#include "template_page.h"

#include "main_page.h"
#include "lv_apps/anim/anim.h"
#include "lv_apps/common/lv_common.h"
#include <stdio.h>


/* #define */

/* static lv_obj_t *obj */

/* 回调函数 */
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

void create_something1(void){

}
void create_something2(void){

}


/*************************
 *        页面动画
 *************************/

void xxx_page_anim_in(uint32_t delay){
    // anim_y_fade_in(xxxx, -50, INFO_RETURN_Y, delay);
    //...
}
void xxx_page_anim_out(uint32_t delay){
    // anim_y_fade_out(xxxx, INFO_RETURN_Y, -50, delay);
    //...
}

/*************************
 *        页面API
 *************************/

void xxx_page_hide_obj(void){
    // lv_obj_add_flag(xxxxx, LV_OBJ_FLAG_HIDDEN);
}

void xxx_page_create_obj(void){
    create_something1();
    create_something2();

    xxx_page_hide_obj();
}