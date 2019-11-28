#include "readfile.h"
using namespace std;

vector <vector<double> > input_arr;
vector <double> var_arr;
vector <vector <double> > tmp(3);
int count=0;

void* read_data_from_file(void* threadarg)
{
  //cout << "OPENING" << endl;
  int count = 0;
  ifstream myfile("InputFile.txt");
  if(!myfile)
  {
    cout<<"File Doesn't Exist"<<endl;
    return 0;
  }
  while(!myfile.eof())
  {
    // myfile>>((struct input_data_struct *) threadarg)->x;
    // myfile>>((struct input_data_struct *) threadarg)->y;
    // myfile>>((struct input_data_struct *) threadarg)->z;
    myfile>>((struct input_data_struct *) threadarg)->sm[0];//x
    myfile>>((struct input_data_struct *) threadarg)->sm[1];//y
    myfile>>((struct input_data_struct *) threadarg)->sm[2];//z
    //tmp[0] = ((struct input_data_struct *) threadarg)->sm[0];
    vector <double> tmp = {((struct input_data_struct *) threadarg)->sm[0],((struct input_data_struct *) threadarg)->sm[1],((struct input_data_struct *) threadarg)->sm[2]};
    // ((struct input_data_struct *) threadarg)->input_array[count][0] = ((struct input_data_struct *) threadarg)->sm[0];
    // ((struct input_data_struct *) threadarg)->input_array[count][1] = ((struct input_data_struct *) threadarg)->sm[1];
    // ((struct input_data_struct *) threadarg)->input_array[count][2] = ((struct input_data_struct *) threadarg)->sm[2];
    //cout<<((struct input_data_struct *) threadarg)->input_array[1][0];
    //count +=1;
    //b ++;
    input_arr.push_back(tmp);
    count = count+1;
    //cout<<"count="<<count<<endl;
    //cout<<"count = "<<count++<<endl;
    //printf("SIZE= %d\n",((struct input_data_struct *) threadarg)->input_size);
    //input_size--;
    //usleep(500);

    for(int i=0;i<((struct input_data_struct *) threadarg)->ws_number;i++)
    {
      sem_post(((struct input_data_struct *) threadarg)->hiddenwrite);
    }
    for(int i=0;i<((struct input_data_struct *) threadarg)->ws_number;i++)
    {
      sem_wait(((struct input_data_struct *) threadarg)->hiddenread);
    }
    //count = 0;



  }
  //cout << "CLOSING" << endl;

  //flgg = true;
  myfile.close();
  *(((struct input_data_struct *) threadarg)->finish_flag) = true;
   for(int i=0;i<((struct input_data_struct *) threadarg)->ws_number;i++)
    {
      sem_post(((struct input_data_struct *) threadarg)->hiddenwrite);
    }
  return 0;
}

