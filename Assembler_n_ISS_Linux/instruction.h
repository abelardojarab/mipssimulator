//Instruction class
#ifndef  _instructionclass_
#define _instructionclass_
#include <string>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>

using namespace std;

class instruction {
private:
  string binaryinstruction;
  string literalinstruction;
  int bin2dec(const char *bin);
  string dec2bin(int input, int m);
  int power(int base, int exponent);
  string label;

public:
  string getinstructionb();
  bool setinstructionb(string inb);

  string getinstructionl();
  bool setinstructionl(string ins);

  int getopcode();
  int getrs();
  int getrt();
  int getrd();
  int getshamt();
  int getimm_or_add();
  bool setimm_or_add(int val);

  int getfunct();

  int gettarget();
  bool settarget(int val);

  string getlabelstring();
  bool setlabelstring(string t);

  instruction(const string &instb, const string &instl, const string &labs) {
    label=std::string(labs);
    binaryinstruction=std::string(instb);
    literalinstruction=std::string(instl);
  };

  instruction() {
    label="";
    binaryinstruction=dec2bin(0,32);
    literalinstruction="";
  };

  instruction& operator= (const string &b) {
    setinstructionb(b);
    setinstructionl("");
    setlabelstring("");
    return *this;
  }

  instruction& operator=(instruction &rhs) {
    if (this!=&rhs) {
      //we needed first to delete associated memory (pointers), though we do not have dynamically allocated memory
      setinstructionb(rhs.getinstructionb());
      setinstructionl("");
      setlabelstring("");
    }
    return *this;
  }

};

#endif
