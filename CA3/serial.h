#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <iostream>
#include <vector>
#include <pthread.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>


//int input_size = 10;

#include "Neuron.h"
#include "Layer.h"
#include "readfile.h"

int serial(std::ifstream& fin);



#endif