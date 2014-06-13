#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include "processor.h"
#include "instruction.h"
#define DEBUG 0

using std::ifstream;
using namespace std;

bool processor::LoadDataMemory(string filenamed)
{

  unsigned int datamaxrange=0;
  unsigned int num=0;
  char buf[256];
  int tempi;

  ifstream ifs1;
  ifs1.open(filenamed.c_str());
  if(!ifs1.is_open()) { // file couldn't be opened
    cerr << "Error: Data Memory file could not be opened." << endl;
    exit(1);
  }

  while( !ifs1.eof() ) {
    ifs1.getline(buf,100);
    tempi = atoi(buf);

    if (num < DmemSize) {
      Data_Mem[num] = tempi;
      if (datamaxrange < num) datamaxrange = num;
      num++;
    } else { break; };
  }
  DataMemoryRange = datamaxrange;
  ifs1.close();
  return true;
}

/**************Store data memory****************************/
bool processor::StoreDataMemory(string filenamed)
{
  int size=(int)Data_Mem.size();
  int lineop=0;
  // cout<<"Data mem size="<<size<<endl;

  char * buff;
  // buff= new char [DataMemoryRange*2];
  buff=(char*)malloc((size*2)*sizeof(char));
  int count=0;

  ofstream ofs1;
  ofs1.open(filenamed.c_str());
  if(!ofs1.is_open()) { // file couldn't be opened
    cerr << "Error: Data Memory file could not be opened." << endl;
    exit(1);
  }
  
  for (int i=0; i<size; i++)
    {
      ofs1<<Data_Mem.at(i);
      ofs1<<'\n';
      buff++;
      count++;
      lineop++;
    }

  //ofs1.write(buffer,count);
  ofs1.close();
  return true;
}

bool processor::LoadInstructionMemory(string filenamei)
{
  unsigned int num=0;
  char buf[256];

  ifstream ifs;
  ifs.open(filenamei.c_str());
  if(!ifs.is_open()) { // file couldn't be opened
    cerr << "Error: Instruction Memory file could not be opened." << endl;
    exit(1);
  }

  while( !ifs.eof() ) {
    ifs.getline(buf,256);
    string temps = buf;
    if (temps.length() == 32) {
      Inst_Mem[num] = temps;
      num++; }
  }
  InstructionCount=(int)num;
  ifs.close();
  return true;
}

