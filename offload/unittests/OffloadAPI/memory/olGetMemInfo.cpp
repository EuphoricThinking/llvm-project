//===------- Offload API tests - olGetMemInfo -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../common/Properties.hpp"
#include <OffloadAPI.h>
#include <gtest/gtest.h>

constexpr size_t SIZE = 1024;

// struct olGetMemInfoBaseTest : OffloadDeviceTest {
//   void *OffsetPtr() { return &reinterpret_cast<char *>(Ptr)[123]; }

//   void *Ptr;
// };

// template <ol_alloc_type_t AllocType>
struct olGetMemInfoAllocTypeTest : OffloadDeviceTestWithParam<ol_alloc_type_t> {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTestWithParam<ol_alloc_type_t>::SetUp());
    ASSERT_SUCCESS(olMemAlloc(Device, getTestParam(), SIZE, &Ptr));
  }

  void TearDown() override {
    ASSERT_SUCCESS(olMemFree(Ptr));
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTestWithParam<ol_alloc_type_t>::TearDown());
  }

  // void *OffsetPtr() { return &reinterpret_cast<char *>(Ptr)[123]; }

  void *Ptr;
};

struct olGetMemInfoTest : OffloadDeviceTest {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTest::SetUp());
    ASSERT_SUCCESS(olMemAlloc(Device, OL_ALLOC_TYPE_DEVICE, SIZE, &Ptr));
  }

  void TearDown() override {
    ASSERT_SUCCESS(olMemFree(Ptr));
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTest::TearDown());
  }

  // void *OffsetPtr() { return &reinterpret_cast<char *>(Ptr)[123]; }

  void *Ptr;
};

OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE_WITH_PARAM(olGetMemInfoAllocTypeTest, testing::ValuesIn(AllocTypes), defaultPrinterWithParam<ol_alloc_type_t>);
// using olGetMemInfoDeviceTest = olGetMemInfoTest<OL_ALLOC_TYPE_DEVICE>;
OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olGetMemInfoTest);
// using olGetMemInfoManagedTest = olGetMemInfoTest<OL_ALLOC_TYPE_MANAGED>;
// OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olGetMemInfoManagedTest);
// using olGetMemInfoHostTest = olGetMemInfoTest<OL_ALLOC_TYPE_HOST>;
// OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olGetMemInfoHostTest);

// #define PER_ALLOC_TEST(FUNCTION)                                               \
//   TEST_P(olGetMemInfoDeviceTest, FUNCTION) {                                   \
//     FUNCTION(this, Ptr, OL_ALLOC_TYPE_DEVICE);                                 \
//   }                                                                            \
//   TEST_P(olGetMemInfoManagedTest, FUNCTION) {                                  \
//     FUNCTION(this, Ptr, OL_ALLOC_TYPE_MANAGED);                                \
//   }                                                                            \
//   TEST_P(olGetMemInfoHostTest, FUNCTION) {                                     \
//     FUNCTION(this, OffsetPtr(), OL_ALLOC_TYPE_HOST);                           \
//   }                                                                            \
//   TEST_P(olGetMemInfoDeviceTest, FUNCTION##Offset) {                           \
//     FUNCTION(this, Ptr, OL_ALLOC_TYPE_DEVICE);                                 \
//   }                                                                            \
//   TEST_P(olGetMemInfoManagedTest, FUNCTION##Offset) {                          \
//     FUNCTION(this, OffsetPtr(), OL_ALLOC_TYPE_MANAGED);                        \
//   }                                                                            \
//   TEST_P(olGetMemInfoHostTest, FUNCTION##Offset) {                             \
//     FUNCTION(this, OffsetPtr(), OL_ALLOC_TYPE_HOST);                           \
//   }

