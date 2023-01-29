CC=g++
CFLAGS=-I.
LIBS = `pkg-config --cflags --libs opencv4`
LDLIBS = -ltiff -lpng -ljpeg -lz -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect

DEPSF = ../include
DEPS = filter.h
IMG = imgDisplay.cpp filter.o 
VID = vidDisplay.cpp filter.o

all: img_output vid_output 

%.o: %.cpp $(DEPSF)/$(DEPS)
	$(CC) -c -o $@ $< $(LIBS)

img_output: $(IMG)
	$(CC) -o $@ $^ $(LIBS)

vid_output: $(VID)
	$(CC) -o  $@ $^ $(LIBS)

clean:
	rm -f *.o *~

# CC = g++
# PROJECT = new_output
# SRC = test.cpp
# LIBS = `pkg-config --cflags --libs opencv4`
# $(PROJECT) : $(SRC)
# 	$(CC) $(SRC) -o $(PROJECT) $(LIBS)