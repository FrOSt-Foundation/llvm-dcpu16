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
#include "DCPU16Subtarget.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "dcpu16"

#define DATALAYOUT "e-p:16:16:16-i8:16:16-i16:16:16-i32:16:16-s0:16:16-n16"

namespace llvm {
Target TheDCPU16Target;
}

extern "C" void LLVMInitializeDCPU16Target() {
  // Register the target.
  RegisterTargetMachine<DCPU16TargetMachine> X(TheDCPU16Target);
}

DCPU16TargetMachine::DCPU16TargetMachine(const Target &T, const Triple &TT,
        StringRef CPU, StringRef FS, const TargetOptions &Options,
        Optional<Reloc::Model> &RM, CodeModel::Model CM, CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, DATALAYOUT, TT, CPU, FS, Options, *RM, CM, OL),
        dataLayout(DATALAYOUT), InstrInfo(*this), TLInfo(*this), Subtarget(TT, CPU, FS), FrameLowering(Subtarget) {
    initAsmInfo();
}

DCPU16TargetMachine::~DCPU16TargetMachine() {}

namespace {
// DCPU16 code generator pass configuration options
class DCPU16PassConfig : public TargetPassConfig {
    public:
        DCPU16PassConfig(DCPU16TargetMachine *TM, PassManagerBase &PM)
            : TargetPassConfig(TM, PM) {}

        DCPU16TargetMachine &getDCPU16TargetMachine() const { return getTM<DCPU16TargetMachine>(); }
};

}

TargetPassConfig *DCPU16TargetMachine::createPassConfig(PassManagerBase &PM) {
    return new DCPU16PassConfig(this, PM);
}
