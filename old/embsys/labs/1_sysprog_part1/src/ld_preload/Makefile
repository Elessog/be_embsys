TARGET_NAME = libhook.so
GCC = gcc

all : hook

hook:
	@echo "TODO : create a shared library with hook.c"
	$(GCC) -c -fPIC hook.c -o hook.o
	$(GCC) -shared -Wl,-soname,$(TARGET_NAME) -o $(TARGET_NAME) hook.o -ldl -lm
