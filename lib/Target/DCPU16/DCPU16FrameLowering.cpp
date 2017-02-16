//===-- DCPU16FrameLowering.cpp - DCPU16 Frame Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the DCPU16 implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "DCPU16FrameLowering.h"
#include "DCPU16InstrInfo.h"
#include "DCPU16MachineFunctionInfo.h"
#include "DCPU16Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

bool DCPU16FrameLowering::hasFP(const MachineFunction &MF) const {
    return false;
}

bool DCPU16FrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo().hasVarSizedObjects();
}

void DCPU16FrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo &MFI = MF.getFrameInfo();
  DCPU16MachineFunctionInfo *DCPU16FI = MF.getInfo<DCPU16MachineFunctionInfo>();
  const DCPU16InstrInfo &TII =
      *static_cast<const DCPU16InstrInfo *>(MF.getSubtarget().getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // Get the number of bytes to allocate from the FrameInfo.
  uint64_t StackSize = MFI.getStackSize();

  uint64_t NumBytes = 0;

  NumBytes = StackSize - DCPU16FI->getCalleeSavedFrameSize();

  // Skip the callee-saved push instructions.
  while (MBBI != MBB.end() && (MBBI->getOpcode() == DCPU16::PUSH16r))
    ++MBBI;

  if (MBBI != MBB.end())
    DL = MBBI->getDebugLoc();

  if (NumBytes) { // adjust stack pointer: RC -= numbytes
    // If there is an SUB16ri of RC immediately before this instruction, merge
    // the two.
    //NumBytes -= mergeRCUpdates(MBB, MBBI, true);
    // If there is an ADD16ri or SUB16ri of RC immediately after this
    // instruction, merge the two instructions.
    // mergeRCUpdatesDown(MBB, MBBI, &NumBytes);

    if (NumBytes) {
      MachineInstr *MI =
        BuildMI(MBB, MBBI, DL, TII.get(DCPU16::SUB16ri), DCPU16::RC)
        .addReg(DCPU16::RC).addImm(NumBytes);
      // The SRW implicit def is dead.
      MI->getOperand(3).setIsDead();
    }
  }
}

void DCPU16FrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  DCPU16MachineFunctionInfo *DCPU16FI = MF.getInfo<DCPU16MachineFunctionInfo>();
  const DCPU16InstrInfo &TII =
      *static_cast<const DCPU16InstrInfo *>(MF.getSubtarget().getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  unsigned RetOpcode = MBBI->getOpcode();
  DebugLoc DL = MBBI->getDebugLoc();

  switch (RetOpcode) {
  case DCPU16::RET:
  case DCPU16::RETI: break;  // These are ok
  default:
    llvm_unreachable("Can only insert epilog into returning blocks");
  }

  // Get the number of bytes to allocate from the FrameInfo
  uint64_t StackSize = MFI.getStackSize();
  unsigned CSSize = DCPU16FI->getCalleeSavedFrameSize();
  uint64_t NumBytes = 0;

  NumBytes = StackSize - CSSize;

  // Skip the callee-saved pop instructions.
  while (MBBI != MBB.begin()) {
    MachineBasicBlock::iterator PI = std::prev(MBBI);
    unsigned Opc = PI->getOpcode();
    if (Opc != DCPU16::POP16r && !PI->isTerminator())
      break;
    --MBBI;
  }

  DL = MBBI->getDebugLoc();

  // If there is an ADD16ri or SUB16ri of RC immediately before this
  // instruction, merge the two instructions.
  //if (NumBytes || MFI.hasVarSizedObjects())
  //  mergeRCUpdatesUp(MBB, MBBI, StackPtr, &NumBytes);

  // adjust stack pointer back: RC += numbytes
  if (NumBytes) {
  MachineInstr *MI =
    BuildMI(MBB, MBBI, DL, TII.get(DCPU16::ADD16ri), DCPU16::RC)
            .addReg(DCPU16::RC).addImm(NumBytes);
    // The SRW implicit def is dead.
    MI->getOperand(3).setIsDead();
  }
}

// FIXME: Can we eleminate these in favour of generic code?
bool
DCPU16FrameLowering::spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DCPU16MachineFunctionInfo *MFI = MF.getInfo<DCPU16MachineFunctionInfo>();
  MFI->setCalleeSavedFrameSize(CSI.size() * 2);

  for (unsigned i = CSI.size(); i != 0; --i) {
    unsigned Reg = CSI[i-1].getReg();
    // Add the callee-saved register as live-in. It's killed at the spill.
    MBB.addLiveIn(Reg);
    BuildMI(MBB, MI, DL, TII.get(DCPU16::PUSH16r))
      .addReg(Reg, RegState::Kill);
  }
  return true;
}

bool
DCPU16FrameLowering::restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                                 MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  for (unsigned i = 0, e = CSI.size(); i != e; ++i)
    BuildMI(MBB, MI, DL, TII.get(DCPU16::POP16r), CSI[i].getReg());

  return true;
}

MachineBasicBlock::iterator DCPU16FrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  const DCPU16InstrInfo &TII =
      *static_cast<const DCPU16InstrInfo *>(MF.getSubtarget().getInstrInfo());
  unsigned StackAlign = getStackAlignment();

  if (!hasReservedCallFrame(MF)) {
    // If the stack pointer can be changed after prologue, turn the
    // adjcallstackup instruction into a 'sub RC, <amt>' and the
    // adjcallstackdown instruction into 'add RC, <amt>'
    // TODO: consider using push / pop instead of sub + store / add
    MachineInstr &Old = *I;
    uint64_t Amount = Old.getOperand(0).getImm();
    if (Amount != 0) {
      // We need to keep the stack aligned properly.  To do this, we round the
      // amount of space needed for the outgoing arguments up to the next
      // alignment boundary.
      Amount = (Amount+StackAlign-1)/StackAlign*StackAlign;

      MachineInstr *New = nullptr;
      if (Old.getOpcode() == TII.getCallFrameSetupOpcode()) {
        New =
            BuildMI(MF, Old.getDebugLoc(), TII.get(DCPU16::SUB16ri), DCPU16::RC)
                .addReg(DCPU16::RC)
                .addImm(Amount);
      } else {
        assert(Old.getOpcode() == TII.getCallFrameDestroyOpcode());
        // factor out the amount the callee already popped.
        uint64_t CalleeAmt = Old.getOperand(1).getImm();
        Amount -= CalleeAmt;
        if (Amount)
          New = BuildMI(MF, Old.getDebugLoc(), TII.get(DCPU16::ADD16ri),
                        DCPU16::RC)
                    .addReg(DCPU16::RC)
                    .addImm(Amount);
      }

      if (New) {
        // The SRW implicit def is dead.
        New->getOperand(3).setIsDead();

        // Replace the pseudo instruction with a new instruction...
        MBB.insert(I, New);
      }
    }
  } else if (I->getOpcode() == TII.getCallFrameDestroyOpcode()) {
    // If we are performing frame pointer elimination and if the callee pops
    // something off the stack pointer, add it back.
    if (uint64_t CalleeAmt = I->getOperand(1).getImm()) {
      MachineInstr &Old = *I;
      MachineInstr *New =
          BuildMI(MF, Old.getDebugLoc(), TII.get(DCPU16::SUB16ri), DCPU16::RC)
              .addReg(DCPU16::RC)
              .addImm(CalleeAmt);
      // The SRW implicit def is dead.
      New->getOperand(3).setIsDead();

      MBB.insert(I, New);
    }
  }

  return MBB.erase(I);
}

void
DCPU16FrameLowering::processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                                         RegScavenger *) const {
}
