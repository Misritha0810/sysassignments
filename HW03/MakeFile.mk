CC=gcc
CFLAGS=-Wall -g
TARGET=Mmaridi
SOURCES=misrita_hw3.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: cleanall

cleanall: clean
	rm -f *.o
