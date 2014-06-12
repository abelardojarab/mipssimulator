#include <fstream>
#include <iostream>
#include <signal.h>
#include <string>
#include <cstdio>
#include <cstdlib>
//#include <unistd.h>
#include "instruction.h"
#include "instruction.cpp"
//#include <sys/time.h>

using std::ifstream;
using namespace std;

//Prototype functions declarations
//Loads the Assembler file
bool LoadAssembler(string filename);

//Transforms an array of strings into a string with the 32-bit binary representation of the instruction 
string converter(string word[]);

//Removes spaces in the string at its left and at its right
string trimString(string str);

//Takes bstring which it contains a string with a binary representation, and concatenates string str after being transformed to binary too
string concatenate(string bstring, string str, int m);

//Transform an array of characters containing a binary representation into a number
int bin2dec(const char *bin);

//Transform an integer number input into a string with its signed m-bits binary representation
string dec2bin(int input, int m);

//Shows the final Assembler code after being processed by the Assembler
bool ShowAssemblerCode();

//Return base elevated to the exponent
int power ( int base, int exponent );

list<string> AssemblerCode;
list<instruction> Asm_Code;
int InstructionCount;

int main(int argc, char* argv[]) 
{     
if (argc != 2) {
	cout<<"Assembler for CA Project - ECE UFL 08"<<endl;
	cout<<"Usage: assembler [code].asm\n";
	cout<<endl;
	return 1;
}

string fileasm(argv[1]);
LoadAssembler(fileasm);
ShowAssemblerCode();
cout<<endl;

AssemblerCode.clear();
Asm_Code.clear();

return(0);
}

//Here starts function declarations
bool LoadAssembler(string filename) 
{
	char read_char_buffer[150];
	string split_String[6];
	string temps;
	
	char* charpointer;
	int x;
	int num = 0;

	ifstream ifs;
	ifs.open(filename.c_str()); // opens the file
	if (!ifs.is_open()) { // file couldn't be opened
		cerr << "Error: Assembler file could not be opened." << endl;
		exit(1);
	}
	cout<<endl<<"************Assembler:**********"<<endl;
	string labels = "";
	while( !ifs.eof() ) {			
	ifs.getline(read_char_buffer,256);			
	temps = read_char_buffer;

	//Split the line which has been read from the file
	x = 0;
	if (trimString(read_char_buffer) != "") {
		size_t found=trimString(read_char_buffer).find(':');
		charpointer = strtok (read_char_buffer,"	,.$: ");
		while (charpointer != NULL)
		{	split_String[x]= trimString(charpointer);
			charpointer = strtok (NULL, " ,.$r() ");  //this is for next searchs
			 x++;
		}
		if (found != string::npos) {
			labels = split_String[0];
			continue;
		} else {
		
			AssemblerCode.push_back(converter(split_String));
			Asm_Code.push_back(instruction(converter(split_String),temps,labels));
			labels="";
			InstructionCount++;
		};
	}
	
	}
	ifs.close();

	//Now we will remove all loops and replace jumps or branches with the absolute address
	cout<<"Finding and replacing labels in the Assembler code:"<<endl;
	int instructionaddress=0;
	list<instruction>::iterator posi, posj;
	posi = Asm_Code.begin();		
	while (posi!=Asm_Code.end()) {
		if ((*posi).getlabelstring() != "") {

			posj = Asm_Code.begin();
			while (posj!=Asm_Code.end()) {

				size_t found=trimString((*posj).getinstructionl().c_str()).find((*posi).getlabelstring());

				if (found != string::npos) {
					cout<<"Branch/jump to \""<<(*posi).getlabelstring()<<"\" found at instruction: "<<(*posj).getinstructionl()<<endl;
					if ((*posj).getopcode()==4 || (*posj).getopcode()==5 || (*posj).getopcode()==1) {
						(*posj).setimm_or_add(instructionaddress);
					}
					if ((*posj).getopcode()==2 || (*posj).getopcode()==3) {
						(*posj).settarget(instructionaddress);
					}

				}
			posj++;
			}
		}
		instructionaddress++;
		posi++;
	}

	cout<<endl;

	//Now we dump the final Assembler code to the output file
	string cg = "Asm_output_";
	filename=cg+filename;
	ofstream myfile (filename.c_str());
	posi = Asm_Code.begin();
	if (myfile.is_open()) {
	while (posi!=Asm_Code.end()) {
		myfile << (*posi).getinstructionb();
		myfile << endl;
		posi++;
	};	
	myfile.close(); }			
	else cout << "Unable to open the output assembler file.";

return true;
}

