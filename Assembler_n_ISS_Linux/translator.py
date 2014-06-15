#!/usr/intel/bin/python2.7
# (c) Intel 2013

import os
import sys
import re
from optparse import OptionParser
from os import listdir
from os.path import isfile, join

def bin(x, digits=0): 
    oct2bin = ['000','001','010','011','100','101','110','111'] 
    binstring = [oct2bin[int(n)] for n in oct(x)] 
    return ''.join(binstring).lstrip('0').zfill(digits) 

def switch_map(opcode,rs,rt,rd,imm):
    smap = {"000000": bin(0,6)+rs+rt+rd+"00000"+bin(32,6), #ADD
            "000001": bin(8,6)+rs+rt+imm, #ADDI
            "000010": bin(0,6)+rs+rt+rd+"00000"+bin(34,6), #SUB
            "000011": bin(23,6)+rs+rt+imm, #SUBI
            "000100": bin(0,6)+rs+rt+rd+"00000"+bin(24,6), #MUL
            "000101": bin(15,6)+rs+rt+imm, #MULI
            "000110": bin(0,6)+rs+rt+rd+"00000"+bin(36,6), #AND
            "000111": bin(12,6)+rs+rt+imm, #ANDI
            "001000": bin(0,6)+rs+rt+rd+"00000"+bin(37,6), #OR
            "001001": bin(13,6)+rs+rt+imm, #ORI
            "001010": bin(0,6)+rs+rt+rd+"00000"+bin(38,6), #XOR
            "001011": bin(14,6)+rs+rt+imm, #XORI
            "001100": bin(35,6)+rs+rt+imm, #LDW
            "001101": bin(43,6)+rs+rt+imm, #STW
            "001110": bin(4,6)+rs+rt+imm, #BEQ
            "001111": bin(4,6)+rs+"00000"+imm, #BZ
            "010000": bin(0,6)+rs+"00000"+"00000"+"00000"+bin(8,6), #JR
            "010001": bin(6,6)+bin(0,26) #HALT
            }
    result = smap[opcode]
    if result == "00000000000000000000000000100000":
        result = bin(0,32)

    return result

def convertasm(x):
    opcode = x[:6]
    rs = x[6:11]
    rt = x[11:16]
    rd = x[16:21]
    imm = x[16:]
    newcode = switch_map(opcode,rs,rt,rd,imm)
    return newcode

parser = OptionParser(description="Program to translate class custom binary to standard MIPS binary",
                      version="%prog 1.0")
parser.add_option("-f", "--file",
                  dest="file_to_convert",
                  default="",
                  help="Specify a file to convert")
(options, args) = parser.parse_args()
input_file = options.file_to_convert
f = open(input_file)
fnew = open(input_file+"asm",'w')
hexlist = [line.strip() for line in f]

binlist = [convertasm(bin(int(x, 16),32)) for x in hexlist]

for x in binlist:
    print x

f.close()
fnew.close()
