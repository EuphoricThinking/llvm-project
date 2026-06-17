//===------- Offload API tests - olGetDeviceInfo --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../common/Fixtures.hpp"
#include <OffloadAPI.h>
#include <gtest/gtest.h>

using olGetDeviceInfoTest = OffloadDeviceTest;
OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE(olGetDeviceInfoTest);

using olGetDeviceInfoPropertyUint32Test = OffloadDeviceTestWithParam<uint32_t>;
// std::vector PropertiesUint32<ol_device_info_t>;
                            // OL_DEVICE_INFO_DOUBLE_FP_SUPPORT);
using PropertiesVec = std::vector<ol_device_info_t>;
using PropertyTuple = std::tuple<size_t, ol_device_info_t>;
using PropertyTuples = std::vector<PropertyTuple>;
using PropertiesSet = std::set<ol_device_info_t>;

template <typename Container>
PropertyTuples createPropertyTuples(size_t PropSize, Container SelectedProperties) {
  PropertyTuples Res;
  for (auto p: SelectedProperties) {
    Res.push_back({PropSize, p});
  }

  return Res;
}
// std::vector<PropertyTuple> PropBoolTuples = 

PropertiesSet PropBool{OL_DEVICE_INFO_SINGLE_FP_SUPPORT, OL_DEVICE_INFO_DOUBLE_FP_SUPPORT, OL_DEVICE_INFO_HALF_FP_SUPPORT};
PropertyTuples BoolProperties = createPropertyTuples(sizeof(bool), PropBool);

PropertiesSet PropUint32{OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE, OL_DEVICE_INFO_MAX_WORK_SIZE, OL_DEVICE_INFO_VENDOR_ID, OL_DEVICE_INFO_NUM_COMPUTE_UNITS, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_CHAR, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_SHORT, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_INT, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_LONG, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_FLOAT, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_DOUBLE, OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_HALF, OL_DEVICE_INFO_MAX_CLOCK_FREQUENCY, OL_DEVICE_INFO_MEMORY_CLOCK_RATE, OL_DEVICE_INFO_ADDRESS_BITS};
PropertyTuples Uint32Properties = createPropertyTuples(sizeof(uint32_t), PropUint32);

PropertiesSet PropUint64{OL_DEVICE_INFO_MAX_MEM_ALLOC_SIZE, OL_DEVICE_INFO_GLOBAL_MEM_SIZE, OL_DEVICE_INFO_WORK_GROUP_LOCAL_MEM_SIZE};
PropertyTuples Uint64Properties = createPropertyTuples(sizeof(uint64_t), PropUint64);

PropertiesSet PropCapabilitiesFlags{OL_DEVICE_INFO_SINGLE_FP_CONFIG, OL_DEVICE_INFO_HALF_FP_CONFIG, OL_DEVICE_INFO_DOUBLE_FP_CONFIG};
// sizeof(ol_device_fp_capability_flags_t) == sizegof(uint32_t) 
PropertyTuples CapabilitesFlagsProperties = createPropertyTuples(sizeof(ol_device_fp_capability_flags_t), PropCapabilitiesFlags);

// template <class T>
// inline 
std::string olGetHostDeviceInfoPropertyTestPrinter(const ::testing::TestParamInfo<
              OffloadParam<PropertyTuple>> &info) {
                auto device = std::get<0>(info.param);
                auto paramTuple = std::get<1>(info.param);

                std::string ss;
                llvm::raw_string_ostream finalName(ss);

                // auto host = TestEnvironment::getHostDevice();

                // if (device.Handle == host) {
                //   ss << "__" << "HOST" << "__";
                // }

                auto property = std::get<1>(paramTuple);


                
                finalName << device.Name << "__" << property;

                return SanitizeString(finalName.str());
              }

struct olGetHostDeviceInfoPropertyTest : OffloadDeviceTestWithParam<PropertyTuple> {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTestWithParam<PropertyTuple>::SetUp());

    // TODO check host and device
    auto paramTuple = this->getTestParam();
    PropertySize = std::get<0>(paramTuple);
    Property = std::get<1>(paramTuple);
  }

  // ol_device_info_t getProperty() {
  //   return std::get<1>(this->getTestParam());
  // }  

  size_t PropertySize = 0;
  ol_device_info_t Property;

  // // Choosing the largest type since current possible types are {bool, uint32_t, uint64_t}
  // uint64_t Value = 0;
};

OFFLOAD_TESTS_INSTANTIATE_HOST_DEVICE_FIXTURE_WITH_PARAM(olGetHostDeviceInfoPropertyTest, testing::ValuesIn(BoolProperties), olGetHostDeviceInfoPropertyTestPrinter);

