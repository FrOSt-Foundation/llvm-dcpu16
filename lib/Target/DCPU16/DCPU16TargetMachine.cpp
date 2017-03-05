//===-- DCPU16TargetMachine.cpp - Define TargetMachine for DCPU16 ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the DCPU16 target.
//
//===----------------------------------------------------------------------===//

#include "DCPU16TargetMachine.h"
#include "DCPU16.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeDCPU16Target() {
  // Register the target.
  RegisterTargetMachine<DCPU16TargetMachine> X(getTheDCPU16Target());
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

DCPU16TargetMachine::DCPU16TargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         CodeModel::Model CM,
                                         CodeGenOpt::Level OL)
    // datalayout string (separated with -): 
    // B16 -> 16 bits per bytes
    // e -> little endian
    // m:e -> elf name mangling (private symbols get .L prefix)
    // p:16:16:16 -> size ptr 16bits, 16bits abi, 16bits pref alignment
    // i16:16:16 -> integers of size 16 alignment (16 abi, 16 pref alignment)
    // a:16:16 -> aggregate type abi & pref alignment
    : LLVMTargetMachine(T, "B16-e-m:e-p:16:16:16-i8:16:16-i16:16:16-i32:16:16-s0:16:16-a:16:16-n16", TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM), CM, OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

DCPU16TargetMachine::~DCPU16TargetMachine() {}

namespace {
/// DCPU16 Code Generator Pass Configuration Options.
class DCPU16PassConfig : public TargetPassConfig {
public:
  DCPU16PassConfig(DCPU16TargetMachine *TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  DCPU16TargetMachine &getDCPU16TargetMachine() const {
    return getTM<DCPU16TargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *DCPU16TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new DCPU16PassConfig(this, PM);
}

bool DCPU16PassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createDCPU16ISelDag(getDCPU16TargetMachine(), getOptLevel()));
  return false;
}
