#include <iostream>
#include <vector>
#include <pthread.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>

#define input_size 25
#define layer_size 10

using namespace std;

class Neuron{
public:
  double bias;
  vector <double> weight;
  Neuron(vector <double> w , double b);
};
Neuron :: Neuron (vector <double> w , double b)
{
  bias = b;
  for(int i=0;i<w.size();i++)
  {
    weight.push_back(w[i]);
  }
}
void initialize_neurons(vector <Neuron>* hid , vector <Neuron>* out)
{
	double hid_bias[layer_size] = {-64.662792988884348,55.166473202056522,-164.65949834408667,15.211160979463422,12.24130356452798,17.021888846682156,-27.158230855968867,4.6294220239768293,2.599812799324801,-140.22968989032836};
	double hid_weights[30] = {0.47033493164029083, 1.4098429917984185, -0.45868250608443023,
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
  ifstream myfile("InputFile.txt");
  if(!myfile)
  {
    cout<<"File Doesn't Exist"<<endl;
    return 0;
  }
  vector <double> xyz(3);
  vector <Neuron> hidden_neuron;
  vector <Neuron> output_neuron;
  initialize_neurons(&hidden_neuron , &output_neuron);
  vector <double> final_res;
  double tmp =0;
  clk = clock();
  while(!myfile.eof())
  {
  	vector <double> hidden_to_output_result;	
  	myfile>>xyz[0];
  	myfile>>xyz[1];
  	myfile>>xyz[2];
  	for(int i=0;i<hidden_neuron.size();i++)
  	{
  		for(int j=0;j<3;j++)
  		{
  			tmp += xyz[j] * hidden_neuron[i].weight[j];
  		}
  		tmp += hidden_neuron[i].bias;
  		hidden_to_output_result.push_back(tmp);
  		tmp = 0;
  	}
  	for(int k=0; k < hidden_to_output_result.size();k++)
  	{
  		tmp += tanh(hidden_to_output_result[k]) * output_neuron[0].weight[k];
  	}
  	tmp += output_neuron[0].bias;
  	final_res.push_back(tmp);
  	tmp = 0;
  }
  for(int g=0;g<final_res.size();g++)
  	cout<<final_res[g]<<endl;
  clk2 = clock();
  cout<<"Time: "<<((float)(clk2 - clk)/CLOCKS_PER_SEC)<<endl;
}