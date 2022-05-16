#include "ArrayHandler.hpp"

ArrayHandler::ArrayHandler(int number_of_arrays, int size, int max_value)
{
  // Init attributes
  m_number_of_arrays = number_of_arrays;
  m_size = size;
  m_max_value = max_value;

  // Allocate arrays
  m_arrays = new int*[m_number_of_arrays];

  // Generate random arrays
  for (int id_array = 0; id_array < m_number_of_arrays;
       id_array++)
    {
      // Generate a random array
      m_arrays[id_array] = generateRandomArray(size, max_value);
    }

}

ArrayHandler::~ArrayHandler()
{
  // Free memory
  for (int i = 0; i < m_number_of_arrays; i++)
    {
      delete m_arrays[0];
      m_arrays[0] = 0;
    }
  delete m_arrays;
  m_arrays = 0;
}

// Getters
int ** ArrayHandler::getArrays()
{
  return m_arrays;
}

int * ArrayHandler::getFirstArray()
{
  return m_arrays[0];
}
