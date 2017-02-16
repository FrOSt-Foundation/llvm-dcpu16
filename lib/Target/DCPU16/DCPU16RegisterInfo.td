//===-- DCPU16RegisterInfo.td - DCPU16 Register defs -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the DCPU16 register file
//===----------------------------------------------------------------------===//

class DCPU16Reg<bits<5> num, string n> : Register<n> {
  field bits<5> Num = num;
  let Namespace = "DCPU16";
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def RA  : DCPU16Reg<0, "A">;
def RB  : DCPU16Reg<1, "B">;
def RC  : DCPU16Reg<2, "C">;
def RX  : DCPU16Reg<3, "X">;
def RY  : DCPU16Reg<4, "Y">;
def RZ  : DCPU16Reg<5, "Z">;
def RI  : DCPU16Reg<6, "I">;
def RJ  : DCPU16Reg<7, "J">;

def RSP : DCPU16Reg<0x1b, "SP">;
def REX  : DCPU16Reg<0x1d, "EX">;

// volatile registers
def GR16 : RegisterClass<"DCPU16", [i16], 16,
    (add RA, RB, RC, RX, RY, RZ, RI, RJ)>;

// stack pointer register
def SPR16 : RegisterClass<"DCPU16", [i16], 16,
    (add RSP)>;

// overflow register
def EXR16 : RegisterClass<"DCPU16", [i16], 16,
    (add REX)> {
        let isAllocatable = 0;
}
