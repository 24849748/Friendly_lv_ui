## 该仓库为本人自学LVGL所写的demo，欢迎学习交流！

该项目在Linux平台上模拟运行，使用LVGL v8.2.0版本，基于官方Linux模拟器工程修改：

- 删除部分example、demo和其他无关文件
- 修改目录结构
- 使用自己的demo程序

后续不同UI将以分支形式push，不再另建仓库。

***

## 以下记录自己搭建的过程：

**在Linux平台搭建LVGL模拟器**

1. 安装SDL2

   ```shell
   sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
   ```

2. 构建目录

   ```shell
   .
   ├── build
   │   └── bin				// 可执行文件生成目录
   ├── lv_apps				// 存放ui程序
   ├── lv_drivers			
   ├── lvgl
   ├── lv_conf.h			// lvgl库配置头文件
   ├── lv_drv_conf.h		// lv_drivers库配置头文件
   ├── main.c				// 主程序
   └── Makefile
   ```

3. 拉取lvgl库和lvgl_drivers库

   ```shell
   git clone -b release/v8.2 https://github.com/lvgl/lvgl.git
   git clone -b release/v8.2 https://github.com/lvgl/lv_drivers.git
   ```

4. lv_conf.h和lv_drv_conf.h

   ```shell
   cp lvgl/lv_conf_template.h lv_conf.h
   cp lv_drivers/lv_drv_conf_template.h lv_drv_conf.h
   ```

   - 启用两个文件，#if 1
   - lv_drv_conf.h配置需要开启USE_MONITOR、USE_MOUSE、USE_MOUSEWHEEL和USE_KEYBOARD
   - 其他按需求修改

5. 编写Makefile

   ```makefile
   #修改自官方Linux模拟工程：https://github.com/lvgl/lv_sim_vscode_sdl
   PROJECT 			?= lv_sim_sdl
   MAKEFLAGS 			:= -j $(shell nproc)
   SRC_EXT      		:= c
   OBJ_EXT				:= o
   CC 					?= gcc
   
   SRC_DIR				:= ./
   WORKING_DIR			:= ./build
   BUILD_DIR			:= $(WORKING_DIR)/obj
   BIN_DIR				:= $(WORKING_DIR)/bin
   
   WARNINGS 			:= -Wall -Wextra \
   						-Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wno-discarded-qualifiers \
   						-Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized \
   						-Wno-unused-parameter -Wno-missing-field-initializers -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default  \
   					  	-Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated  \
   						-Wempty-body -Wshift-negative-value -Wstack-usage=2048 \
               			-Wtype-limits -Wsizeof-pointer-memaccess -Wpointer-arith
   
   CFLAGS 				:= -O0 -g $(WARNINGS)
   
   # Add simulator define to allow modification of source
   DEFINES				:= -D SIMULATOR=1 -D LV_BUILD_TEST=0
   
   # Include simulator inc folder first so lv_conf.h from custom UI can be used instead
   # INC 				:= -I./ui/simulator/inc/ -I./ -I./lvgl/
   INC 				:= -I./lv_apps/ -I./ -I./lvgl/
   LDLIBS	 			:= -lSDL2 -lm
   BIN 				:= $(BIN_DIR)/demo
   
   COMPILE				= $(CC) $(CFLAGS) $(INC) $(DEFINES)
   
   # Automatically include all source files
   SRCS 				:= $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '*/\.*')
   OBJECTS    			:= $(patsubst $(SRC_DIR)%,$(BUILD_DIR)/%,$(SRCS:.$(SRC_EXT)=.$(OBJ_EXT)))
   
   all: default
   
   $(BUILD_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
   	@echo 'Building project file: $<'
   	@mkdir -p $(dir $@)
   	@$(COMPILE) -c -o "$@" "$<"
   
   default: $(OBJECTS)
   	@mkdir -p $(BIN_DIR)
   	$(CC) -o $(BIN) $(OBJECTS) $(LDFLAGS) ${LDLIBS}
   
   clean:
   	rm -rf $(WORKING_DIR)
   
   install: ${BIN}
   	install -d ${DESTDIR}/usr/lib/${PROJECT}/bin
   	install $< ${DESTDIR}/usr/lib/${PROJECT}/bin/
   
   ```

6. 修改main.c头文件，去除模拟器鼠标指针

