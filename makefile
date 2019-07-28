

CXX	= c++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 

all: $(BLD)/main.exe

$(BLD)/main.o: $(SRC)/main.cpp
	$(CXX) -c $(SRC)/main.cpp -o $@ $(INCLUDE)

$(BLD)/unit.o: $(SRC)/unit.cpp
	$(CXX) -c $(SRC)/unit.cpp  -o $@ $(INCLUDE)

$(BLD)/weapon.o: $(SRC)/weapon.cpp
	$(CXX) -c $(SRC)/weapon.cpp  -o $@ $(INCLUDE)
    
$(BLD)/main.exe: $(BLD)/main.o $(BLD)/unit.o $(BLD)/weapon.o
	$(CXX) $** -o $@ $(INCLUDE)
