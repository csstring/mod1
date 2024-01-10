CXX = c++ 
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Werror
LDFLAGS = #-fsanitize=address -g

SRC_DIR = ./src

OBJ_DIR = ./

INC_DIR = ./include

LIB_DIR = ./lib-universal

GLFW_LIB_DIR = $(LIB_DIR)


TARGET = ./particle

SOURCES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/imgui/*.cpp)

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(patsubst $(SRC_DIR)/imgui/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES)))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ -L$(GLFW_LIB_DIR) -lglfw3 -lGLEW -framework Cocoa -framework OpenGL -framework IOKit -framework OpenCL

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(INC_DIR)/imgui -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(INC_DIR)/imgui -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

fclean: clean
	rm -f $(TARGET)

re : 
	${MAKE} fclean 
	${MAKE} all

.PHONY: all clean fclean re