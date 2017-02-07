//===-- DCPU16TargetMachine.cpp - Define TargetMachine for DCPU16 -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about DCPU16 target spec.
//
//===----------------------------------------------------------------------===//

#include "DCPU16TargetMachine.h"
#include "DCPU16.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "dcpu16"

extern "C" void LLVMInitializeDCPU16Target() {
  // Register the target.
}
