#pragma once

#include "Fixtures.hpp"
#include "OffloadAPI.h"

inline constexpr size_t MAX_DEVICE_INFO_BYTES = 8;

constexpr char zeroArray[MAX_DEVICE_INFO_BYTES] = {};

template <typename T> struct SizedProperty {
  size_t size;
  T property;
};

template <typename T>
using PropertiesWithSizeContainer = std::vector<SizedProperty<T>>;
template <typename T> using PropertiesContainer = std::set<T>;
template <typename T>
using PropertiesTypes = std::unordered_map<T, SizedProperty<T>>;

template <typename T>
auto createPropertiesWithSizeContainer(
    size_t PropSize, PropertiesContainer<T> SelectedProperties)
    -> PropertiesWithSizeContainer<T> {
  PropertiesWithSizeContainer<T> Res;
  for (auto p : SelectedProperties) {
    Res.push_back({PropSize, p});
  }

  return Res;
}

template <typename T>
auto mergeProperties(std::initializer_list<PropertiesWithSizeContainer<T>>
                         properties) -> PropertiesWithSizeContainer<T> {
  PropertiesWithSizeContainer<T> finalProperties;

  for (auto prop : properties) {
    finalProperties.insert(finalProperties.end(), prop.begin(), prop.end());
  }

  return finalProperties;
}

template <typename T>
PropertiesContainer<T>
removeIrrelevantProperties(PropertiesContainer<T> base,
                           PropertiesContainer<T> unwanted) {
  PropertiesContainer<T> res(base);

  for (auto prop : unwanted) {
    res.erase(prop);
  }

  return res;
}

template <typename T>
PropertiesTypes<T> inline createTypesMap(
    std::initializer_list<PropertiesWithSizeContainer<T>> properties) {
  PropertiesTypes<T> Res;

  for (auto container : properties) {
    for (auto prop : container) {
      Res.insert({prop.property, prop});
    }
  }

  return Res;
}

// ol_device_info_t
using DeviceInfoProp = PropertiesContainer<ol_device_info_t>;
using DeviceInfoProperties = PropertiesWithSizeContainer<ol_device_info_t>;
using DeviceInfoPropertiesTypes = PropertiesTypes<ol_device_info_t>;

extern DeviceInfoProp PropBool;
extern DeviceInfoProperties BoolProperties;

extern DeviceInfoProp PropUint32;
extern DeviceInfoProperties Uint32Properties;

extern DeviceInfoProp PropUint64;
extern DeviceInfoProperties Uint64Properties;

extern DeviceInfoProp PropCapabilitiesFlags;
// sizeof(ol_device_fp_capability_flags_t) == sizeof(uint32_t)
extern DeviceInfoProperties CapabilitesFlagsProperties;

extern DeviceInfoProp PropDeviceType;
extern DeviceInfoProperties DeviceTypeProperties;

extern DeviceInfoProp PropPlatform;
extern DeviceInfoProperties PlatformProperties;

extern DeviceInfoProp PropNames;
extern DeviceInfoProperties NamesProperties;

extern DeviceInfoProp PropDimensions;
extern DeviceInfoProperties DimensionsProperties;

extern DeviceInfoProperties RelevantGTCapabilitiesProperties;
extern DeviceInfoProperties RelevantGTUint32Properties;

extern DeviceInfoPropertiesTypes propertiesTypes;

inline bool defaultCheckIsNonZero(char *buffer) {
  return memcmp(buffer, zeroArray, MAX_DEVICE_INFO_BYTES) != 0;
}

template <typename T>
inline std::string defaultPropertyTestPrinter(
    const ::testing::TestParamInfo<OffloadParam<SizedProperty<T>>> &info) {
  auto device = std::get<0>(info.param);
  auto paramData = std::get<1>(info.param);

  std::string ss;
  llvm::raw_string_ostream finalName(ss);

  auto property = paramData.property;
  finalName << device.Name << "__" << property;

  return SanitizeString(finalName.str());
}

template <typename T>
struct olPropertyTest : OffloadDeviceTestWithParam<SizedProperty<T>> {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(
        OffloadDeviceTestWithParam<SizedProperty<T>>::SetUp());

    auto paramData = this->getTestParam();
    PropertySize = paramData.size;
    Property = paramData.property;
  }

  size_t PropertySize = 0;
  T Property;
};

struct olGetHostDeviceInfoPropertyTest : olPropertyTest<ol_device_info_t> {
  bool isHost() { return Host == this->Device; }
};

struct olGetHostDeviceInfoTest : OffloadDeviceTest {
  void SetUp() override { RETURN_ON_FATAL_FAILURE(OffloadDeviceTest::SetUp()); }

  bool isHost() { return Host == this->Device; }
};

// ol_symbol_info
using SymbolInfoTuple = SizedProperty<ol_symbol_info_t>;
using SymbolInfoProp = PropertiesContainer<ol_symbol_info_t>;
using SymbolInfoProperties = PropertiesWithSizeContainer<ol_symbol_info_t>;

extern SymbolInfoProp PropSymbolInfoGlobal;
extern SymbolInfoProperties SymbolGlobalProperties;

struct olGetSymbolInfoSizeGlobalTest
    : OffloadGlobalTestWithParam<SymbolInfoTuple> {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(
        OffloadGlobalTestWithParam<SymbolInfoTuple>::SetUp());

    auto paramData = this->getTestParam();
    PropertySize = paramData.size;
    Property = paramData.property;
  }

  size_t PropertySize = 0;
  ol_symbol_info_t Property;
};

// ol_platform_info_t
extern ol_platform_info_t PlatformInfoNames[3];

// ol_alloc_type_t
extern ol_alloc_type_t AllocTypes[3];

// ol_mem_info_t
using MemInfoProp = PropertiesContainer<ol_mem_info_t>;
using MemInfoProperties = PropertiesWithSizeContainer<ol_mem_info_t>;

extern MemInfoProp PropMemInfo;
extern MemInfoProperties MemInfoSizeProperties;
