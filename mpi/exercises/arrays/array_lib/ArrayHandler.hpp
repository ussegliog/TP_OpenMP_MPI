# include "array_functions.hpp"

#ifndef ARRAYHANDLER_H
#define ARRAYHANDLER_H

class ArrayHandler
{
public:
  // Constructor/Destructor
  ArrayHandler(int number_of_arrays, int size, int max_value=99999);
  ~ArrayHandler();

  // Getters
  int ** getArrays();
  int * getFirstArray();

private:
  int m_number_of_arrays;
  int m_size;
  int m_max_value;
  // Data as private attribute
  int ** m_arrays;
};
#endif
