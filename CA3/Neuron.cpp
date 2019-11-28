#include "Neuron.h"

using namespace std;

Neuron :: Neuron (vector <double> w , double b)
{
  bias = b;
  for(int i=0;i<w.size();i++)
  {
    weight.push_back(w[i]);
  }
}
