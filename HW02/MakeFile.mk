CC=gcc
CFLAGS=-Wall -g
TARGET=Mmaridi
SOURCES=Mmaridi_HW02.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: cleanall

cleanall: clean
	rm -f *.o
