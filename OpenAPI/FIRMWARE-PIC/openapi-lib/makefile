CC_OPTIONS=-mcpu=cortex-m4 -mthumb -msingle-pic-base -fPIC -Wall
GCC_PATH=C:/Users/1124/.platformio/packages/toolchain-gccarmnoneeabi/bin/

all:
	@echo   * Creating OpenAPI PIC Library
	$(GCC_PATH)arm-none-eabi-gcc $(CC_OPTIONS) -g -Os -c OpenAPI-shared.c -o OpenAPI-shared.o
	$(GCC_PATH)arm-none-eabi-gcc -shared -Wl,-soname,libopenapi.a -nostdlib -o libopenapi.a OpenAPI-shared.o