TEST_P(olGetHostDeviceInfoPropertyTest, SuccessSupport) {
  uint64_t Value = 0;
  ASSERT_SUCCESS(olGetDeviceInfo(Device, Property, PropertySize, &Value)); 

  std::cout << this->Device << " " << Host << std::endl;
}


// using olGetDeviceInfoPropertyBoolTest = OffloadDeviceTestWithParam<ol_device_info_t>;

// // using olGetDeviceInfoPropertyUint64Test = OffloadDeviceTestWithParam<uint64_t>;
// OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE_WITH_PARAM(olGetDeviceInfoPropertyBoolTest, testing::ValuesIn(PropBool), defaultPrinterWithParam<ol_device_info_t>);

// TEST_P(olGetDeviceInfoPropertyBoolTest, Success) {
//   bool Dummy;
//   ASSERT_SUCCESS(olGetDeviceInfo(Device, getTestParam(), sizeof(Dummy), &Dummy));
// }




// TEST_P(olGetDeviceInfoPropertyUint32Test, SUCCESS)

// using TestParams = std::variant<uint32_t, bool>;

// using olGetDeviceInfoPropertyTest = OffloadDeviceTestWithParam<TestParams>;
// OFFLOAD_TESTS_INSTANTIATE_DEVICE_FIXTURE_WITH_PARAM(olGetDeviceInfoPropertyTest, ::testing::Values(TestParams{true}, TestParams{uint32_t{1}}));

#define OL_DEVICE_INFO_TEST_SUCCESS_CHECK(TestName, PropType, PropName, Dev,   \
                                          Expr)                                \
  TEST_P(olGetDeviceInfoTest, Test##Dev##TestName) {                           \
    PropType Value;                                                            \
    ASSERT_SUCCESS(olGetDeviceInfo(Dev, PropName, sizeof(Value), &Value));     \
    Expr;                                                                      \
  }

#define OL_DEVICE_INFO_TEST_DEVICE_SUCCESS(TestName, PropType, PropName)       \
  OL_DEVICE_INFO_TEST_SUCCESS_CHECK(TestName, PropType, PropName, Device, {})

#define OL_DEVICE_INFO_TEST_HOST_SUCCESS(TestName, PropType, PropName)         \
  OL_DEVICE_INFO_TEST_SUCCESS_CHECK(TestName, PropType, PropName, Host, {})

#define OL_DEVICE_INFO_TEST_SUCCESS(TestName, PropType, PropName)              \
  OL_DEVICE_INFO_TEST_DEVICE_SUCCESS(TestName, PropType, PropName)             \
  OL_DEVICE_INFO_TEST_HOST_SUCCESS(TestName, PropType, PropName)

#define OL_DEVICE_INFO_TEST_DEVICE_VALUE_GT(TestName, PropType, PropName,      \
                                            LowBound)                          \
  OL_DEVICE_INFO_TEST_SUCCESS_CHECK(TestName, PropType, PropName, Device,      \
                                    ASSERT_GT(Value, LowBound))

#define OL_DEVICE_INFO_TEST_HOST_VALUE_GT(TestName, PropType, PropName,        \
                                          LowBound)                            \
  OL_DEVICE_INFO_TEST_SUCCESS_CHECK(TestName, PropType, PropName, Host,        \
                                    ASSERT_GT(Value, LowBound))

#define OL_DEVICE_INFO_TEST_VALUE_GT(TestName, PropType, PropName, LowBound)   \
  OL_DEVICE_INFO_TEST_DEVICE_VALUE_GT(TestName, PropType, PropName, LowBound)  \
  OL_DEVICE_INFO_TEST_HOST_VALUE_GT(TestName, PropType, PropName, LowBound)

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

OL_DEVICE_INFO_TEST_VALUE_GT(MaxWorkGroupSize, uint32_t,
                             OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE, 0);

TEST_P(olGetDeviceInfoTest, SuccessMaxWorkGroupSizePerDimension) {
  ol_dimensions_t Value{0, 0, 0};
  ASSERT_SUCCESS(
      olGetDeviceInfo(Device, OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE_PER_DIMENSION,
                      sizeof(Value), &Value));
  ASSERT_GT(Value.x, 0u);
  ASSERT_GT(Value.y, 0u);
  ASSERT_GT(Value.z, 0u);
}

OL_DEVICE_INFO_TEST_VALUE_GT(MaxWorkSize, uint32_t,
                             OL_DEVICE_INFO_MAX_WORK_SIZE, 0);

