

CXX	= c++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 

all: $(BLD)/main.exe

$(BLD)/main.o: $(SRC)/main.cpp
	$(CXX) -c $(SRC)/main.cpp  -o $(BLD)/main.o $(INCLUDE)

$(BLD)/unit.o: $(SRC)/unit.cpp
	$(CXX) -c $(SRC)/unit.cpp  -o $(BLD)/unit.o $(INCLUDE)

$(BLD)/weapon.o: $(SRC)/weapon.cpp
	$(CXX) -c $(SRC)/weapon.cpp  -o $(BLD)/weapon.o $(INCLUDE)
    
$(BLD)/main.exe: $(BLD)/main.o $(BLD)/unit.o $(BLD)/weapon.o
	$(CXX) $(BLD)/main.o $(BLD)/unit.o $(BLD)/weapon.o -o $(BLD)/main.exe $(INCLUDE)
