#include <experimental/random>

#ifndef ARRAYFUNCTIONS_H
#define ARRAYFUNCTIONS_H

int * generateRandomArray(int sz, int max_value=99999);
void sortArray(int * array, int size);
int * gatherSortArrays(int ** arrays, int nbArrays, int size,
                       int max_value=99999);

#endif
