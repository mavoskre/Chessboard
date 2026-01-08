CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = chess_project
SOURCES = main.cpp ChessEngine.cpp
GIF = chess_study.gif

all: $(TARGET)
	./$(TARGET)
	open $(GIF)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) move_*.png $(GIF)

run:
	./$(TARGET)
	open $(GIF)