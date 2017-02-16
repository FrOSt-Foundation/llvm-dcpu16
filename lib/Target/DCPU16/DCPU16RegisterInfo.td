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

class DCPU16Reg<bits<4> num, string n> : Register<n> {
  field bits<4> Num = num;
  let Namespace = "DCPU16";
}

class DCPU16RegWithSubregs<bits<4> num, string n, list<Register> subregs> 
  : RegisterWithSubRegs<n, subregs> {
  field bits<4> Num = num;
  let Namespace = "DCPU16";
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def PCB  : DCPU16Reg<0,  "r0">;
def SPB  : DCPU16Reg<1,  "r1">;
def SRB  : DCPU16Reg<2,  "r2">;
def CGB  : DCPU16Reg<3,  "r3">;
def FPB  : DCPU16Reg<4,  "r4">;
def R5B  : DCPU16Reg<5,  "r5">;
def R6B  : DCPU16Reg<6,  "r6">;
def R7B  : DCPU16Reg<7,  "r7">;
def R8B  : DCPU16Reg<8,  "r8">;
def R9B  : DCPU16Reg<9,  "r9">;
def R10B : DCPU16Reg<10, "r10">;
def R11B : DCPU16Reg<11, "r11">;
def R8JB : DCPU16Reg<12, "r12">;
def R13B : DCPU16Reg<13, "r13">;
def R14B : DCPU16Reg<14, "r14">;
def R15B : DCPU16Reg<15, "r15">;

def subreg_8bit : SubRegIndex<8> { let Namespace = "DCPU16"; }

let SubRegIndices = [subreg_8bit] in {
def PC  : DCPU16RegWithSubregs<0,  "r0",  [PCB]>;
def SP  : DCPU16RegWithSubregs<1,  "r1",  [SPB]>;
def SR  : DCPU16RegWithSubregs<2,  "r2",  [SRB]>;
def CG  : DCPU16RegWithSubregs<3,  "r3",  [CGB]>;
def FP  : DCPU16RegWithSubregs<4,  "r4",  [FPB]>;
def R5  : DCPU16RegWithSubregs<5,  "r5",  [R5B]>;
def R6  : DCPU16RegWithSubregs<6,  "r6",  [R6B]>;
def R7  : DCPU16RegWithSubregs<7,  "r7",  [R7B]>;
def R8  : DCPU16RegWithSubregs<8,  "r8",  [R8B]>;
def R9  : DCPU16RegWithSubregs<9,  "r9",  [R9B]>;
def R10 : DCPU16RegWithSubregs<10, "r10", [R10B]>;
def R11 : DCPU16RegWithSubregs<11, "r11", [R11B]>;
def R12 : DCPU16RegWithSubregs<12, "r12", [R8JB]>;
def R13 : DCPU16RegWithSubregs<13, "r13", [R13B]>;
def R14 : DCPU16RegWithSubregs<14, "r14", [R14B]>;
def R15 : DCPU16RegWithSubregs<15, "r15", [R15B]>;
}

def GR8 : RegisterClass<"DCPU16", [i8], 8,
   // Volatile registers
  (add R8JB, R13B, R14B, R15B, R11B, R10B, R9B, R8B, R7B, R6B, R5B,
   // Frame pointer, sometimes allocable
   FPB,
   // Volatile, but not allocable
   PCB, SPB, SRB, CGB)>;

def GR16 : RegisterClass<"DCPU16", [i16], 16,
   // Volatile registers
  (add R12, R13, R14, R15, R11, R10, R9, R8, R7, R6, R5,
   // Frame pointer, sometimes allocable
   FP,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