bool processor::Simulator()
{
  InitializeSimulation();
  unsigned int cycle = 0;
  int STALL = 0, BRANCH = 0, BRANCH_TKN = 0, INSTR_COUNT=0;
  int stall_count = 0;

  bool exit_flag = false;
  int latency_to_end = 3;

  double CPI;

  cout<<endl<<"******************Simulation running:*********************"<<endl;

  //Simulation of the pipeline starts here:

  while((unsigned int) PC<ImemSize){

    /***********************Here we start the FETCH stage*************************/
    if(STALL > 0){
      //dont fetch anything if we are having a stall, just place NOP operations
      STALL = STALL - 1;
      stall_count= stall_count+1;
#if DEBUG
      cout<<"stall cycle no"<<stall_count<<endl;
#endif
    }
    else{
      if(exit_flag == false)
        {
          INSTR = Inst_Mem[PC];
#if DEBUG
          cout<<endl<<endl<<"PC: "<<PC<<"\tInstruction: "<<INSTR.getinstructionb()<<endl;
#endif
          PC=PC+1;
          INSTR_COUNT=INSTR_COUNT + 1;
        }
      else
        {
          INSTR = dec2bin(7,6) + dec2bin(0,26);
        }

    }

#if DEBUG
    cout<<"Got instruction: "<<INSTR.getopcode()<<"\n";
#endif
    /***********************Here we start the DECODE stage*************************/
    //-----Decoding the instruction

    opcode = IF_to_ID_INSTR.getopcode();
    reg1 = IF_to_ID_INSTR.getrs();
    reg2 = IF_to_ID_INSTR.getrt();

    //check for type of instruction
    // i type: dest r3=r2
    if (opcode== 11 || opcode== 10 || opcode== 5 || opcode== 35 || opcode== 12 || opcode== 13 || opcode== 9 || opcode== 8 || opcode == 23) {
      reg3 = reg2;
    }
    else if (opcode == 43)
      reg3 = 0;
    else
      reg3 = IF_to_ID_INSTR.getrd();

    shamt = IF_to_ID_INSTR.getshamt();
    funct = IF_to_ID_INSTR.getfunct();
    immediate = IF_to_ID_INSTR.getimm_or_add();
    target = IF_to_ID_INSTR.gettarget();

    //-----Access to the register file... mimicing dual port
    if(reg1 == MEM_to_WB_reg3 && reg1 != 0)
      {
        if(MEM_to_WB_opcode != 35)
          r1 = MEM_to_WB_ALUResult;
        else
          r1 = MEM_to_WB_DataResult;
      }
    else
      {
        r1 = RegFile[reg1];
      }
    if(reg2 == MEM_to_WB_reg3 && reg2 != 0)
      {
        if(MEM_to_WB_opcode != 35)
          r2 = MEM_to_WB_ALUResult;
        else
          r2 = MEM_to_WB_DataResult;
      }
    else
      {
        r2 = RegFile[reg2];
      }

#if DEBUG
    cout<<"Fetch:: reg1 is R"<<reg1<<" value is "<<r1<<endl;
    cout<<"Fetch:: reg2 is R"<<reg2<<" value is "<<r2<<endl;
#endif

    //This indicates result should be written in register file
    if (opcode == 6 || opcode == 7 || opcode == 43 || opcode == 4 || opcode == 5 || opcode == 2 || opcode == 3 || (opcode == 0 && funct == 8))
      local_stage_ID_Write_Register_File_Flag = false; // indicates do not write to register file
    else
      local_stage_ID_Write_Register_File_Flag = true;

    /********************Here we start the EXECUTE stage**************************/

    local_stage_EXE_Write_Register_File_Flag = ID_to_EXE_Write_Register_File_Flag;

    switch(ID_to_EXE_opcode) {

      //-----Adding the I-type instructions
    case 8:  //addi
#if DEBUG
      cout<<"\n addition***** "<<ID_to_EXE_r1<<"+"<<ID_to_EXE_immediate<<"\n";
#endif
      if(ID_to_EXE_immediate > 32767)
        local_stage_EXE_ALUResult = ID_to_EXE_r1 - (65536-ID_to_EXE_immediate);
      else
        local_stage_EXE_ALUResult = ID_to_EXE_r1 + ID_to_EXE_immediate;
      break;
    case 23:  //subi
#if DEBUG
      cout<<"\n subtraction**** "<<ID_to_EXE_r1<<"-"<<ID_to_EXE_immediate<<"\n";
#endif
      local_stage_EXE_ALUResult = ID_to_EXE_r1 - ID_to_EXE_immediate;
      break;

    case 9: //addiu
      local_stage_EXE_ALUResult = ID_to_EXE_r1 + (unsigned int)ID_to_EXE_immediate;
      break;

    case 12:  //andi
      local_stage_EXE_ALUResult = ID_to_EXE_r1 & ID_to_EXE_immediate;
      break;

    case 13: //ori
      local_stage_EXE_ALUResult = ID_to_EXE_r1 | ID_to_EXE_immediate;
      break;

    case 35:  //lw
      local_stage_EXE_ALUResult = ID_to_EXE_r1 + ID_to_EXE_immediate;
      break;

    case 43: //sw
      local_stage_EXE_ALUResult = ID_to_EXE_r1 + ID_to_EXE_immediate;
      break;

    case 10:  //slti
      if(ID_to_EXE_r1 < ID_to_EXE_immediate){
        local_stage_EXE_ALUResult = 1;
      } else {
        local_stage_EXE_ALUResult = 0;
      }
      break;

    case 11:  //sltiu
      if((unsigned int) ID_to_EXE_r1 < (unsigned int)ID_to_EXE_immediate){
        local_stage_EXE_ALUResult = 1;
      } else {
        local_stage_EXE_ALUResult = 0;
      }
      break;

      //-----Adding the R-type instructions
    case 0:

      if(ID_to_EXE_funct == 32){ //add
#if DEBUG
        cout<<"\n **addition**"<<ID_to_EXE_r1<<"+"<<ID_to_EXE_r2<<"\n";
#endif
        local_stage_EXE_ALUResult = ID_to_EXE_r1 + ID_to_EXE_r2;
      }
      if(ID_to_EXE_funct == 34){ //sub
        local_stage_EXE_ALUResult = ID_to_EXE_r1 - ID_to_EXE_r2;
      }
      if(ID_to_EXE_funct == 33){ //addu
        temp_local_stage_EXE_ALUResult = ID_to_EXE_r1 + ID_to_EXE_r2;
        local_stage_EXE_ALUResult= (unsigned int)temp_local_stage_EXE_ALUResult;
      }
      if(ID_to_EXE_funct == 35){ //subu
        temp_local_stage_EXE_ALUResult = (unsigned int)ID_to_EXE_r1 - (unsigned int)ID_to_EXE_r2;
        if(temp_local_stage_EXE_ALUResult>=0)// we are not going to use this is just for fun:-)
          local_stage_EXE_ALUResult = temp_local_stage_EXE_ALUResult;
        else
          local_stage_EXE_ALUResult = -1*temp_local_stage_EXE_ALUResult;
      }
      if(ID_to_EXE_funct == 24){ //mult
#if DEBUG
        cout<<"\n **multiplication** "<<ID_to_EXE_r1<<"+"<<ID_to_EXE_r2<<"\n";
#endif
        local_stage_EXE_Mflag = false;
        local_stage_EXE_ALUResult = ID_to_EXE_r1 * ID_to_EXE_r2;
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 25){ //multu
        local_stage_EXE_Mflag = false;
        local_stage_EXE_ALUResult = (unsigned int)ID_to_EXE_r1 * (unsigned int)ID_to_EXE_r2;
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 26){ //div
        local_stage_EXE_Mflag = false;
        local_stage_EXE_ALUResult =   ID_to_EXE_r1 / ID_to_EXE_r2;
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 27){ //divu
        local_stage_EXE_Mflag = false;
        local_stage_EXE_ALUResult = (unsigned int)ID_to_EXE_r1 / (unsigned int)ID_to_EXE_r2;
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 36){ //and
        local_stage_EXE_ALUResult = ID_to_EXE_r1 & ID_to_EXE_r2;
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 37){ //or
        local_stage_EXE_ALUResult = ID_to_EXE_r1 | ID_to_EXE_r2;
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 0){ //sll
        local_stage_EXE_ALUResult =  (ID_to_EXE_r1 << ID_to_EXE_shamt);
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 2){ //srl
        local_stage_EXE_ALUResult =  (ID_to_EXE_r1 >> ID_to_EXE_shamt);
        //put result in r30 during writeback stage
      }
      if(ID_to_EXE_funct == 42)
        { //slt
          if(ID_to_EXE_r1 < ID_to_EXE_r2)
            local_stage_EXE_ALUResult = 1;
          else
            local_stage_EXE_ALUResult = 0;
        }
      if(ID_to_EXE_funct == 43)
        { //sltu
          if((unsigned int) ID_to_EXE_r1 < (unsigned int) ID_to_EXE_r2)
            local_stage_EXE_ALUResult = 1;
          else
            local_stage_EXE_ALUResult = 0;
        }

      break;
    }

    local_stage_EXE_immediate = ID_to_EXE_immediate;
    local_stage_EXE_opcode = ID_to_EXE_opcode;
    local_stage_EXE_r2 = ID_to_EXE_r2;
    local_stage_EXE_reg3 = ID_to_EXE_reg3;

    /***********************Here we start the MEMORY ACCESS stage*************************/
    if(EXE_to_MEM_opcode == 35){//lw
      local_stage_WB_DataResult = Data_Mem[EXE_to_MEM_ALUResult];
    }

    if(EXE_to_MEM_opcode == 43){//sw
#if DEBUG
      cout<<"Writing to memory = "<<EXE_to_MEM_ALUResult<<"\tvalue = "<< EXE_to_MEM_r2<<"\n";
#endif
      Data_Mem[EXE_to_MEM_ALUResult]= EXE_to_MEM_r2;
    }

    local_stage_WB_Write_Register_File_Flag = EXE_to_MEM_Write_Register_File_Flag;
    local_stage_WB_ALUResult = EXE_to_MEM_ALUResult;
    local_stage_WB_reg3 = EXE_to_MEM_reg3;
    local_stage_WB_Mflag = EXE_to_MEM_Mflag;
    local_stage_WB_opcode = EXE_to_MEM_opcode;

    /***********************Here we start the REGISTER WRITE BACK stage*************************/
    if(MEM_to_WB_Write_Register_File_Flag == true && MEM_to_WB_opcode != 7 && MEM_to_WB_opcode != 6)
      {
        if(MEM_to_WB_opcode==35){//lw
          RegFile[MEM_to_WB_reg3]= MEM_to_WB_DataResult;
        }
        else if (MEM_to_WB_opcode != 43){
          RegFile[MEM_to_WB_reg3]= MEM_to_WB_ALUResult;
#if DEBUG
          cout<<"Written "<<RegFile[MEM_to_WB_reg3]<<" to register r"<<MEM_to_WB_reg3<<endl;
#endif
        }
      }
    else
#if DEBUG
      cout<<"Written Nothing to register file"<<endl;
#endif
    RegFile[0] = 0; // r0 is always 0;
#if DEBUG
    cout<<"IF_to_ID_INSTR.getopcode() = "<<IF_to_ID_INSTR.getopcode()<<"\n";
    cout<<"IF_to_ID_INSTR.getrt() = "<<IF_to_ID_INSTR.getrt()<<"\n";
    cout<<"IF_to_ID_INSTR.getrs() = "<<IF_to_ID_INSTR.getrs()<<"\n";
    cout<<"IF_to_ID_INSTR.getrd() = "<<IF_to_ID_INSTR.getrd()<<"\n";
    cout<<"IF_to_ID_reg3 = "<<reg3<<"\n";
    cout<<"ID_to_EXE_opcode = "<<ID_to_EXE_opcode<<"\n";
    cout<<"ID_to_EXE_reg3 = "<<ID_to_EXE_reg3<<"\n";
#endif
    if(((ID_to_EXE_opcode == 35)&&((IF_to_ID_INSTR.getrs()==ID_to_EXE_reg3)||(IF_to_ID_INSTR.getrt()==ID_to_EXE_reg3))))
      {
        STALL = 1;
        if(IF_to_ID_INSTR.getopcode() == 43 && IF_to_ID_INSTR.getrt() == ID_to_EXE_reg3 && IF_to_ID_INSTR.getrs() != ID_to_EXE_reg3)
          STALL=0;

#if DEBUG
        cout<<"Got Stall"<<endl;
#endif
      }

    /********************Propagation of the register values**************************************/

    if (STALL==0 && BRANCH == 0 && BRANCH_TKN == 0) {//If there is no stall propagate values of the register from fetch to decode and further
      //****IF to ID******
      IF_to_ID_PC = PC - 1;
      IF_to_ID_INSTR = INSTR;
      //****ID to EXE******
      ID_to_EXE_PC = IF_to_ID_PC;
      ID_to_EXE_opcode = opcode;
      ID_to_EXE_reg3 = reg3;
      ID_to_EXE_shamt = shamt;
      ID_to_EXE_funct= funct;
      ID_to_EXE_immediate = immediate;
      ID_to_EXE_r1 = r1;
      ID_to_EXE_r2 = r2;
      ID_to_EXE_target = target;
      ID_to_EXE_Write_Register_File_Flag = local_stage_ID_Write_Register_File_Flag;
      ID_to_EXE_reg1 = reg1;
      ID_to_EXE_reg2 = reg2;
    }
    else if(STALL==1){
      ID_to_EXE_opcode = 7;
      ID_to_EXE_Write_Register_File_Flag = false;
    }

    //****EXE to MEM******
    EXE_to_MEM_ALUResult = local_stage_EXE_ALUResult;
    EXE_to_MEM_Zero = local_stage_EXE_Zero;
    EXE_to_MEM_r2 = local_stage_EXE_r2;
    EXE_to_MEM_reg3 = local_stage_EXE_reg3;
    EXE_to_MEM_Mflag = local_stage_EXE_Mflag;
    EXE_to_MEM_opcode = local_stage_EXE_opcode;
    local_stage_EXE_Mflag = true;
    local_stage_EXE_Zero = false;
    EXE_to_MEM_immediate = local_stage_EXE_immediate;
    EXE_to_MEM_Write_Register_File_Flag = local_stage_EXE_Write_Register_File_Flag;

    //****MEM to WB******
    MEM_to_WB_opcode= local_stage_WB_opcode;
    MEM_to_WB_DataResult = local_stage_WB_DataResult;
    MEM_to_WB_reg3 = local_stage_WB_reg3;
    MEM_to_WB_ALUResult = local_stage_WB_ALUResult;
    MEM_to_WB_Mflag = local_stage_WB_Mflag;
    MEM_to_WB_Write_Register_File_Flag = local_stage_WB_Write_Register_File_Flag;

    /*****************************************************************************************/
#if DEBUG
    cout<<"Values before data hazard detection and after pipeline propagation"<<endl;
    cout<<"pipelines fetch to decode :::::::::::::IF_ID\n";
    cout<<"IF_to_ID_PC: "<<IF_to_ID_PC<<endl;

    cout<<"pipelines decode to execute ::::::::::::\n";
    cout<<"ID_to_EXE_PC ::: "<<ID_to_EXE_PC<<endl;
    cout<<"ID_to_EXE_opcode :: "<<ID_to_EXE_opcode<<endl;
    cout<<"ID_to_EXE_reg3  "<<ID_to_EXE_reg3<<endl;
    cout<<"ID_to_EXE_shamt  "<<ID_to_EXE_shamt<<endl;
    cout<<"ID_to_EXE_funct  "<<ID_to_EXE_funct<<endl;
    cout<<"ID_to_EXE_r1  "<<ID_to_EXE_r1<<endl;
    cout<<"ID_to_EXE_r2  "<<ID_to_EXE_r2<<endl;
    cout<<"ID_to_EXE_target  "<<ID_to_EXE_target<<endl;
    cout<<"ID_to_EXE_reg1  "<<ID_to_EXE_reg1<<endl;
    cout<<"ID_to_EXE_reg2  "<<ID_to_EXE_reg2<<endl;

    cout<<"pipelines execute TO mem::::::::::::\n";
    cout<<"EXE_to_MEM_opcode "<<EXE_to_MEM_opcode<<endl;
    cout<<"EXE_to_MEM_reg3 "<<EXE_to_MEM_reg3<<endl;
    cout<<"EXE_to_MEM_ALUResult"<<EXE_to_MEM_ALUResult<<endl;

    cout<<"pipelines mem TO wb:::::::::::::\n";
    cout<<"MEM_to_WB_opcode  "<<MEM_to_WB_opcode<<endl;
    cout<<"MEM_to_WB_reg3  "<<MEM_to_WB_reg3<<endl;
    cout<<"MEM_to_WB_ALUResult"<<MEM_to_WB_ALUResult<<endl;
#endif
    /**************Detection of hazards*************************/
    
    /*********RAW hazards: compare output register of instruction in execute stage with inputs of that in decode stage*********/
    //Detecting when result of MEM/WB can be forwarded to ALU's input i.e. ID/EXE register

    if((ID_to_EXE_opcode != 7) && (MEM_to_WB_opcode != 7) && (MEM_to_WB_opcode != 4)&& (MEM_to_WB_opcode != 5)&& (MEM_to_WB_opcode != 2)&& (MEM_to_WB_opcode != 1)&& (MEM_to_WB_opcode != 43))
      {
        if(ID_to_EXE_reg1==MEM_to_WB_reg3)
          {
#if DEBUG
            cout<< "data forwarding from write back stage"<<endl;
            cout<<"inside forwarding logic::: old:ID_to_EXE_r1 "<<ID_to_EXE_r1<<endl;
#endif
            if(MEM_to_WB_opcode == 35)
              ID_to_EXE_r1 = MEM_to_WB_DataResult;
            else
              ID_to_EXE_r1 = MEM_to_WB_ALUResult;
#if DEBUG
            cout<<" new:"<<ID_to_EXE_r1<<endl;
#endif

          }
        if(ID_to_EXE_opcode != 8 && ID_to_EXE_opcode != 23)
          {
            if(ID_to_EXE_reg2==MEM_to_WB_reg3)  {
#if DEBUG
              cout<< "\ndata forwarding from write back stage "<<endl;
              cout<<"\ninside forwarding logic::: old:ID_to_EXE_r2 "<<ID_to_EXE_r2<<endl;
#endif
              if(MEM_to_WB_opcode == 35)
                ID_to_EXE_r2 = MEM_to_WB_DataResult;
              else
                ID_to_EXE_r2 = MEM_to_WB_ALUResult;
#if DEBUG
              cout<<" new:"<<ID_to_EXE_r2<<endl;
#endif

            }
          }

      }
    /*  its working for normal alu data forwarding */
    if((ID_to_EXE_opcode != 7) && (EXE_to_MEM_opcode != 7) && (EXE_to_MEM_opcode != 4)&& (EXE_to_MEM_opcode != 5)&& (EXE_to_MEM_opcode != 2)&& (EXE_to_MEM_opcode != 1)&& (EXE_to_MEM_opcode != 43))
      {
        if(ID_to_EXE_reg1==EXE_to_MEM_reg3)
          {
#if DEBUG
            cout<< "data forwarding case "<<endl;
            cout<<"inside forwarding logic::: old: "<<ID_to_EXE_r1;
#endif
            //forwarding logic: forward from ALU output of execute stage to ALU input
            ID_to_EXE_r1 = EXE_to_MEM_ALUResult;
#if DEBUG
            cout<<" new:"<<ID_to_EXE_r1<<endl;
#endif

          }
        if(ID_to_EXE_opcode != 8 && ID_to_EXE_opcode != 23)
          {
            if(ID_to_EXE_reg2==EXE_to_MEM_reg3)
              {
#if DEBUG
                cout<< "data forwarding case"<<endl;
                cout<<"inside forwarding logic::: old: "<<ID_to_EXE_r2<<endl;
#endif
                //forwarding logic: forward from ALU output of execute stage to ALU input
                ID_to_EXE_r2 = EXE_to_MEM_ALUResult;
#if DEBUG
                cout<<" new:"<<ID_to_EXE_r2<<endl;
#endif
              }
          }
      }

    //hardware to check branch condition and calculate the next PC
    if(STALL==0)
      {
        switch(opcode){
        case 4://be
          BRANCH = 1;
#if DEBUG
          cout<<"In branch be r1 = "<<ID_to_EXE_r2 <<"\t r2 = "<<ID_to_EXE_r1<<endl;
#endif
          if((ID_to_EXE_r1 - ID_to_EXE_r2) == 0)
            {
              PC=immediate ;//calculate the target address of instruction
              INSTR_COUNT = INSTR_COUNT - 1;
              BRANCH_TKN = 1;
            }
          break;

        case 5://bne
          BRANCH = 1;
#if DEBUG
          cout<<"In branch bne r1 = "<<ID_to_EXE_r2 <<"\t r2 = "<<ID_to_EXE_r1<<endl;
#endif
          if((ID_to_EXE_r1 - ID_to_EXE_r2) != 0)
            {
              PC=immediate ;//calculate the target address of instruction
              INSTR_COUNT = INSTR_COUNT - 1;
              BRANCH_TKN = 1;
            }
          break;

        case 1://bge
          BRANCH = 1;
#if DEBUG
          cout<<"In branch bge r1 = "<<ID_to_EXE_r2 <<"\t r2 = "<<ID_to_EXE_r1<<endl;
#endif
          if(ID_to_EXE_r2 >= ID_to_EXE_r1)
            {
              PC=immediate ;//calculate the target address of instruction
              INSTR_COUNT = INSTR_COUNT - 1;
              BRANCH_TKN = 1;
            }
          break;

        case 2:
#if DEBUG
          cout<<"****** jump Target ******"<<target<<endl;
          cout<<"****** jump immediate ******"<<immediate<<endl;
#endif
          BRANCH = 1;
          PC=immediate;//jump
          BRANCH_TKN = 1;
          break;
        default:
          BRANCH = 0;

        }
      }
    //Finding the exit opcode to end simulation
    if(INSTR.getopcode()==6 && STALL ==0 && BRANCH_TKN != 1){
#if DEBUG
      cout<<"\nGot exit instruction\n";
#endif
      exit_flag = true;
    }

    if(STALL == 0 && BRANCH_TKN == 1)
      {
        BRANCH_TKN = 0;
        BRANCH = 0;
        IF_to_ID_INSTR = dec2bin(7, 6) + dec2bin(0,26);// opcode for nop

        //decode to execute
        ID_to_EXE_PC = IF_to_ID_PC;
        ID_to_EXE_opcode = 7;
        ID_to_EXE_reg3 = 0;
        ID_to_EXE_shamt = 0;
        ID_to_EXE_funct= 0;
        ID_to_EXE_immediate = 0;
        ID_to_EXE_r1 = 0;
        ID_to_EXE_r2 = 0;
        ID_to_EXE_target = 0;
        ID_to_EXE_reg1 = 0;
        ID_to_EXE_reg2 = 0;
        ID_to_EXE_Write_Register_File_Flag = false;
      } else if(STALL==0 && BRANCH == 1 && BRANCH_TKN == 0){
      //If there is stall do not propagate instruction fetched ,instead insert a nop in that place
      //fetch to decode
      BRANCH = 0;
      IF_to_ID_PC = PC-1;
      IF_to_ID_INSTR = INSTR;

      //decode to execute
      ID_to_EXE_PC = IF_to_ID_PC;
      ID_to_EXE_opcode = 7;
      ID_to_EXE_reg3 = 0;
      ID_to_EXE_shamt = 0;
      ID_to_EXE_funct= 0;
      ID_to_EXE_immediate = 0;
      ID_to_EXE_r1 = 0;
      ID_to_EXE_r2 = 0;
      ID_to_EXE_target = 0;
      ID_to_EXE_reg1 = 0;
      ID_to_EXE_reg2 = 0;
      ID_to_EXE_Write_Register_File_Flag = false;
    }
    /*******************************************************************************************************************/

    if(latency_to_end == 0) {
      cycle++;
      break;
    }

    //We need to give some clock cycles before finishing simulation
    if(exit_flag == true) {
      latency_to_end--;
    }
    cycle++;

  }//end of pipelining loop

  CPI = ((double)(cycle) /  (double)(INSTR_COUNT-1));

  cout <<endl<< "RESULTS AFTER SIMULATION:";
  cout << endl<< "******************Performance analysis:*******************"<<endl;
  cout << "Clock cycles used by the program: " << cycle <<endl;
  cout << "Instruction Count: " << INSTR_COUNT-1 <<endl;
  cout << "Stall count: " << stall_count <<endl;
  cout << "CPI: " << CPI <<endl;

  return true;
}

