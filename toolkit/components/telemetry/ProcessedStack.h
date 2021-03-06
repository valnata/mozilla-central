/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ProcessedStack_h__
#define ProcessedStack_h__

#include <string>
#include <vector>

namespace mozilla {
namespace Telemetry {

// This class represents a stack trace and the modules referenced in that trace.
// It is designed to be easy to read and write to disk or network and doesn't
// include any logic on how to collect or read the information it stores.
class ProcessedStack
{
public:
  ProcessedStack();
  size_t GetStackSize() const;
  size_t GetNumModules() const;

  struct Frame
  {
    // The offset of this program counter in its module or an absolute pc.
    uintptr_t mOffset;
    // The index to pass to GetModule to get the module this program counter
    // was in.
    uint16_t mModIndex;
  };
  struct Module
  {
    // The file name, /foo/bar/libxul.so for example.
    std::string mName;

    // The address it was loaded to.
    // FIXME: remove this once chrome hang has switched to using offsets.
    uintptr_t mStart;

    // The size of this mapping. May or may not be the entire file.
    // FIXME: remove this. It was only used as a sanity check.
    size_t mMappingSize;
    // Windows specific fields. On other platforms they are 0/empty.
    int mPdbAge;
    std::string mPdbSignature;
    std::string mPdbName;

    bool operator==(const Module& other) const;
  };

  const Frame &GetFrame(unsigned aIndex) const;
  void AddFrame(const Frame& aFrame);
  const Module &GetModule(unsigned aIndex) const;
  void AddModule(const Module& aFrame);

  void Clear();

  // FIXME: remove these once chrome hang has switched to using offsets.
  bool HasModule(const Module &aModule) const;
  void RemoveModule(unsigned aIndex);

private:
  std::vector<Module> mModules;
  std::vector<Frame> mStack;
};

// Get the current list of loaded modules, filter and pair it to the provided
// stack. We let the caller collect the stack since different callers have
// different needs (current thread X main thread, stopping the thread, etc).
// FIXME: remove the aRelative option once chrome hang has switched to using
// offsets.
ProcessedStack
GetStackAndModules(const std::vector<uintptr_t> &aPCs, bool aRelative);

} // namespace Telemetry
} // namespace mozilla
#endif // ProcessedStack_h__
