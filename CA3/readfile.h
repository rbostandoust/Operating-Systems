#ifndef __READFILE_H__
#define __READFILE_H__

#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fstream>
#include <string>
#include <math.h>

#include "Neuron.h"
#include "Layer.h"

//bool flgg = false;

//std::vector <double> z = {0,0,0};


struct input_data_struct{
  //std::vector <std::vetcor <double> > input;
  int ws_number;
  bool* finish_flag;
  double* sm;
  sem_t* hiddenwrite;
  sem_t* hiddenread; 
};

struct hidden_data_struct{
	int neuron_num;
  bool* finish_flag;
	double* xyz;
	double* o;
	Neuron* n;
	sem_t* hiddenwrite;
  sem_t* hiddenread;
  sem_t* outputwrite;
  sem_t* outputread;    
};

struct output_data_struct{
	int amount;
  bool* finish_flag;
	double* o;
  double* v;
	sem_t* outputwrite;
	sem_t* outputread;
  sem_t* variancewrite;
	Neuron* n; 
};

struct variance_data_struct{
  int inputsize;
  sem_t* variancewrite;

};


void* read_data_from_file(void* threadarg);
void* calculate_hidden_ouputs(void* targ);
void* calculate_output(void* thread);
void* calculate_variance(void* ta);

#endif
