CC = arm-none-linux-gnueabi-gcc
CFLAG = -ljpeg -lcam -lcv -lhighgui -lcvaux -lml -lz -lcxcore  -lpthread -lm -g -lrt
LIBSO = -L../opencv \
			 -L../camlib
INCLUDE = -I../opencvinc \
			-I../tcc \
			-I../cam 

SRC = main.c
TARGET = test

all:
	$(CC) $(SRC) $(LIBSO) $(INCLUDE) $(CFLAG) -o $(TARGET)
clean:
	rm -rf test
	rm -rf main.o
