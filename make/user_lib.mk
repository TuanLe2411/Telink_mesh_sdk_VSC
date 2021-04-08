OUT_DIR += /user_lib

OBJS += \
$(OUT_PATH)/user_lib/app_lcd_handler.o \
$(OUT_PATH)/user_lib/app_serial.o \
$(OUT_PATH)/user_lib/app_lcd_model.o \

$(OUT_PATH)/user_lib/%.o: $(TEL_PATH)/firmware/user_lib/%.c
	@echo 'Building file: $<'
	@tc32-elf-gcc $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"
