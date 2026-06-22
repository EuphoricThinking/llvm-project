#pragma once

#include "Fixtures.hpp"

constexpr size_t MAX_DEVICE_INFO_BYTES = 8;

constexpr char zeroArray[MAX_DEVICE_INFO_BYTES] = {};

using PropertiesVec = std::vector<ol_device_info_t>;
using PropertyTuple = std::tuple<size_t, ol_device_info_t>;
using PropertyTuples = std::vector<PropertyTuple>;
using PropertiesSet = std::set<ol_device_info_t>;
using PropertiesTypes = std::unordered_map<ol_device_info_t, PropertyTuple>;

inline size_t getSize(PropertyTuple &prop) { return std::get<0>(prop); }

inline ol_device_info_t getProp(PropertyTuple &prop) { return std::get<1>(prop); }

template <typename Container>
PropertyTuples createPropertyTuples(size_t PropSize,
                                    Container SelectedProperties) {
  PropertyTuples Res;
  for (auto p : SelectedProperties) {
    Res.push_back({PropSize, p});
  }

  return Res;
}

template <typename T>
PropertyTuples mergeProperties(std::initializer_list<T> properties) {
  PropertyTuples finalProperties;

  for (auto prop : properties) {
    finalProperties.insert(finalProperties.end(), prop.begin(), prop.end());
  }

  return finalProperties;
}

PropertyTuples
inline copyRelevantProperties(PropertyTuples properties,
                       std::initializer_list<ol_device_info_t> unwanted,
                       PropertiesTypes typesMap) {
  PropertyTuples res(properties);

  for (auto prop : unwanted) {
    res.erase(std::find(res.begin(), res.end(), typesMap.at(prop)));
  }

  return res;
}

PropertiesTypes
inline createTypesMap(std::initializer_list<PropertyTuples> properties) {
  PropertiesTypes Res;

  for (auto container : properties) {
    for (auto prop : container) {
      Res.insert({getProp(prop), prop});
    }
  }

  return Res;
}

inline bool defaultCheckIfNonZero(char* buffer) {
  return memcmp(buffer, zeroArray, MAX_DEVICE_INFO_BYTES);
}

template <typename Container>
bool isMeaningfulForHost(ol_device_info_t prop, Container notMeaningful) {
  return notMeaningful.find(prop) == notMeaningful.end();
}

extern PropertiesSet PropBool;
extern PropertyTuples BoolProperties;

extern PropertiesSet PropUint32;
extern PropertyTuples Uint32Properties;

extern PropertiesSet PropUint64;
extern PropertyTuples Uint64Properties;

extern PropertiesSet PropCapabilitiesFlags;
// sizeof(ol_device_fp_capability_flags_t) == sizegof(uint32_t)
extern PropertyTuples CapabilitesFlagsProperties;

extern PropertiesSet PropDeviceType;
extern PropertyTuples DeviceTypeProperties;

extern PropertiesSet PropPlatform;
extern PropertyTuples PlatformProperties;

extern PropertiesSet PropNonZeroMiscellaneous;
extern PropertyTuples NonZeroMiscProperties;

extern PropertiesTypes propertiesTypes;

inline std::string olGetHostDeviceInfoPropertyTestPrinter(
    const ::testing::TestParamInfo<OffloadParam<PropertyTuple>> &info) {
  auto device = std::get<0>(info.param);
  auto paramTuple = std::get<1>(info.param);

  std::string ss;
  llvm::raw_string_ostream finalName(ss);

  auto property = std::get<1>(paramTuple);
  finalName << device.Name << "__" << property;

  return SanitizeString(finalName.str());
}

struct olGetHostDeviceInfoPropertyTest
    : OffloadDeviceTestWithParam<PropertyTuple> {
  void SetUp() override {
    RETURN_ON_FATAL_FAILURE(OffloadDeviceTestWithParam<PropertyTuple>::SetUp());

    auto paramTuple = this->getTestParam();
    PropertySize = std::get<0>(paramTuple);
    Property = std::get<1>(paramTuple);
  }

  size_t PropertySize = 0;
  ol_device_info_t Property;

  bool isHost() { return Host == this->Device; }
};