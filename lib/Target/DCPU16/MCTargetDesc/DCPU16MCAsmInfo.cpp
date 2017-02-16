//===-- DCPU16MCAsmInfo.cpp - DCPU16 asm properties -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the DCPU16MCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "DCPU16MCAsmInfo.h"
using namespace llvm;

void DCPU16MCAsmInfo::anchor() { }

DCPU16MCAsmInfo::DCPU16MCAsmInfo(const Triple &TT) {
  PointerSize = CalleeSaveStackSlotSize = 2;

  LabelPrefix = ":";
  LabelSuffix = "";

  CommentString = ";";

  AlignmentIsInBytes = false;
  UsesELFSectionDirectiveForBSS = true;
  HasDotTypeDotSizeDirective = false;
}
