//===-- DCPU16MCAsmInfo.h - DCPU16 asm properties --------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the DCPU16MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DCPU16_MCTARGETDESC_DCPU16MCASMINFO_H
#define LLVM_LIB_TARGET_DCPU16_MCTARGETDESC_DCPU16MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class DCPU16MCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit DCPU16MCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
