CXX = g++
PY 	= python3

SAMPLES_OBJ = create_samples.o prng.o
RESULTS_OBJ = get_results.o tests.o nist_tests.o nist_funcs.o cephes.o
TIME_OBJ 		= time_check.o prng.o

EXEC_FILES  = create_samples get_results time_check



.PHONY : all

all : samples results time graph
# all : ${EXEC_FILES} 
# 	./create_samples
# 	./get_results
# 	./time_check



.PHONY : samples

samples : create_samples
	./$^

create_samples : ${SAMPLES_OBJ}
	${CXX} $^ -o $@

create_samples.o : create_samples.cpp
	${CXX} -c $^ -o $@

prng.o : src/prng.cpp
	${CXX} -c $^ -o $@



.PHONY : results

results : get_results
	./$^

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



.PHONY : time

time : time_check
	./$^

time_check : ${TIME_OBJ}
	${CXX} $^ -o $@

time_check.o : time_check.cpp
	${CXX} -c $^ -o $@



.PHONY : graph

graph : generation_time.csv time_graph.py
	${PY} time_graph.py

generation_time.csv : time


.PHONY : doc pdf

doc : Doxyfile
	doxygen Doxyfile

Doxyfile : 
	doxygen -g

pdf : doc
	cd latex/ && make


.PHONY : clean cleanall

clean : 
	rm -f ${SAMPLES_OBJ} ${RESULTS_OBJ} ${TIME_OBJ} ${EXEC_FILES}

cleanall : clean
	rm -rI samples generation_time.csv generation_time.png html latex


