//===-- DCPU16MCTargetDesc.h - DCPU16 Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides DCPU16 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DCPU16_MCTARGETDESC_DCPU16MCTARGETDESC_H
#define LLVM_LIB_TARGET_DCPU16_MCTARGETDESC_DCPU16MCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class Triple;

Target &getTheDCPU16Target();

} // End llvm namespace

// Defines symbolic names for DCPU16 registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "DCPU16GenRegisterInfo.inc"

// Defines symbolic names for the DCPU16 instructions.
#define GET_INSTRINFO_ENUM
#include "DCPU16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "DCPU16GenSubtargetInfo.inc"

#endif