bool ShowAssemblerCode() {
	int num=0;
	list<instruction>::iterator pos;
	pos = Asm_Code.begin();
	cout<<"Clean assembler code:"<<endl;
	while (pos!=Asm_Code.end()) {
		cout<<num<<"\t"<<(*pos).getinstructionb()<<"\t"<<(*pos).getinstructionl()<<"\t\t"<<(*pos).getlabelstring()<<endl;
		num++;
		pos++;
	}
	cout<<"\tProgram size: "<<InstructionCount<<" instructions"<<endl;
	return true;
}

string converter(string word[]) {
string firstbinaryString;
//We add here the R-type instructions
if (word[0].compare("add") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("32"),6);
} else if (word[0].compare("sub") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("34"),6);
} else if (word[0].compare("addu") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("33"),6);
} else if (word[0].compare("subu") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("35"),6);
} else if (word[0].compare("mult") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,"30",5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("24"),6);
} else if (word[0].compare("multu") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("25"),6);	
} else if (word[0].compare("div") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,"30",5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("26"),6);
} else if (word[0].compare("divu") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("27"),6);
} else if (word[0].compare("and") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("36"),6);
} else if (word[0].compare("or") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("37"),6);
} else if (word[0].compare("sll") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,string("00"),5);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,string("00"),6);
} else if (word[0].compare("srl") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,string("00"),5);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,string("02"),6);
} else if (word[0].compare("slt") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("42"),6);
} else if (word[0].compare("sltu") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("43"),6);
} else if (word[0].compare("jr") == 0) {
firstbinaryString = dec2bin(0,6);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = firstbinaryString + "00000";
firstbinaryString = concatenate(firstbinaryString,string("08"),6);

}//We add here the I-type instructions
else if (word[0].compare("addi") == 0) {
firstbinaryString = dec2bin(8,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("addiu") == 0) {
firstbinaryString = dec2bin(9,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("subi") == 0) {
firstbinaryString = dec2bin(23,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("andi") == 0) {
firstbinaryString = dec2bin(12,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("ori") == 0) {
firstbinaryString = dec2bin(13,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("load") == 0) {
firstbinaryString = dec2bin(35,6);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[2],16);
} else if (word[0].compare("store") == 0) {
firstbinaryString = dec2bin(43,6);
firstbinaryString = concatenate(firstbinaryString,word[3],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[2],16);
} else if (word[0].compare("lui") == 0) {
firstbinaryString = dec2bin(15,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("beq") == 0) {
firstbinaryString = dec2bin(4,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("bne") == 0) {
firstbinaryString = dec2bin(5,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("bge") == 0) {//branch greater than
firstbinaryString = dec2bin(1,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("slti") == 0) {
firstbinaryString = dec2bin(10,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);
} else if (word[0].compare("sltiu") == 0) {
firstbinaryString = dec2bin(11,6);
firstbinaryString = concatenate(firstbinaryString,word[2],5);
firstbinaryString = concatenate(firstbinaryString,word[1],5);
firstbinaryString = concatenate(firstbinaryString,word[3],16);	
}//We add here the J-type instructions
else if (word[0].compare("j") == 0) {
firstbinaryString = dec2bin(2,6);
firstbinaryString = concatenate(firstbinaryString,word[1],26);
} else if (word[0].compare("jal") == 0) {
firstbinaryString = dec2bin(3,6);
firstbinaryString = concatenate(firstbinaryString,word[1],26);
} else if (word[0].compare("exit") == 0) {			
firstbinaryString = dec2bin(6,6);
firstbinaryString = firstbinaryString + "00000000000000000000000000";
} else if (word[0].compare("nop") == 0) {
firstbinaryString = dec2bin(7,6);
firstbinaryString = firstbinaryString + "00000000000000000000000000";
} else {
firstbinaryString = word[0];
}
return firstbinaryString;	
}
string trimString(string str)
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

string concatenate(string bstring, string str, int m)
{
bstring = bstring + dec2bin(atoi(str.c_str()),m);
return bstring;
}

string dec2bin(int input, int m)
{
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
int bin2dec(const char *bin)
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

int power ( int base, int exponent ) {
    long p=1;
    int i;
    for ( i=1 ; i<=exponent ; i++) {
        p*=base;
    }
    return(p);
}


