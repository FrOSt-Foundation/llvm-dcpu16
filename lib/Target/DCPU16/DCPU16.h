//===-- DCPU16.h - Top-level interface for DCPU16 representation *- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM DCPU16 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DCPU16_DCPU16_H
#define LLVM_LIB_TARGET_DCPU16_DCPU16_H

#include "MCTargetDesc/DCPU16MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class DCPU16TargetMachine;
  class FunctionPass;

} // end namespace llvm;

#endif
