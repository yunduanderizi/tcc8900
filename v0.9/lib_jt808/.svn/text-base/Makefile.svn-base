#CC=g++
CC=arm-none-linux-gnueabi-g++
CFLAGS = -c -static -I../../../../trunk/packages/target/libboost_1_49_0/include\
	 -I../../../../trunk/packages/target/sqlite3/include\
	 -I../../innov/utility/include\
	 -I../../innov/table/include\
	 -I../../innov/gb19056/include
LIBS = -L../../../../trunk/packages/target/libboost_1_49_0/lib -lboost_system -lboost_chrono -lboost_timer -lboost_program_options\
       -lboost_locale\
       -L../../../../trunk/packages/target/sqlite3/lib\
       -L../../innov/utility/lib -lutility\
       -L../../innov/table/lib -ltables\
       -L../../innov/gb19056/lib -lgb19056\
#       -lpthread -lrt  
objects = ConfigManager.o ConnectionManager.o JT808Manager.o JT808MessageHandler.o JT808MessageHelper.o JT808ApplicationHandler.o\
	  JT808FifoMessageHandler.o cpipe.o tool.o
#AR=ar
#all:libjt808.a
libjt808.a:$(objects)
	$(CC) -o libjt808.a ConfigManager.o ConnectionManager.o JT808Manager.o JT808MessageHandler.o JT808MessageHelper.o\
	       	JT808ApplicationHandler.o cpipe.o tool.o -shared
ConfigManager.o:ConfigManager.cpp ConfigManager.h
	$(CC) -c ConfigManager.cpp $(CFLAGS)
ConnectionManager.o:ConnectionManager.cpp ConnectionManager.h
	$(CC) -c ConnectionManager.cpp $(CFLAGS)
JT808Manager.o:JT808Manager.cpp
	$(CC) -c JT808Manager.cpp $(CFLAGS)
JT808MessageHandler.o:JT808MessageHandler.cpp JT808MessageHandler.h
	$(CC) -c JT808MessageHandler.cpp $(CFLAGS)
JT808MessageHelper.o:JT808MessageHelper.cpp JT808MessageHelper.h
	$(CC) -c JT808MessageHelper.cpp $(CFLAGS)
JT808ApplicationHandler.o:JT808ApplicationHandler.cpp JT808ApplicationHandler.h
	$(CC) -c JT808ApplicationHandler.cpp $(CFLAGS)
JT808FifoMessageHandler.o:JT808FifoMessageHandler.cpp
	$(CC) -c JT808FifoMessageHandler.cpp $(CFLAGS)
cpipe.o:cpipe.cpp
	$(CC) -c cpipe.cpp $(CFLAGS)
tool.o:tool.cpp
	$(CC) -c tool.cpp $(CFLAGS)
.PHONY:clean
clean:
	rm -f *.o libjt808.a
