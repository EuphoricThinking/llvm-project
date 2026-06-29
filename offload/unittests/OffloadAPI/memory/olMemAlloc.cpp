//===------- Offload API tests - olMemAlloc -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../common/Properties.hpp"
#include <OffloadAPI.h>
#include <gtest/gtest.h>

using olMemAllocTest = OffloadDeviceTest;
OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olMemAllocTest);

using olMemAllocAllocTypesTest = OffloadDeviceTestWithParam<ol_alloc_type_t>;
OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE_WITH_PARAM(olMemAllocAllocTypesTest, testing::ValuesIn(AllocTypes), defaultPrinterWithParam<ol_alloc_type_t>); //printerMine);

TEST_P(olMemAllocAllocTypesTest, Success) {
  void *Alloc = nullptr;
  ASSERT_SUCCESS(olMemAlloc(Device, getTestParam(), 1024, &Alloc));
  ASSERT_NE(Alloc, nullptr);
  olMemFree(Alloc);
}

TEST_P(olMemAllocTest, SuccessAllocMany) {
  std::vector<void *> Allocs;
  Allocs.reserve(1000);

  for (size_t I = 1; I < 1000; I++) {
    void *Alloc = nullptr;
    ASSERT_SUCCESS(olMemAlloc(Device, AllocTypes[I % 3], 1024 * I, &Alloc));
    ASSERT_NE(Alloc, nullptr);

    Allocs.push_back(Alloc);
  }

  for (auto *A : Allocs) {
    olMemFree(A);
  }
}

TEST_P(olMemAllocTest, InvalidNullDevice) {
  void *Alloc = nullptr;
  ASSERT_ERROR(OL_ERRC_INVALID_NULL_HANDLE,
               olMemAlloc(nullptr, OL_ALLOC_TYPE_DEVICE, 1024, &Alloc));
}

TEST_P(olMemAllocTest, InvalidNullOutPtr) {
  ASSERT_ERROR(OL_ERRC_INVALID_NULL_POINTER,
               olMemAlloc(Device, OL_ALLOC_TYPE_DEVICE, 1024, nullptr));
}
