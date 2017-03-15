//===-- DCPU16MCTargetDesc.cpp - DCPU16 Target Descriptions ---------------===//
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

#include "DCPU16MCTargetDesc.h"
#include "InstPrinter/DCPU16InstPrinter.h"
#include "DCPU16MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "DCPU16GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "DCPU16GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "DCPU16GenRegisterInfo.inc"

static MCInstrInfo *createDCPU16MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitDCPU16MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createDCPU16MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitDCPU16MCRegisterInfo(X, DCPU16::A);
  return X;
}

static MCSubtargetInfo *
createDCPU16MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createDCPU16MCSubtargetInfoImpl(TT, CPU, FS);
}

static MCInstPrinter *createDCPU16MCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new DCPU16InstPrinter(MAI, MII, MRI);
  return nullptr;
}

extern "C" void LLVMInitializeDCPU16TargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<DCPU16MCAsmInfo> X(getTheDCPU16Target());

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getTheDCPU16Target(),
                                      createDCPU16MCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getTheDCPU16Target(),
                                    createDCPU16MCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getTheDCPU16Target(),
                                          createDCPU16MCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheDCPU16Target(),
                                        createDCPU16MCInstPrinter);
}