void* calculate_hidden_ouputs(void* targ)
{
  
  while(true){

    sem_wait(((struct hidden_data_struct *) targ)->hiddenwrite);
    if ( *(((struct hidden_data_struct *) targ)->finish_flag) == true){
     // cout<<"gone!"<<endl;
      sem_post(((struct hidden_data_struct *) targ)->outputwrite);
      break;
    }
    double hidout=0;
   // cout<<"************************************Hidden_Thread["<<(((struct hidden_data_struct *) targ)->neuron_num)<<"]"<<endl;
    //printf("***HIDDEN_THREAD: %d\n", (((struct hidden_data_struct *) targ)->neuron_num));
    // if((((struct hidden_data_struct *) targ)->neuron_num) == 0)
    // {
    // (((struct hidden_data_struct *) targ)->n)->weight[0] = 0.47033493164029083;
    // (((struct hidden_data_struct *) targ)->n)->weight[1] = 1.4098429917984185;
    // (((struct hidden_data_struct *) targ)->n)->weight[2] = -0.45868250608443023;
    // //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
    // }
    for(int i=0;i<3;i++)
    {
      hidout += (((struct hidden_data_struct *) targ)->n)->weight[i] * (((struct hidden_data_struct *) targ)->xyz)[i];
      //cout<<"weight["<<i<<"]"<<(((struct hidden_data_struct *) targ)->n)->weight[i]<<endl;
      //cout<<"xyz["<<i<<"]"<<(((struct hidden_data_struct *) targ)->xyz)[i]<<endl;
      //cout<<
    }
    //cout<<"bias="<<(((struct hidden_data_struct *) targ)->n).bias<<endl;
    hidout += (((struct hidden_data_struct *) targ)->n)->bias;
    (*(((struct hidden_data_struct *) targ)->o)) = tanh(hidout);
    //usleep(500);
    //cout<<"LAST OUT="<<hidout<<"....TANH="<<(*(((struct hidden_data_struct *) targ)->o))<<endl;
    sem_post(((struct hidden_data_struct *) targ)->outputwrite);
    sem_wait(((struct hidden_data_struct *) targ)->outputread);
    sem_post(((struct hidden_data_struct *) targ)->hiddenread);
    //cout << "ckecking" << endl;
    
  }

}
void* calculate_output(void* thread)
{
  while(true)
  {
    double lastout = 0;
    //cout<<"We are in Last"<<endl;
    for(int j=0;j<((struct output_data_struct *) thread)->amount;j++)
    {
      //cout<<"*";
      sem_wait(((struct output_data_struct *) thread)->outputwrite);
    }
    //cout<<endl;
    if ( *(((struct output_data_struct *) thread)->finish_flag) == true){
      //cout<<"OUtput gone!"<<endl;
      return 0;
    }
    for(int i=0;i<((struct output_data_struct *) thread)->amount;i++)
    {
      lastout += (((struct output_data_struct *) thread)->n)->weight[i] * (((struct output_data_struct *) thread)->o)[i];
      //cout<<"OUTPUT_Weight= "<<(((struct output_data_struct *) thread)->n).weight[i]<<endl;
      //printf("output_weight=%lf\n" , (((struct output_data_struct *) thread)->n)->weight[i]);
      //cout<<"OUTPUT_TANH="<<(((struct output_data_struct *) thread)->o)[i]<<endl;
      //printf("output_tanh=%lf\n",(((struct output_data_struct *) thread)->o)[i]);

    }
    lastout += (((struct output_data_struct *) thread)->n)->bias;
    var_arr.push_back(lastout); 
    sem_post(((struct output_data_struct *) thread)->variancewrite);

    //usleep(500);
    //cout<<"OUTPUT BIas="<<(((struct output_data_struct *) thread)->n).bias<<endl;
    cout<<lastout<<endl;
    for(int j=0;j<((struct output_data_struct *) thread)->amount;j++)
    {
      sem_post(((struct output_data_struct *) thread)->outputread);
    }
    
  }
}

void* calculate_variance(void* ta)
{
  //-in2 + sqrt( abs( in2^2- 4*in1*in3))/ (2*in1+ sin( in1*pi))
  //
  vector <double> accurate_res;
  double tmp_res=0;
  long int part =0;
  for(int i=0;i<((struct variance_data_struct *) ta)->inputsize;i++)
    sem_wait(((struct variance_data_struct *) ta)->variancewrite);
  //cout<<"##############"<<endl;
  for(int i=0;i<var_arr.size();i++)
  {
    part = input_arr[i][1]*input_arr[i][1] - 4*input_arr[i][0]*input_arr[i][2];
    tmp_res = -1*input_arr[i][1] + sqrt(abs(part)) / (2*input_arr[i][0] + sin(3.14159265*input_arr[i][0]));
    accurate_res.push_back(tmp_res);
    //cout<<tmp_res<<endl;
  }
  double var_out = 0;
  for(int i=0;i<var_arr.size();i++)
  {
    var_out+= (accurate_res[i]-var_arr[i])*(accurate_res[i]-var_arr[i]); 
  }
  cout<<"Variance:"<<sqrt(var_out/var_arr.size())<<endl;
  //cout<<"We are in Var Thread"<<var_arr.size()<<endl;
}
