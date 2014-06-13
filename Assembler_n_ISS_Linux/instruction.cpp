#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <string>
#include <string.h>
#include <vector>
#include <list>
#include <cmath>

#include "instruction.h"
using std::ifstream;
using namespace std;

string instruction::getinstructionb() {
  return binaryinstruction;
};

string instruction::getinstructionl() {
  return literalinstruction;
};

int instruction::getopcode(){
  return bin2dec(binaryinstruction.substr(0,6).c_str());
};

int instruction::getrs(){
  return bin2dec(binaryinstruction.substr(6,5).c_str());
};

int instruction::getrt(){
  return bin2dec(binaryinstruction.substr(11,5).c_str());
};

int instruction::getrd(){
  return bin2dec(binaryinstruction.substr(16,5).c_str());
};

int instruction::getshamt(){
  return bin2dec(binaryinstruction.substr(21,5).c_str());
};

int instruction::getfunct(){
  return bin2dec(binaryinstruction.substr(26,6).c_str());
};

int instruction::getimm_or_add(){
  return bin2dec(binaryinstruction.substr(16,16).c_str());
};

bool instruction::setimm_or_add(int val){
  string result;
  result = string(dec2bin(val,16));
  binaryinstruction.replace(16,result.length(),result);
  return true;
};

int instruction::gettarget() {
  return bin2dec(binaryinstruction.substr(6,26).c_str());
};

bool instruction::settarget(int val) {
  string result;
  result = string(dec2bin(val,26));
  binaryinstruction.replace(6,result.length(),result);
  return true;
};

bool instruction::setlabelstring(string t){
  label = t;
  return true;
};

string instruction::getlabelstring(){
  return label;
};

// convert a binary string to a decimal number, returns decimal value
int instruction::bin2dec(const char *bin)
{
  int  b, k, m, n;
  int  len, sum = 0;

  len = strlen(bin) - 1;
  for(k = 0; k <= len; k++)
    {
      n = (bin[k] - '0'); // char to numeric value
      if ((n > 1) || (n < 0))
        {
          puts("\n\n ERROR! BINARY has only 1 and 0!\n");
          return (0);
        }
      for(b = 1, m = len; m > k; m--)
        {
          // 1 2 4 8 16 32 64 ... place-values, reversed here
          b *= 2;
        }
      // sum it up
      sum = sum + n * b;
      //printf("%d*%d + ",n,b);  // uncomment to show the way this works
    }
  return(sum);
}

int instruction::power ( int base, int exponent ) {
  long p=1;
  int i;

  for ( i=1 ; i<=exponent ; i++) {
    p*=base;
  }

  return(p);
}

string instruction::dec2bin(int input, int m)
{
  string result;
  static int i = 0;
  //char array[m+1];
  char * array;
  array=(char*)malloc((m+1)*sizeof(char));

  int limit = 0;
  limit = power(2, m);
  const int MASK = limit/2; // the binary equivalent is: 1 + m 0 bits

  array[m] = '\0';
  for (i= 0; i < m ;i++)
    {
      if (input & MASK){
        array[i]='1';}
      else{
        array[i]='0';}
      input = input << 1;
    }

  result = string(array);
  return (result);
}

bool instruction::setinstructionb(string inb) {
  binaryinstruction = inb;
  return true;
};

bool instruction::setinstructionl(string ins) {
  literalinstruction = ins;
  return true;
};
