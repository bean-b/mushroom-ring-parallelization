CC = gcc
CXX=g++
OMP=-fopenmp
INCLUDE=/usr/local/include/trng
LIB=trng4
OPTFLAGS = -O3
PROGS = mushroom_seq mushroom_omp     

OBJS= getCommandLine.o display.o gnuplot_i.o

all: $(PROGS)

%.o: %.cpp %.hpp 
	$(CXX) ${OMP} -c -o $@ $< -I$(INCLUDE) -l$(LIB)

## sequential
mushroom_seq: $(OBJS) mushroom_seq.cpp
	$(CXX) ${OMP} ${OPTFLAGS} -o mushroom_seq $(OBJS) mushroom_seq.cpp -I$(INCLUDE) -l$(LIB)

######### OpenMP
mushroom_omp:mushroom_omp.cpp
	$(CXX) ${OMP} ${OPTFLAGS} -o mushroom_omp $(OBJS) mushroom_omp.cpp -I$(INCLUDE) -l$(LIB)

# getCommandLine.cpp : getCommandLine.hpp

display.o: display.c 
	$(CC) $(OMP) -c -o $@ $<


gnuplot.o: gnuplot.c gnuplot.h
	$(CC) $(OMP) -c -o $@ $<


######### clean
clean:
		rm -f $(OBJS) $(PROGS) *.o
