//===-- DCPU16InstPrinter.cpp - Convert DCPU16 MCInst to assembly syntax --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an DCPU16 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "DCPU16InstPrinter.h"
#include "DCPU16.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"


// Include the auto-generated portion of the assembly writer.
#include "DCPU16GenAsmWriter.inc"

void DCPU16InstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                  StringRef Annot, const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void DCPU16InstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm())
    O << Op.getImm();
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void DCPU16InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                     raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
    O << Op.getImm();
  } else {
    assert(Op.isExpr() && "unknown operand kind in printOperand");
    Op.getExpr()->print(O, &MAI);
  }
}

void DCPU16InstPrinter::printSrcMemOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O,
                                           const char *Modifier) {
  const MCOperand &Base = MI->getOperand(OpNo);
  const MCOperand &Disp = MI->getOperand(OpNo+1);

  // Print displacement first

  // If the global address expression is a part of displacement field with a
  // register base, we should not emit any prefix symbol here, e.g.
  //   SET r1, &foo
  // vs
  //   SET r2, glb(r1)
  // Otherwise (!) dcpu16-as will silently miscompile the output :(
  if (!Base.getReg())
    O << '&';
  else
    O << '[';

  if (Disp.isExpr())
    Disp.getExpr()->print(O, &MAI);
  else {
    assert(Disp.isImm() && "Expected immediate in displacement field");
    O << Disp.getImm();
  }

  // Print register base field
  if (Base.getReg())
    O << '+' << getRegisterName(Base.getReg()) << ']';
}

void DCPU16InstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case DCPU16CC::COND_E:
   O << "IFE\tEX, 0";
   break;
  case DCPU16CC::COND_NE:
   O << "IFN\tEX, 0";
   break;
  case DCPU16CC::COND_HS:
   O << "IFN\tEX, 0xFFFF";
   break;
  case DCPU16CC::COND_LO:
   O << "IFE\tEX, 0xFFFF";
   break;
  case DCPU16CC::COND_GE:
   O << "IFN\tEX, 0xFFFF";
   break;
  case DCPU16CC::COND_L:
   O << "IFE\tEX, 0xFFFF";
   break;
  }
}
