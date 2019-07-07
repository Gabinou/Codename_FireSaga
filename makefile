
CXX	= c++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 
main.exe:$(SRC)/main.cpp
    $(CXX) $(SRC)/main.cpp  -o $(BLD)/main.exe $(INCLUDE)