//===-- DCPU16RegisterInfo.h - DCPU16 Register Information Impl -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the DCPU16 implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DCPU16_DCPU16REGISTERINFO_H
#define LLVM_LIB_TARGET_DCPU16_DCPU16REGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "DCPU16GenRegisterInfo.inc"

namespace llvm {

struct DCPU16RegisterInfo : public DCPU16GenRegisterInfo {
public:
  DCPU16RegisterInfo();

  /// Code Generation virtual methods...
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
  const uint32_t  *getCallPreservedMask(const MachineFunction&, CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;
  const TargetRegisterClass*
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Debug information queries.
  unsigned getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
