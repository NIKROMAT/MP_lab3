TARGET = lab3
CXX = g++
PY = python3
OBJ = main.o lab3.o




.PHONY : all

all : ${TARGET} 


${TARGET} : ${OBJ}
	${CXX} $^ -o $@

main.o : main.cpp
	${CXX} -c $^ -o $@

lab3.o : lab3.cpp
	${CXX} -c $^ -o $@