unsigned int processor::getInstructionCount() {
  return InstructionCount;
}

bool processor::ShowRegisterFile() {
  //Shows Regfile Content
  int i;
  cout << endl<< "******************Register File Content:******************"<<endl;
  for(i=0;i < 32;i=i+4){
    cout <<"R"<<i<<": "<<RegFile[i]<<"\t" <<"R"<<i+1<<": "<<RegFile[i+1]<<"\t" <<"R"<<i+2<<": "<<RegFile[i+2]<<"\t" <<"R"<<i+3<<": "<<RegFile[i+3]<<"\t" << endl;
  }
  return true;
}

bool processor::ShowInstructionMemory() {
  unsigned int i;
  //Show content of instruction memory
  cout << endl<< "******************Instruction Memory:*********************"<<endl;
  for(i=0;i < (unsigned int) InstructionCount && i < ImemSize ;i++){
    cout << Inst_Mem[i] << endl;
  }

  return true;
}

bool processor::ShowDataMemory() {
  unsigned int i;
  //Shows content of data memory
  cout << endl<< "**********************Data Memory:************************"<<endl;
  for(i=0;i < DataMemoryRange;i++){
    cout << "Word #"<<i<<": " <<Data_Mem[i]<< ""<<endl;
  }
  return true;
}

