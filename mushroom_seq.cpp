#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include "getCommandLine.hpp"

// Original Attribution of Conway's Game of Life from HW3
// Edited by Bibiane M. and Libby Shoop for Mushroom Fairy Rings Simulation

// TRNG library for c++
#include <trng/mt19937_64.hpp>
#include <trng/mt19937.hpp>
#include <trng/lcg64_shift.hpp>
#include <trng/normal_dist.hpp>
#include <trng/uniform_dist.hpp>
#include <trng/uniform01_dist.hpp>
#include <trng/exponential_dist.hpp>
#include <unistd.h>

#define SEED 1995       // for debugging using the same grid
#define probSpore 0.0005 // Initial spore density is 5 %
#define probSporeToHyphae 0.5 // Probability that SPORE -> YOUNG
#define probMushroom 0.8 // Probability that MATURING -> MUSHROOM (else OLDER)
#define probSpread 0.25 // Probability that EMPTY -> YOUNG (from neighboring YOUNG)
 
// functions found in this file below main()
void initGrid(int *grid, int dim, int seed);
void initCenterGrid(int *grid, int dim);
void printGrid(int *grid, int dim);
void init_ghosts(int *grid, int *newGrid, int dim);
void apply_rules(int *grid, int *newGrid, trng::lcg64_shift &RNengine1, int dim);
void update_grid(int *grid, int *newGrid, int dim);
void sim(int *grid, int *newGrid, trng::lcg64_shift &RNengine1, int dim);
void prtdat(int dim, int *grid, const char *fnam);

extern "C" void drawFirstAndLast();
 

int main(int argc, char* argv[])
{

    
    

    int i, j;  // loop variables
    
    /////////////////// default values  ///////////////////////////////////
    // number of game steps
    int itEnd = 1 << 11;
    // dimension of square grid
    int dim = 2048;      // Grid dimension (excluding ghost cells)
    // if in debug mode, we will use a fixed seed to initialize the grid
    int debug = 0;
    // whether to print the grid for debugging
    int verbose = 0;

    int numThreads = 1;  // not used in sequential version, but ready for openMP

    // grab the command line arguments
    getArguments(argc, argv, &dim, &itEnd, &debug, &verbose, &numThreads);

    // !#!#!#!#!#!#!#!#
    // for sequential version, keep numThreads at 1. REMOVE FOR OpenMP
    if (numThreads != 1) numThreads = 1;
    // !#!#!#!#!#!#!#!# FIX ABOVE LINE FOR openMP version by removing
    
    // grid array with dimension dim + ghost columns and rows
    int    arraySize = (dim+2) * (dim+2);
    size_t bytes     = arraySize * sizeof(int);
    int    *grid     = (int*)malloc(bytes);
 
    // allocate result grid
    int */*restrict*/newGrid = (int*) malloc(bytes);



    printf("debug %d   verbose %d    dim %d     iter %d   \n", debug, verbose, dim, itEnd);
    int seed;
    if(debug){
        seed=1995;
    }else{
        seed=time(NULL);
    }

    initGrid(grid, dim, seed);
  
    int total = 0; // total number of cells  
    int it;

    double st = omp_get_wtime(); // start timing

    // declare and seed the simulation's random number generator
    trng::lcg64_shift RNengine1;
    //seeding 
    RNengine1.seed((long unsigned int) seed);
    
    if(verbose){
        prtdat(dim,  grid, "initial.dat");
    }

    //////////////   run the simulation 
    for(it = 0; it < itEnd; it++){
        sim( grid, newGrid, RNengine1, dim);
    }

    // sum up cells
    for (i = 1; i <= dim; i++) {
        for (j = 1; j <= dim; j++) {
            total += grid[i*(dim+2) + j];
        }
    }
    printf("Total: %d\n", total);

    if (verbose) {
    //   printf("Final grid:\n\n");
    //   printGrid(grid, dim); // print final grid
      prtdat(dim,  grid, "final.dat");
    }

    double runtime = omp_get_wtime() - st;
    printf("Total time: %f s\n", runtime);


    if(verbose){
        drawFirstAndLast();
    }
    

    free(grid);
    free(newGrid);
 
    return 0;
}

// Simulate the fairy mushroom rings by initializing ghost rows, 
// applying the rules, and updating the grid.
//
// Note about C++ : this & syntax below is how we pass an object, in this case
// RNemgine1, by reference to this function.
//
void sim(int *grid, int *newGrid, trng::lcg64_shift &RNengine1, int dim) {
    // printGrid(grid);  
    // printf("---------------------------------------\n");
    init_ghosts(grid, newGrid, dim);
    apply_rules(grid, newGrid, RNengine1, dim);
    update_grid(grid, newGrid, dim);  
}
 
