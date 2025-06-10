# TARGET = create_samples
CXX = g++
PY = python3
# OBJ = create_samples.o prng.o




.PHONY : all test results samples

# all : ${TARGET} 

samples : create_samples
	./$^

create_samples : create_samples.o prng.o
	${CXX} $^ -o $@

create_samples.o : create_samples.cpp
	${CXX} -c $^ -o $@

prng.o : src/prng.cpp
	${CXX} -c $^ -o $@



results : get_results
	./get_results

get_results : get_results.o tests.o
	${CXX} $^ -o get_results

get_results.o : get_results.cpp
	${CXX} -c $^ -o $@

tests.o : src/tests.cpp
	${CXX} -c $^ -o $@


test : t
	./t

t : test.cpp
	${CXX} $^ -o t