bool processor::InitializeSimulation() {
  //Initialize Program Counter, internal nodes and all pipeline registers to 0
  //IF
  PC = 0 ;
  INSTR = dec2bin(0,32);

  //IF_to_ID
  IF_to_ID_PC = 0;
  IF_to_ID_INSTR = dec2bin(7,6) + dec2bin(0,26);
  IF_to_ID_Write_Register_File_Flag = true;

  //ID
  //int local_stage_ID_PC = 0;
  opcode=7;
  reg3 = 0;
  shamt = 0;
  funct= 0;
  immediate = 0;
  r1 = 0;
  r2 = 0;
  target = 0;

  //ID_to_EXE
  ID_to_EXE_PC = 0;//used for jump and link
  ID_to_EXE_opcode=7;
  ID_to_EXE_reg3 = 0;
  ID_to_EXE_shamt = 0;
  ID_to_EXE_funct= 0;
  ID_to_EXE_immediate = 0;
  ID_to_EXE_r1 = 0;
  ID_to_EXE_r2 = 0;
  ID_to_EXE_target = 0;
  ID_to_EXE_Write_Register_File_Flag = true;
  ID_to_EXE_reg1=0;
  ID_to_EXE_reg2=0;

  //EXE
  temp_local_stage_EXE_ALUResult=0;// for unsigned operations
  local_stage_EXE_ALUResult = 0;
  local_stage_EXE_Zero = false;
  local_stage_EXE_r2 = 0;
  local_stage_EXE_reg3 = 0;
  local_stage_EXE_Mflag = true;
  local_stage_EXE_opcode = 7;
  local_stage_EXE_immediate = 0;
  local_stage_EXE_Write_Register_File_Flag = true;

  //EXE_MEM
  EXE_to_MEM_ALUResult = 0;
  EXE_to_MEM_Zero = false;
  EXE_to_MEM_r2 = 0;
  EXE_to_MEM_reg3 = 0;
  EXE_to_MEM_Mflag = true;
  EXE_to_MEM_opcode = 7;
  EXE_to_MEM_Write_Register_File_Flag = true;

  //MEM
  local_stage_WB_opcode= 7;
  local_stage_WB_DataResult = 0;
  local_stage_WB_reg3 = 0;
  local_stage_WB_ALUResult = 0;
  local_stage_WB_Mflag = true;
  local_stage_WB_Write_Register_File_Flag = true;

  //MEM_to_WB
  MEM_to_WB_opcode= 7;
  MEM_to_WB_DataResult = 0;
  MEM_to_WB_reg3 = 0;
  MEM_to_WB_ALUResult = 0;
  MEM_to_WB_Mflag = true;
  MEM_to_WB_Write_Register_File_Flag = true;
  return true;
};

