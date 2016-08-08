CXX = g++
INC_DIR = include
CXXFLAGS = -std=c++11 -I $(INC_DIR) 
LIB = -pthread

SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)

# preprocessor flags
CPPFLAGS := -MMD -MP 

myhttpd: $(OBJ)
	$(CXX) -o $@ $^ $(LIB)
    
-include $(DEP)

clean:
	$(RM) myhttpd $(OBJ) $(DEP)
