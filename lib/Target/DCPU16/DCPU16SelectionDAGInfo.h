//===-- DCPU16SelectionDAGInfo.h - DCPU16 SelectionDAG Info -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the DCPU16 subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef DCPU16SELECTIONDAGINFO_H
#define DCPU16SELECTIONDAGINFO_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

namespace llvm {

class DCPU16TargetMachine;

class DCPU16SelectionDAGInfo : public SelectionDAGTargetInfo {
public:
  explicit DCPU16SelectionDAGInfo() = default;
  ~DCPU16SelectionDAGInfo() = default;
};

}

#endif
