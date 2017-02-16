//= DCPU16InstPrinter.h - Convert DCPU16 MCInst to assembly syntax -*- C++ -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints a DCPU16 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DCPU16_INSTPRINTER_DCPU16INSTPRINTER_H
#define LLVM_LIB_TARGET_DCPU16_INSTPRINTER_DCPU16INSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {
  class DCPU16InstPrinter : public MCInstPrinter {
  public:
    DCPU16InstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                      const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

    void printInst(const MCInst *MI, raw_ostream &O, StringRef Annot,
                   const MCSubtargetInfo &STI) override;

    // Autogenerated by tblgen.
    void printInstruction(const MCInst *MI, raw_ostream &O);
    static const char *getRegisterName(unsigned RegNo);

    void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O,
                      const char *Modifier = nullptr);
    void printPCRelImmOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
    void printSrcMemOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O,
                            const char *Modifier = nullptr);
    void printCCOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);

  };
}

#endif