// Applies the rules of the mushroom simulation
void apply_rules(int *grid, int *newGrid, trng::lcg64_shift &RNengine1, int dim) {
    int i, j, k;
    trng::uniform01_dist<> uni; 

    // iterate over the grid
    for (i = 1; i <= dim; i++) {
        for (j = 1; j <= dim; j++) {
            
            double randN = uni(RNengine1); //inside loop

            int id = i*(dim+2) + j;
            
            // Implementing the Mushroom Simulation Rules
            if (grid[id] == 0) { // EMPTY, probSpread to YOUNG
                int neighbors[8] = { 
                    grid[id+(dim+2)], grid[id-(dim+2)], 
                    grid[id+1], grid[id-1], grid[id+(dim+3)], 
                    grid[id-(dim+3)], grid[id-(dim+1)], grid[id+(dim+1)] };
                for(k = 0; k < 8; k++) { //this should be okay sequential in the parallel version since it is only 8 elements

                    if(neighbors[k] == 1 && randN < probSpread) {
                        newGrid[id] = 1;
                        break; // break out of the inner k loop
                    } else{
                        newGrid[id] = 0; 
                    }
                } // (ELSE EMPTY, no change)
            } else if (grid[id] == 1) { // SPORE, probSporeToHyphae to YOUNG

                if(randN < probSporeToHyphae) {
                    newGrid[id] = 2;
                } else {
                    newGrid[id] = 1;
                }
            } else if (grid[id] == 2) { // YOUNG -> MATURING
                newGrid[id] = 3;
            } else if (grid[id] == 3) { // MATURING, probMushroom to MUSHROOM else OLDER
                if(randN < probMushroom) {
                    newGrid[id] = 4; // MUSHROOM
                } else {
                    newGrid[id] = 5; // OLDER
                }
            } else if (grid[id] == 4 || grid[id] == 5) { // MUSHROOM or OLDER -> DECAYING
                newGrid[id] = 6;
            } else if (grid[id] == 6) { // DECAYING -> DEAD1
                newGrid[id] = 7;
            } else if (grid[id] == 7) { // DECAYING -> DEAD2
                newGrid[id] = 8;
            } else if (grid[id] == 8) { // DEAD2 -> EMPTY
                newGrid[id] = 9;
            }
        }
    }
}

// Transfers newGrid data to grid to prepare for next time step
void update_grid(int *grid, int *newGrid, int dim) {
    int i, j;
    // copy new grid over, as pointers cannot be switched on the device
    for(i = 1; i <= dim; i++) {
        for(j = 1; j <= dim; j++) {
            int id = i*(dim+2) + j;
            grid[id] = newGrid[id];
        }
    }
}

// Initializes ghost rows and columns
void init_ghosts(int *grid, int *newGrid, int dim) {
    int i;

    // ghost rows
    for (i = 1; i <= dim; i++) { 
      grid[(dim+2)*(dim+1)+i] = grid[(dim+2)+i]; // copy first row to bottom ghost row
      grid[i] = grid[(dim+2)*dim + i]; // copy last row to top ghost row
    }

    // ghost columns
    for (i = 0; i <= dim+1; i++) {
      grid[i*(dim+2)+dim+1] = grid[i*(dim+2)+1];  // copy first column to right most ghost column
      grid[i*(dim+2)] = grid[i*(dim+2) + dim]; // copy last column to left most ghost column
    }
}

// assign initial population randomly, based on probSpore 
void initGrid(int *grid, int dim, int debug) {

    trng::lcg64_shift RNengine1;   // rand number generator
    trng::uniform01_dist<> uni;    // uniform distribution between 0,1

    //seeding 
    if (debug) {
      RNengine1.seed((long unsigned int) SEED);
    } else {
      RNengine1.seed((long unsigned int)time(NULL)); 
    }
    
    int i, j;

    for(i = 1; i <= dim; i++) {
        for(j = 1; j <= dim; j++) {
            double randN = uni(RNengine1);  // next random num
            if(randN < probSpore) {
                grid[i*(dim+2)+j] = 1; // SPORE
            } else{
                grid[i*(dim+2)+j] = 0; // EMPTY
            }
        }
    }
}

void initCenterGrid(int *grid, int dim) {
    int i, j;
    for(i = 1; i <= dim; i++) {
        for(j = 1; j <= dim; j++) {
          if ((i == dim/2) && (j == dim/2)) {
            grid[i*(dim+2)+j] = 1; // SPORE
          } else {
            grid[i*(dim+2)+j] = 0; // EMPTY
          }
        }
    }
}


// printGrid for debugging
void printGrid(int *grid, int dim) {
    int i, j;
    for(i = 1; i <= dim; i++) {
        for(j = 1; j <= dim; j++) {
            int id = i*(dim+2) + j;
            printf("%d", grid[id]);
        }
        printf("\n");
    }
}


void prtdat(int dim, int *grid, const char *fnam)
{
  int ix, iy;
  FILE *fp;

  fp = fopen(fnam, "w");

    for(ix = 1; ix <= dim; ix++) {
        for(iy = 1; iy <= dim; iy++) {
            int id = ix*(dim+2) + iy;
            fprintf(fp, "%8d", grid[id]);
        }
        fprintf(fp, "\n");
    }

  fclose(fp);
}
