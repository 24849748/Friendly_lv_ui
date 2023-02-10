#ifndef _MUYU_PAGE_H_
#define _MUYU_PAGE_H_

#include "lvgl/lvgl.h"


void muyu_page_anim_in(uint32_t delay);  //进入新页面api
void muyu_page_anim_out(uint32_t delay); //不常调用此函数

void muyu_page_hide_obj(void);           //不常调用此函数
void muyu_page_create_obj(void);         //在程序最开始创建

#endif