TEST_P(olGetDeviceInfoTest, SuccessMaxWorkSizePerDimension) {
  ol_dimensions_t Value{0, 0, 0};
  ASSERT_SUCCESS(olGetDeviceInfo(Device,
                                 OL_DEVICE_INFO_MAX_WORK_SIZE_PER_DIMENSION,
                                 sizeof(Value), &Value));
  ASSERT_GT(Value.x, 0u);
  ASSERT_GT(Value.y, 0u);
  ASSERT_GT(Value.z, 0u);
}

OL_DEVICE_INFO_TEST_DEVICE_VALUE_GT(VendorId, uint32_t,
                                    OL_DEVICE_INFO_VENDOR_ID, 0);
OL_DEVICE_INFO_TEST_HOST_SUCCESS(VendorId, uint32_t, OL_DEVICE_INFO_VENDOR_ID);
OL_DEVICE_INFO_TEST_VALUE_GT(NumComputeUnits, uint32_t,
                             OL_DEVICE_INFO_NUM_COMPUTE_UNITS, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(SingleFPConfig, ol_device_fp_capability_flags_t,
                             OL_DEVICE_INFO_SINGLE_FP_CONFIG, 0);
OL_DEVICE_INFO_TEST_SUCCESS(HalfFPConfig, ol_device_fp_capability_flags_t,
                            OL_DEVICE_INFO_HALF_FP_CONFIG);
OL_DEVICE_INFO_TEST_VALUE_GT(DoubleFPConfig, ol_device_fp_capability_flags_t,
                             OL_DEVICE_INFO_DOUBLE_FP_CONFIG, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(NativeVectorWidthChar, uint32_t,
                             OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_CHAR, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(NativeVectorWidthShort, uint32_t,
                             OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_SHORT, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(NativeVectorWidthInt, uint32_t,
                             OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_INT, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(NativeVectorWidthLong, uint32_t,
                             OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_LONG, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(NativeVectorWidthFloat, uint32_t,
                             OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_FLOAT, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(NativeVectorWidthDouble, uint32_t,
                             OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_DOUBLE, 0);
OL_DEVICE_INFO_TEST_SUCCESS(SingleFPSupport, bool,
                            OL_DEVICE_INFO_SINGLE_FP_SUPPORT);
OL_DEVICE_INFO_TEST_SUCCESS(DoubleFPSupport, bool,
                            OL_DEVICE_INFO_DOUBLE_FP_SUPPORT);
OL_DEVICE_INFO_TEST_SUCCESS(HalfFPSupport, bool,
                            OL_DEVICE_INFO_HALF_FP_SUPPORT);
OL_DEVICE_INFO_TEST_SUCCESS(NativeVectorWidthHalf, uint32_t,
                            OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_HALF);
OL_DEVICE_INFO_TEST_VALUE_GT(MaxClockFrequency, uint32_t,
                             OL_DEVICE_INFO_MAX_CLOCK_FREQUENCY, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(MemoryClockRate, uint32_t,
                             OL_DEVICE_INFO_MEMORY_CLOCK_RATE, 0);
OL_DEVICE_INFO_TEST_VALUE_GT(AddressBits, uint32_t, OL_DEVICE_INFO_ADDRESS_BITS,
                             0);
OL_DEVICE_INFO_TEST_DEVICE_VALUE_GT(MaxMemAllocSize, uint64_t,
                                    OL_DEVICE_INFO_MAX_MEM_ALLOC_SIZE, 0);
OL_DEVICE_INFO_TEST_HOST_SUCCESS(MaxMemAllocSize, uint64_t,
                                 OL_DEVICE_INFO_MAX_MEM_ALLOC_SIZE);
OL_DEVICE_INFO_TEST_DEVICE_VALUE_GT(GlobalMemSize, uint64_t,
                                    OL_DEVICE_INFO_GLOBAL_MEM_SIZE, 0);
OL_DEVICE_INFO_TEST_HOST_SUCCESS(GlobalMemSize, uint64_t,
                                 OL_DEVICE_INFO_GLOBAL_MEM_SIZE);
OL_DEVICE_INFO_TEST_DEVICE_VALUE_GT(SharedMemSize, uint64_t,
                                    OL_DEVICE_INFO_WORK_GROUP_LOCAL_MEM_SIZE,
                                    0);
OL_DEVICE_INFO_TEST_HOST_SUCCESS(SharedMemSize, uint64_t,
                                 OL_DEVICE_INFO_WORK_GROUP_LOCAL_MEM_SIZE);

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
