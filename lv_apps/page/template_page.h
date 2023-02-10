#ifndef _TEMPLATE_PAGE_H_
#define _TEMPLATE_PAGE_H_

#include "lvgl/lvgl.h"

void xxx_page_anim_in(uint32_t delay);  //进入新页面api
void xxx_page_anim_out(uint32_t delay); //不常调用此函数

void xxx_page_hide_obj(void);           //不常调用此函数
void xxx_page_create_obj(void);         //在程序最开始创建

#endif
