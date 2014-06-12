#ifndef _processorclass_ 
#define _processorclass_
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include "instruction.h"

using namespace std;

class processor 
{
private:
unsigned int ImemSize;
unsigned int DmemSize;
unsigned int RegSize;

//Data structures for the instruction and data memory and register file
vector<string> Inst_Mem;
vector<int> Data_Mem;
vector<int> RegFile;

private:
unsigned int InstructionCount;
unsigned int DataMemoryRange;
//These are the registers of the microprocessor
//IF
int PC;
instruction INSTR;

//IF_to_ID
int IF_to_ID_PC;
instruction IF_to_ID_INSTR;
bool IF_to_ID_Write_Register_File_Flag;

//ID
int opcode;
int reg1;
int reg2;
int reg3;
int shamt;
int funct;
int immediate;
int r1;
int r2;
int target;
bool local_stage_ID_Write_Register_File_Flag;

//ID_to_EXE
int ID_to_EXE_PC;
int ID_to_EXE_opcode;
int ID_to_EXE_reg3;
int ID_to_EXE_shamt;
int ID_to_EXE_funct;
int ID_to_EXE_immediate;
int ID_to_EXE_r1;
int ID_to_EXE_r2;
int ID_to_EXE_target;
bool ID_to_EXE_Write_Register_File_Flag;
int ID_to_EXE_reg1;
int ID_to_EXE_reg2;

//EXE
int temp_local_stage_EXE_ALUResult;
int local_stage_EXE_ALUResult;
bool local_stage_EXE_Zero;
int local_stage_EXE_r2;
int local_stage_EXE_reg3;
bool local_stage_EXE_Mflag;
int local_stage_EXE_opcode;
int local_stage_EXE_immediate;
bool local_stage_EXE_Write_Register_File_Flag;

//EXE_to_MEM
int EXE_to_MEM_ALUResult;
bool EXE_to_MEM_Zero;
int EXE_to_MEM_r2;
int EXE_to_MEM_reg3;
bool EXE_to_MEM_Mflag;
int EXE_to_MEM_opcode;
int EXE_to_MEM_immediate;
bool EXE_to_MEM_Write_Register_File_Flag;

//MEM
int local_stage_WB_opcode;
int local_stage_WB_DataResult;
int local_stage_WB_reg3;
int local_stage_WB_ALUResult;
bool local_stage_WB_Mflag;
bool local_stage_WB_Write_Register_File_Flag;

//MEM_to_WB
int MEM_to_WB_opcode;
int MEM_to_WB_DataResult;
int MEM_to_WB_reg3;
int MEM_to_WB_ALUResult;
bool MEM_to_WB_Mflag;
bool MEM_to_WB_Write_Register_File_Flag;

//WB shadow register
int s_opcode;
int s_DataResult;
int s_ALUResult;
int s_WB_reg3;

public:
//General functions for external interface with the simulator:
bool LoadInstructionMemory(string filenamei);
bool LoadDataMemory(string filenamed);
bool Simulator();
bool ShowRegisterFile();
bool ShowInstructionMemory();
bool ShowDataMemory();
unsigned int getInstructionCount();
bool InitializeSimulation();
//function to store data memory values in memory file
bool StoreDataMemory(string filenamed);
//class constructor
processor(unsigned int imems, unsigned int dmems, unsigned int regs)
: ImemSize(imems), DmemSize(dmems), RegSize(regs) {

Inst_Mem=vector<string>(ImemSize+1);
Data_Mem=vector<int>(DmemSize+1) ;
RegFile=vector<int>(RegSize+1);
InstructionCount = 0;
DataMemoryRange = 0;
//intialize Data Memory 
unsigned int u=0;
while(u < DmemSize) {
Data_Mem[u] = 0;
u++;
}

u=0;
//intialize Instruction Memory
while(u < ImemSize) {
Inst_Mem[u] ="00000000000000000000000000000000";
u++;
}

u=0;
//intialize Instruction Memory
while(u < DmemSize) {
Data_Mem[u] =0;
u++;
}

u=0;
//Initialize Register File
while(u < RegSize) {
RegFile[u] = 1;
RegFile[0] = 0;
u++;
}
InitializeSimulation();
};

~processor() {
Inst_Mem.clear();
Data_Mem.clear();
RegFile.clear();
};

private:
string dec2bin(int input, int m);
int bin2dec(const char *bin);
int power (int base, int exponent);
string trimString(string str);
};
#endif
