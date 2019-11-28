#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <cstring>

#include "Neuron.h"
#include "Layer.h"
#include "readfile.h"

using namespace std;
#define input_size 25
#define layer_size 10


void test(vector<vector<double> >* t)
{
  vector <double> v1 = {1 , 2, 3};
  vector <double> v2 = {4 , 5, 6};
  t->push_back(v1);
  t->push_back(v2);
  t->push_back(v2);

}
void initialize_neurons(vector <Neuron>* hid , vector <Neuron>* out)
{
  
	double hid_bias[layer_size] = {-64.662792988884348,55.166473202056522,-164.65949834408667,15.211160979463422,12.24130356452798,17.021888846682156,-27.158230855968867,4.6294220239768293,2.599812799324801,-140.22968989032836};
	double hid_weights[3*layer_size] = {0.47033493164029083, 1.4098429917984185, -0.45868250608443023,
	9.6050793142547, -4.8106381377184553, -6.2203249699239107,
	31.770859194938183, -5.7448922199483343, -16.587627795598152,
    -0.84940531846430345, 0.992233239423821, -1.0016216732907082,
    -31.627591279841969, 6.372163860709442, 16.370799951110723,
    -54.424139618749813, 18.259089064353208, 28.641186983131369,
    -0.79810901615687035, 1.4504845684255849, -2.1005592298494506,
    -0.4491767869239795, 0.029756397265863002, -0.13040976475951177,
    -0.0095618922693373482, -0.033137398429464504, 0.00097940644650935786,
    23.972375584069319, 1.39315413638509, -0.74824486890357866};
    double out_bias = -52.815634642162856;
  vector <double> out_weights={-2.0210328911392796, 1.4925632391411969, 0.44033520941192872, -1.1474388699560774, -0.84731807564734474, -0.44500692998149916, -0.54580535770972349, 1.2629238917177057, 29.750097611943637, -11.125476345495747};
    for(int i=0;i<layer_size;i++)
    {
    	vector <double> vec_tmp ={hid_weights[3*i],hid_weights[3*i+1],hid_weights[3*i+2]};
    	Neuron tmp(vec_tmp, hid_bias[i]);
    	hid->push_back(tmp);
    }
    Neuron tem(out_weights , out_bias);
    out->push_back(tem);
    

}


int main()
{
  clock_t clk , clk2;
  
  bool ef = false;
  Layer hidden (layer_size);
  Layer output (1);
  Layer variance(1);
  input_data_struct in_data;
  vector <hidden_data_struct> hid_data(layer_size);
  output_data_struct out_data;
  variance_data_struct var_data;
  sem_init(&hidden.write , 0 , 0);
  sem_init(&hidden.read , 0 , 0);
  sem_init(&output.write,0,0);
  sem_init(&output.read,0,0);
  sem_init(&variance.write,0,0);
  //int b=100;

  hidden.shared_memory = new double (3);
  output.shared_memory = new double (layer_size);
  

  initialize_neurons(&hidden.layer_neuron , &output.layer_neuron);

  in_data.hiddenwrite = &hidden.write;
  in_data.hiddenread = &hidden.read;
  in_data.ws_number = layer_size;
  in_data.sm = hidden.shared_memory;
  in_data.finish_flag = &ef;
  

  for(int j=0;j<layer_size;j++)
  {

  	hid_data[j].hiddenwrite = &hidden.write;
  	hid_data[j].hiddenread = &hidden.read;
  	hid_data[j].outputwrite = &output.write;
  	hid_data[j].outputread = &output.read;
  	hid_data[j].neuron_num = j;
  	hid_data[j].o = &output.shared_memory[j];
  	hid_data[j].xyz = hidden.shared_memory;
  	hid_data[j].n = &hidden.layer_neuron[j];
    hid_data[j].finish_flag = &ef;
  }

  clk = clock();
  pthread_t input_thread;
  pthread_create(&input_thread,NULL,read_data_from_file,&in_data);
  pthread_t hidden_thread [layer_size];
  for(int j=0;j<layer_size;j++)
  {
  	pthread_create(&hidden_thread[j],NULL,calculate_hidden_ouputs,&hid_data[j]);	
  }

  out_data.amount = layer_size;
  out_data.n = &output.layer_neuron[0];
  out_data.outputwrite = &output.write;
  out_data.outputread = &output.read;
  out_data.variancewrite = &variance.write;
  out_data.o = output.shared_memory;
  out_data.finish_flag = &ef;

  pthread_t output_thread;
  pthread_create(&output_thread,NULL,calculate_output,&out_data);

  var_data.variancewrite = &variance.write;
  var_data.inputsize = input_size;

  pthread_t variance_thread;
  pthread_create(&variance_thread,NULL,calculate_variance,&var_data);

  pthread_join(input_thread,NULL);
  for(int j=0;j<layer_size;j++)
  {
    pthread_join(hidden_thread[j],NULL);  
  }
  pthread_join(output_thread, NULL);
  pthread_join(variance_thread,NULL);
  clk2 = clock();
  cout<<"Time: "<<((float)(clk2 - clk)/CLOCKS_PER_SEC)<<endl;

}
