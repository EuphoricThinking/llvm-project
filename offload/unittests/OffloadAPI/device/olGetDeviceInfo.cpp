//===------- Offload API tests - olGetDeviceInfo --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// #include "../common/Fixtures.hpp"
#include "../common/Device.hpp"
#include <OffloadAPI.h>
#include <gtest/gtest.h>

using olGetDeviceInfoTest = OffloadDeviceTest;
OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olGetDeviceInfoTest);

PropertyTuples JustSupportedProperties = mergeProperties(
    {BoolProperties,
     {propertiesTypes.at(OL_DEVICE_INFO_HALF_FP_CONFIG),
      propertiesTypes.at(OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_HALF)}});

PropertyTuples relevantGTCapabilitiesProperties =
    copyRelevantProperties(CapabilitesFlagsProperties,
                           {OL_DEVICE_INFO_HALF_FP_CONFIG}, propertiesTypes);

PropertyTuples relevantGTUint32Properties = copyRelevantProperties(
    Uint32Properties, {OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_HALF},
    propertiesTypes);

PropertyTuples NonZeroProperties =
    mergeProperties({relevantGTCapabilitiesProperties,
                     relevantGTUint32Properties, Uint64Properties});

PropertiesSet HostNotMeaningfulGT{
    OL_DEVICE_INFO_VENDOR_ID, OL_DEVICE_INFO_MAX_MEM_ALLOC_SIZE,
    OL_DEVICE_INFO_GLOBAL_MEM_SIZE, OL_DEVICE_INFO_WORK_GROUP_LOCAL_MEM_SIZE};

using olGetHostDeviceInfoPropertySupportTest = olGetHostDeviceInfoPropertyTest;
using olGetHostDeviceInfoPropertyNonZeroTest = olGetHostDeviceInfoPropertyTest;

OFFLOAD_TESTS_INSTANTIATE_HOST_DEVICE_FIXTURE_WITH_PARAM(
    olGetHostDeviceInfoPropertySupportTest,
    testing::ValuesIn(JustSupportedProperties),
    olGetHostDeviceInfoPropertyTestPrinter);

OFFLOAD_TESTS_INSTANTIATE_HOST_DEVICE_FIXTURE_WITH_PARAM(
    olGetHostDeviceInfoPropertyNonZeroTest,
    testing::ValuesIn(NonZeroProperties),
    olGetHostDeviceInfoPropertyTestPrinter);

// those without gt
TEST_P(olGetHostDeviceInfoPropertySupportTest, Success) {
  // Choosing the largest type since current possible types are {bool, uint32_t,
  // uint64_t}
  // uint64_t Value = 0;
  char Value[MAX_DEVICE_INFO_BYTES];
  ASSERT_SUCCESS(olGetDeviceInfo(Device, Property, PropertySize, &Value));

  // std::cout << this->Device << " " << Host << std::endl;
}

TEST_P(olGetHostDeviceInfoPropertyNonZeroTest, Value) {
  // uint64_t Value = 0;
  char Value[MAX_DEVICE_INFO_BYTES] = {};
  ASSERT_SUCCESS(olGetDeviceInfo(Device, Property, PropertySize, &Value));

  if (!isHost() || isMeaningfulForHost(Property, HostNotMeaningfulGT)) {
    // ASSERT_GT(*reinterpret_cast<uint64_t*>(Value), 0ul);
    ASSERT_TRUE(defaultCheckIsNonZero(Value));
  }
}

TEST_P(olGetDeviceInfoTest, SuccessType) {
  ol_device_type_t DeviceType;
  ASSERT_SUCCESS(olGetDeviceInfo(Device, OL_DEVICE_INFO_TYPE,
                                 sizeof(ol_device_type_t), &DeviceType));
}

TEST_P(olGetDeviceInfoTest, HostSuccessType) {
  ol_device_type_t DeviceType;
  ASSERT_SUCCESS(olGetDeviceInfo(Host, OL_DEVICE_INFO_TYPE,
                                 sizeof(ol_device_type_t), &DeviceType));
  ASSERT_EQ(DeviceType, OL_DEVICE_TYPE_HOST);
}

TEST_P(olGetDeviceInfoTest, SuccessPlatform) {
  ol_platform_handle_t Platform = nullptr;
  ASSERT_SUCCESS(olGetDeviceInfo(Device, OL_DEVICE_INFO_PLATFORM,
                                 sizeof(ol_platform_handle_t), &Platform));
  ASSERT_NE(Platform, nullptr);
}