// void SuccessDevice(olGetMemInfoBaseTest *Fixture, void *Ptr,
//                    ol_alloc_type_t Type) {
//   ol_device_handle_t RetrievedDevice;
TEST_P(olGetMemInfoAllocTypeTest, SuccessDevice) {
  ol_device_handle_t RetrievedDevice;
  ASSERT_SUCCESS(olGetMemInfo(Ptr, OL_MEM_INFO_DEVICE,
                              sizeof(RetrievedDevice), &RetrievedDevice));
  ASSERT_EQ(RetrievedDevice, Device);
}
// PER_ALLOC_TEST(SuccessDevice);

// void SuccessBase(olGetMemInfoBaseTest *Fixture, void *Ptr,
//                  ol_alloc_type_t Type) {
TEST_P(olGetMemInfoAllocTypeTest, SuccessBase) {
  void *RetrievedBase;
  ASSERT_SUCCESS(olGetMemInfo(Ptr, OL_MEM_INFO_BASE,
                              sizeof(RetrievedBase), &RetrievedBase));
  ASSERT_EQ(RetrievedBase, Ptr);
}
// PER_ALLOC_TEST(SuccessBase);

// void SuccessSize(olGetMemInfoBaseTest *Fixture, void *Ptr,
//                  ol_alloc_type_t Type) {
TEST_P(olGetMemInfoAllocTypeTest, SuccessSize) {
  size_t RetrievedSize;
  ASSERT_SUCCESS(olGetMemInfo(Ptr, OL_MEM_INFO_SIZE,
                              sizeof(RetrievedSize), &RetrievedSize));
  ASSERT_EQ(RetrievedSize, SIZE);
}
// PER_ALLOC_TEST(SuccessSize);

// void SuccessType(olGetMemInfoBaseTest *Fixture, void *Ptr,
//                  ol_alloc_type_t Type) {
TEST_P(olGetMemInfoAllocTypeTest, SuccessType) {
  ol_alloc_type_t RetrievedType;
  ASSERT_SUCCESS(olGetMemInfo(Ptr, OL_MEM_INFO_TYPE,
                              sizeof(RetrievedType), &RetrievedType));
  ASSERT_EQ(RetrievedType, getTestParam());
}
// PER_ALLOC_TEST(SuccessType);

TEST_P(olGetMemInfoTest, InvalidNotFound) {
  // Assuming that we aren't unlucky and happen to get 0x1234 as a random
  // pointer
  void *RetrievedBase;
  ASSERT_ERROR(OL_ERRC_NOT_FOUND,
               olGetMemInfo(reinterpret_cast<void *>(0x1234), OL_MEM_INFO_BASE,
                            sizeof(RetrievedBase), &RetrievedBase));
}

TEST_P(olGetMemInfoTest, InvalidNullPtr) {
  ol_device_handle_t RetrievedDevice;
  ASSERT_ERROR(OL_ERRC_INVALID_NULL_POINTER,
               olGetMemInfo(nullptr, OL_MEM_INFO_DEVICE,
                            sizeof(RetrievedDevice), &RetrievedDevice));
}

TEST_P(olGetMemInfoTest, InvalidSizeZero) {
  ol_device_handle_t RetrievedDevice;
  ASSERT_ERROR(OL_ERRC_INVALID_SIZE,
               olGetMemInfo(Ptr, OL_MEM_INFO_DEVICE, 0, &RetrievedDevice));
}

TEST_P(olGetMemInfoTest, InvalidSizeSmall) {
  ol_device_handle_t RetrievedDevice;
  ASSERT_ERROR(OL_ERRC_INVALID_SIZE,
               olGetMemInfo(Ptr, OL_MEM_INFO_DEVICE,
                            sizeof(RetrievedDevice) - 1, &RetrievedDevice));
}

TEST_P(olGetMemInfoTest, InvalidNullPointerPropValue) {
  ol_device_handle_t RetrievedDevice;
  ASSERT_ERROR(
      OL_ERRC_INVALID_NULL_POINTER,
      olGetMemInfo(Ptr, OL_MEM_INFO_DEVICE, sizeof(RetrievedDevice), nullptr));
}
