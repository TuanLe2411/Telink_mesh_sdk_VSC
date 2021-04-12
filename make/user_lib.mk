OUT_DIR += /user_lib

OBJS += \
$(OUT_PATH)/user_lib/serial.o \

$(OUT_PATH)/user_lib/%.o: $(TEL_PATH)/firmware/user_lib/%.c
	@echo 'Building file: $<'
	@tc32-elf-gcc $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"