TEST_P(olGetDeviceInfoTest, SuccessName) {
  size_t Size = 0;
  ASSERT_SUCCESS(olGetDeviceInfoSize(Device, OL_DEVICE_INFO_NAME, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> Name;
  Name.resize(Size);
  ASSERT_SUCCESS(
      olGetDeviceInfo(Device, OL_DEVICE_INFO_NAME, Size, Name.data()));
  ASSERT_EQ(std::strlen(Name.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, HostName) {
  size_t Size = 0;
  ASSERT_SUCCESS(olGetDeviceInfoSize(Host, OL_DEVICE_INFO_NAME, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> Name;
  Name.resize(Size);
  ASSERT_SUCCESS(olGetDeviceInfo(Host, OL_DEVICE_INFO_NAME, Size, Name.data()));
  ASSERT_EQ(std::strlen(Name.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, SuccessProductName) {
  size_t Size = 0;
  ASSERT_SUCCESS(
      olGetDeviceInfoSize(Device, OL_DEVICE_INFO_PRODUCT_NAME, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> Name;
  Name.resize(Size);
  ASSERT_SUCCESS(
      olGetDeviceInfo(Device, OL_DEVICE_INFO_PRODUCT_NAME, Size, Name.data()));
  ASSERT_EQ(std::strlen(Name.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, SuccessUID) {
  size_t Size = 0;
  ASSERT_SUCCESS(olGetDeviceInfoSize(Device, OL_DEVICE_INFO_UID, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> UID;
  UID.resize(Size);
  ASSERT_SUCCESS(olGetDeviceInfo(Device, OL_DEVICE_INFO_UID, Size, UID.data()));
  ASSERT_EQ(std::strlen(UID.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, HostProductName) {
  size_t Size = 0;
  ASSERT_SUCCESS(olGetDeviceInfoSize(Host, OL_DEVICE_INFO_PRODUCT_NAME, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> Name;
  Name.resize(Size);
  ASSERT_SUCCESS(
      olGetDeviceInfo(Host, OL_DEVICE_INFO_PRODUCT_NAME, Size, Name.data()));
  ASSERT_EQ(std::strlen(Name.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, HostUID) {
  size_t Size = 0;
  ASSERT_SUCCESS(olGetDeviceInfoSize(Host, OL_DEVICE_INFO_UID, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> UID;
  UID.resize(Size);
  ASSERT_SUCCESS(olGetDeviceInfo(Host, OL_DEVICE_INFO_UID, Size, UID.data()));
  ASSERT_EQ(std::strlen(UID.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, SuccessVendor) {
  size_t Size = 0;
  ASSERT_SUCCESS(olGetDeviceInfoSize(Device, OL_DEVICE_INFO_VENDOR, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> Vendor;
  Vendor.resize(Size);
  ASSERT_SUCCESS(
      olGetDeviceInfo(Device, OL_DEVICE_INFO_VENDOR, Size, Vendor.data()));
  ASSERT_EQ(std::strlen(Vendor.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, SuccessDriverVersion) {
  size_t Size = 0;
  ASSERT_SUCCESS(
      olGetDeviceInfoSize(Device, OL_DEVICE_INFO_DRIVER_VERSION, &Size));
  ASSERT_GT(Size, 0ul);
  std::vector<char> DriverVersion;
  DriverVersion.resize(Size);
  ASSERT_SUCCESS(olGetDeviceInfo(Device, OL_DEVICE_INFO_DRIVER_VERSION, Size,
                                 DriverVersion.data()));
  ASSERT_EQ(std::strlen(DriverVersion.data()), Size - 1);
}

TEST_P(olGetDeviceInfoTest, SuccessMaxWorkGroupSizePerDimension) {
  ol_dimensions_t Value{0, 0, 0};
  ASSERT_SUCCESS(
      olGetDeviceInfo(Device, OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE_PER_DIMENSION,
                      sizeof(Value), &Value));
  ASSERT_GT(Value.x, 0u);
  ASSERT_GT(Value.y, 0u);
  ASSERT_GT(Value.z, 0u);
}

TEST_P(olGetDeviceInfoTest, SuccessMaxWorkSizePerDimension) {
  ol_dimensions_t Value{0, 0, 0};
  ASSERT_SUCCESS(olGetDeviceInfo(Device,
                                 OL_DEVICE_INFO_MAX_WORK_SIZE_PER_DIMENSION,
                                 sizeof(Value), &Value));
  ASSERT_GT(Value.x, 0u);
  ASSERT_GT(Value.y, 0u);
  ASSERT_GT(Value.z, 0u);
}

TEST_P(olGetDeviceInfoTest, InvalidNullHandleDevice) {
  ol_device_type_t DeviceType;
  ASSERT_ERROR(OL_ERRC_INVALID_NULL_HANDLE,
               olGetDeviceInfo(nullptr, OL_DEVICE_INFO_TYPE,
                               sizeof(ol_device_type_t), &DeviceType));
}

TEST_P(olGetDeviceInfoTest, InvalidEnumerationInfoType) {
  ol_device_type_t DeviceType;
  ASSERT_ERROR(OL_ERRC_INVALID_ENUMERATION,
               olGetDeviceInfo(Device, OL_DEVICE_INFO_FORCE_UINT32,
                               sizeof(ol_device_type_t), &DeviceType));
}

TEST_P(olGetDeviceInfoTest, InvalidSizePropSize) {
  ol_device_type_t DeviceType;
  ASSERT_ERROR(OL_ERRC_INVALID_SIZE,
               olGetDeviceInfo(Device, OL_DEVICE_INFO_TYPE, 0, &DeviceType));
}

TEST_P(olGetDeviceInfoTest, InvalidSizePropSizeSmall) {
  ol_device_type_t DeviceType;
  ASSERT_ERROR(OL_ERRC_INVALID_SIZE,
               olGetDeviceInfo(Device, OL_DEVICE_INFO_TYPE,
                               sizeof(DeviceType) - 1, &DeviceType));
}

TEST_P(olGetDeviceInfoTest, InvalidNullPointerPropValue) {
  ol_device_type_t DeviceType;
  ASSERT_ERROR(OL_ERRC_INVALID_NULL_POINTER,
               olGetDeviceInfo(Device, OL_DEVICE_INFO_TYPE, sizeof(DeviceType),
                               nullptr));
}
