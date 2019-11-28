#ifndef __LAYER_H__
#define __LAYER_H__


#include "Neuron.h"
#include <unistd.h>

class Layer{
public:
  int sze;
  std::vector <Neuron> layer_neuron;
  double* shared_memory;
  sem_t write;
  sem_t read;
  Layer(int);
};


#endif
