//===-- DCPU16RegisterInfo.cpp - DCPU16 Register Information --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the DCPU16 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "DCPU16RegisterInfo.h"
#include "DCPU16.h"
#include "DCPU16MachineFunctionInfo.h"
#include "DCPU16TargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "dcpu16-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "DCPU16GenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
DCPU16RegisterInfo::DCPU16RegisterInfo()
  : DCPU16GenRegisterInfo(DCPU16::RA) {}

const MCPhysReg*
DCPU16RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const DCPU16FrameLowering *TFI = getFrameLowering(*MF);
  static const MCPhysReg CalleeSavedRegs[] = {
      DCPU16::RX, DCPU16::RY, DCPU16::RZ, DCPU16::RI, DCPU16::RJ, 0
  };
  static const MCPhysReg CalleeSavedRegsFP[] = {
    DCPU16::RX, DCPU16::RY, DCPU16::RZ, DCPU16::RI, 0
  };

  if (TFI->hasFP(*MF))
    return CalleeSavedRegsFP;
  else
    return CalleeSavedRegs;
}

const uint32_t *DCPU16RegisterInfo::getCallPreservedMask(const MachineFunction&, CallingConv::ID CallConv) const {
    switch (CallConv) {
        default:
            llvm_unreachable("Unsupported calling conv");
        case CallingConv::C:
        case CallingConv::Fast:
            return CSR_DCPU16_RegMask;
    }
}

BitVector DCPU16RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  const DCPU16FrameLowering *TFI = getFrameLowering(MF);

  // Mark 3 special registers as reserved.
  Reserved.set(DCPU16::REX);
  Reserved.set(DCPU16::RSP);

  // Mark frame pointer as reserved if needed.
  if (TFI->hasFP(MF)) {
    Reserved.set(DCPU16::RJ);
  }

  return Reserved;
}

const TargetRegisterClass *
DCPU16RegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &DCPU16::GR16RegClass;
}

void
DCPU16RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                        int SPAdj, unsigned FIOperandNum,
                                        RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const DCPU16FrameLowering *TFI = getFrameLowering(MF);
  DebugLoc dl = MI.getDebugLoc();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  unsigned BasePtr = (TFI->hasFP(MF) ? DCPU16::RJ : DCPU16::RSP);
  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // Skip the saved PC
  Offset += 2;

  if (!TFI->hasFP(MF))
    Offset += MF.getFrameInfo().getStackSize();
  else
    Offset += 2; // Skip the saved FP

  // Fold imm into offset
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (MI.getOpcode() == DCPU16::ADD16ri) {
    // This is actually "load effective address" of the stack slot
    // instruction. We have only two-address instructions, thus we need to
    // expand it into mov + add
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

    MI.setDesc(TII.get(DCPU16::MOV16rr));
    MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);

    if (Offset == 0)
      return;

    // We need to materialize the offset via add instruction.
    unsigned DstReg = MI.getOperand(0).getReg();
    if (Offset < 0)
      BuildMI(MBB, std::next(II), dl, TII.get(DCPU16::SUB16ri), DstReg)
        .addReg(DstReg).addImm(-Offset);
    else
      BuildMI(MBB, std::next(II), dl, TII.get(DCPU16::ADD16ri), DstReg)
        .addReg(DstReg).addImm(Offset);

    return;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

unsigned DCPU16RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const DCPU16FrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? DCPU16::RJ : DCPU16::RSP;
}
