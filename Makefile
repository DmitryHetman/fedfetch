CC=gcc
W=-Wall -Wextra -Wpedantic -Wno-unused-result
CFLAGS=-O2 -march=native -s -fno-stack-protector -fno-plt -no-pie -pipe -o
TARGET=fedfetch
static:
	$(CC) $(W) $(CFLAGS) $(TARGET) $(TARGET).c
	strip -s $(TARGET)
edit:
	$(EDITOR) $(TARGET).c
install:
	mv fedfetch /usr/bin/
uninstall:
	rm /usr/bin/fedfetch
clean:
	rm archfetch
