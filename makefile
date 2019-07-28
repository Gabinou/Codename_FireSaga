

CXX	= c++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 

all: $(BLD)/main.exe

$(BLD)/main.o: $(SRC)/main.cpp
	$(CXX) -c $** -o $@ $(INCLUDE)

$(BLD)/unit.o: $(SRC)/unit.cpp
	$(CXX) -c $**  -o $@ $(INCLUDE)

$(BLD)/weapon.o: $(SRC)/weapon.cpp
	$(CXX) -c $**  -o $@ $(INCLUDE)
    
$(BLD)/main.exe: $(BLD)/main.o $(BLD)/unit.o $(BLD)/weapon.o
	$(CXX) $** -o $@ $(INCLUDE)
