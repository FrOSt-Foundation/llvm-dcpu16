//==- DCPU16CallingConv.td - Calling Conventions for DCPU16 -*- tablegen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for DCPU16 architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// DCPU16 Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_DCPU16 : CallingConv<[
  // promote i8 return values to i16
  CCIfType<[i8], CCPromoteToType<i16>>,

  // i16 are returned in registers A, B, C
  CCIfType<[i16], CCAssignToReg<[A, B, C]>>
]>;

//===----------------------------------------------------------------------===//
// DCPU16 Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_DCPU16_AssignStack : CallingConv<[
  // Promote i8 arguments to i16.
  CCIfType<[i8], CCPromoteToType<i16>>,

  //the first 3 integers args of non var-args functions are passed in integer registers
  CCIfNotVarArg<CCIfType<[i16], CCAssignToReg<[A, B, C]>>>,

  // Integer values get stored in stack slots that are 1 byte in
  // size and 1-byte aligned.
  CCIfType<[i16], CCAssignToStack<1, 1>>
]>;

def CSR_DCPU16 : CalleeSavedRegs<(add X, Y, Z, I, J)>;
