#include <iostream>
#include <signal.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "processor.h"
#include <sys/time.h>

int main(int argc, char* argv[])
{
  processor *p1;

  if (argc != 2 && argc != 3) {
    cout<<"Computer Architecture project 1 - ECE PSU 14"<<endl;
    cout<<"Simple MIPS simulator\n";
    cout<<"Usage: simulator [instruction].mem [data.mem]\n";

    cout<<endl<<"Help:"<<endl;
    cout<<"[instruction.mem] -> file with initial content of the instruction memory."<<endl;
    cout<<"[data.mem] -> optional file with initial content of the data memory."<<endl<<endl;
    return 1;
  }

  string fileinstruction(argv[1]);
  string filedata;
  if (argc == 3) filedata = argv[2];

  struct timeval tv1, tv2;
  gettimeofday(&tv1, NULL);

  p1 = new processor(1000,1500,32);
  if (argc == 3) p1->LoadDataMemory(filedata);
  p1->LoadInstructionMemory(fileinstruction);
  p1->ShowInstructionMemory();

  p1->Simulator();

  gettimeofday(&tv2, NULL);
  double total = ((1000000*tv2.tv_sec + tv2.tv_usec) - (tv1.tv_sec*1000000 + tv1.tv_usec))/1.0;
  cout <<endl<< "RESULTS AFTER SIMULATION FOR:"<<fileinstruction;
  p1->ShowDataMemory();
  p1->ShowRegisterFile();
  p1->StoreDataMemory("Result_mem.txt");
  //p1->ShowPerformance();

  cout<<endl;
  cout<<"Simulation took " << total << " microseconds."<<endl<<endl;
  delete p1;
  return(0);
}
