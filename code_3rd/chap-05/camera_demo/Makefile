CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
LDFLAGS =
LDLIBS = -lglut -lGL -lGLU -lm

SOURCES = $(shell ls *.cpp)
#OBJECTS = $(SOURCES:%.cpp=%.o)
OBJECTS = $(subst .cpp,.o,$(SOURCES))
TARGET = Transform

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"
