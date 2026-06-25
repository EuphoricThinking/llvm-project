#pragma once

#include "Fixtures.hpp"
#include "OffloadAPI.h"

constexpr size_t MAX_DEVICE_INFO_BYTES = 8;

constexpr char zeroArray[MAX_DEVICE_INFO_BYTES] = {};

// ol_device_info_t
template <typename T> using PropertiesVec = std::vector<T>;
template <typename T> using PropertyTuple = std::tuple<size_t, T>;
template <typename T> using PropertyTuples = std::vector<PropertyTuple<T>>;

template <typename T> using PropertiesSet = std::set<T>;
template <typename T> using PropertiesTypes = std::unordered_map<T, PropertyTuple<T>>;

using DeviceInfoTuple = PropertyTuple<ol_device_info_t>;
using DeviceInfoProp = PropertiesSet<ol_device_info_t>;
using DeviceInfoProperties = PropertyTuples<ol_device_info_t>;
using DeviceInfoPropertiesTypes = PropertiesTypes<ol_device_info_t>;

template <typename T>
inline size_t getSize(PropertyTuple<T> &prop) { return std::get<0>(prop); }

template <typename T>
inline ol_device_info_t getProp(PropertyTuple<T> &prop) {
  return std::get<1>(prop);
}

template <typename T>
auto createPropertyTuples(size_t PropSize,
                                    PropertiesSet<T> SelectedProperties) {
  PropertyTuples<T> Res;
  for (auto p : SelectedProperties) {
    Res.push_back({PropSize, p});
  }

  return Res;
}

// TODO FIX
template <typename T>
auto mergeProperties(std::initializer_list<PropertyTuples<T>> properties) -> PropertyTuples<T> {
  PropertyTuples<T> finalProperties;

  for (auto prop : properties) {
    finalProperties.insert(finalProperties.end(), prop.begin(), prop.end());
  }

  return finalProperties;
}

template <typename T>
PropertyTuples<T> inline copyRelevantProperties(
    PropertyTuples<T> properties, std::initializer_list<T> unwanted,
    PropertiesTypes<T> typesMap) {
  PropertyTuples<T> res(properties);

  for (auto prop : unwanted) {
    res.erase(std::find(res.begin(), res.end(), typesMap.at(prop)));
  }

  return res;
}

template <typename T>
PropertiesTypes<T> inline createTypesMap(
    std::initializer_list<PropertyTuples<T>> properties) {
  PropertiesTypes<T> Res;

  for (auto container : properties) {
    for (auto prop : container) {
      Res.insert({getProp(prop), prop});
    }
  }

  return Res;
}

inline bool defaultCheckIsNonZero(char *buffer) {
  return memcmp(buffer, zeroArray, MAX_DEVICE_INFO_BYTES) != 0;
}

// template <typename Container>
// bool isMeaningfulForHost(ol_device_info_t prop, Container notMeaningful) {
//   return notMeaningful.find(prop) == notMeaningful.end();
// }

extern DeviceInfoProp PropBool;
extern DeviceInfoProperties BoolProperties;

extern DeviceInfoProp PropUint32;
extern DeviceInfoProperties Uint32Properties;

extern DeviceInfoProp PropUint64;
extern DeviceInfoProperties Uint64Properties;

extern DeviceInfoProp PropCapabilitiesFlags;
// sizeof(ol_device_fp_capability_flags_t) == sizegof(uint32_t)
extern DeviceInfoProperties CapabilitesFlagsProperties;

extern DeviceInfoProp PropDeviceType;
extern DeviceInfoProperties DeviceTypeProperties;

extern DeviceInfoProp PropPlatform;
extern DeviceInfoProperties PlatformProperties;

extern DeviceInfoProp PropNames;
extern DeviceInfoProperties NamesProperties;

extern DeviceInfoProp PropDimensions;
extern DeviceInfoProperties DimensionsProperties;


extern DeviceInfoPropertiesTypes propertiesTypes;

inline std::string olGetHostDeviceInfoPropertyTestPrinter(
    const ::testing::TestParamInfo<OffloadParam<PropertyTuple<ol_device_info_t>>> &info) {
  auto device = std::get<0>(info.param);
  auto paramTuple = std::get<1>(info.param);

  std::string ss;
  llvm::raw_string_ostream finalName(ss);

  auto property = std::get<1>(paramTuple);
  finalName << device.Name << "__" << property;

  return SanitizeString(finalName.str());
}

struct olGetHostDeviceInfoPropertyTest
    : OffloadDeviceTestWithParam<DeviceInfoTuple> {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTestWithParam<DeviceInfoTuple>::SetUp());

    auto paramTuple = this->getTestParam();
    PropertySize = std::get<0>(paramTuple);
    Property = std::get<1>(paramTuple);
  }

  size_t PropertySize = 0;
  ol_device_info_t Property;

  bool isHost() { return Host == this->Device; }


};

struct olGetHostDeviceInfoTest : OffloadDeviceTest {
  void SetUp() override { RETURN_ON_FATAL_FAILURE(OffloadDeviceTest::SetUp()); }

  bool isHost() { return Host == this->Device; }
};