string processor::dec2bin(int input, int m) {
  string result;
  if (input < 0) {
    input = power(2,m)+input;
    result = dec2bin(input, m);
    return(result);
  } else {
    int i = 0;
    //char array[m+1];
    char * array;
    array=(char*)malloc((m+1)*sizeof(char));

    int limit = 0;
    limit = power(2, m);
    const int MASK = limit/2; // the binary equivalent is: 1 + m 0 bits

    array[m] = '\0';
    for (i= 0; i < m ;i++)
      {
        if (input & MASK) {
          array[i]='1';}
        else{
          array[i]='0';}
        input = input << 1;
      }

    result = string(array);
    return (result);
  }
}

// convert a binary string to a decimal number, returns decimal value
int processor::bin2dec(const char *bin)
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

int processor::power ( int base, int exponent ) {
  long p=1;
  int i;
  for ( i=1 ; i<=exponent ; i++) {
    p*=base;
  }
  return(p);
}

string processor::trimString(string str)
{
  char const* delims = " \t\r\n";
  //remove leading whitespace
  string::size_type  notwhite = str.find_first_not_of(delims);
  str.erase(0,notwhite);

  //remove trailing whitespace
  notwhite = str.find_last_not_of(delims);
  str.erase(notwhite+1);
  return str;
}
