CXX = c++ 
CXXFLAGS = -std=c++17 -O3 #-Wall -Wextra -Werror
LDFLAGS = -fsanitize=address -g3

SRC_DIR = ./src

OBJ_DIR = ./

INC_DIR = ./include

LIB_DIR = ./lib-universal

GLFW_LIB_DIR = $(LIB_DIR)


TARGET = ./particle

SOURCES = $(wildcard $(SRC_DIR)/*.cpp ./imgui/*.cpp ./stableFluidsScene/*.cpp ./particleScene/*.cpp)

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(patsubst ./imgui/%.cpp,$(OBJ_DIR)/%.o,$(patsubst ./stableFluidsScene/%.cpp,$(OBJ_DIR)/%.o,$(patsubst ./particleScene/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES)))))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ -L$(GLFW_LIB_DIR) -lglfw3 -lGLEW -framework Cocoa -framework OpenGL -framework IOKit -framework OpenCL

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -Iimgui/$(INC_DIR) -IstableFluidsScene/$(INC_DIR) -IparticleScene/$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: ./imgui/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -Iimgui/$(INC_DIR) -IstableFluidsScene/$(INC_DIR) -IparticleScene/$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: ./stableFluidsScene/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -Iimgui/$(INC_DIR) -IstableFluidsScene/$(INC_DIR) -IparticleScene/$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: ./particleScene/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -Iimgui/$(INC_DIR) -IstableFluidsScene/$(INC_DIR) -IparticleScene/$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

fclean: clean
	rm -f $(TARGET)

re : 
	${MAKE} fclean 
	${MAKE} all

.PHONY: all clean fclean re