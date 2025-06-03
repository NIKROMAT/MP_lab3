TARGET = create_samples
CXX = g++
PY = python3
OBJ = create_samples.o prng.o




.PHONY : all

all : ${TARGET} 


${TARGET} : ${OBJ}
	${CXX} $^ -o $@

create_samples.o : create_samples.cpp
	${CXX} -c $^ -o $@

prng.o : prng.cpp
	${CXX} -c $^ -o $@
