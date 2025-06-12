# TARGET = create_samples
CXX = g++
PY 	= python3
# OBJ = create_samples.o prng.o

SAMPLES_OBJ = create_samples.o prng.o
RESULTS_OBJ = get_results.o tests.o nist_tests.o nist_funcs.o cephes.o
TIME_OBJ 		= time_check.o prng.o

EXEC_FILES  = create_samples get_results time_check



.PHONY : all test results samples time clean cleanall

all : ${EXEC_FILES}
	./create_samples
	./get_results
	./time_check

samples : create_samples
	./$^

# create_samples : create_samples.o prng.o
create_samples : ${SAMPLES_OBJ}
	${CXX} $^ -o $@

create_samples.o : create_samples.cpp
	${CXX} -c $^ -o $@

prng.o : src/prng.cpp
	${CXX} -c $^ -o $@



results : get_results
	./get_results

get_results : ${RESULTS_OBJ}
	${CXX} $^ -o get_results

get_results.o : get_results.cpp
	${CXX} -c $^ -o $@

tests.o : src/tests.cpp
	${CXX} -c $^ -o $@

nist_tests.o : src/nist_tests.cpp
	${CXX} -c $^ -o $@

nist_funcs.o : src/nist_funcs.cpp
	${CXX} -c $^ -o $@

cephes.o : src/external/cephes.cpp
	${CXX} -c $^ -o $@


time : time_check
	./time_check

time_check : ${TIME_OBJ}
	${CXX} $^ -o $@

time_check.o : time_check.cpp
	${CXX} -c $^ -o $@


clean : 
	rm -f ${SAMPLES_OBJ} ${RESULTS_OBJ} ${TIME_OBJ}

cleanall : clean
	rm -rI ${EXEC_FILES} samples generation_time.csv



test : t
	./t

t : test.o nist_tests.o
	${CXX} $^ -o t

test.o : test.cpp
	${CXX} -c $^ -o $@