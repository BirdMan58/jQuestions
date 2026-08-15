CC = gcc

TARGET = main
SRC = main.c 

build:
	$(CC) $(SRC) -o $(TARGET)
run: build
	./$(TARGET)
clean:
	rm -f $(TARGET)
	rm -f data/index.txt
	touch data/index.txt
	rm -rf data/questions/*