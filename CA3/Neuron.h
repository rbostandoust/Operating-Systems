#ifndef __NEURON_H__
#define __NEURON_H__

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

//using namespace std;

class Neuron{
public:
  double bias;
  std::vector <double> weight;
  Neuron(std::vector <double> w , double b);
};

#endif
