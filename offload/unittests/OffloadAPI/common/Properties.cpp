#include "Properties.hpp"

// ol_device_info_t
DeviceInfoProp PropBool{OL_DEVICE_INFO_SINGLE_FP_SUPPORT,
                       OL_DEVICE_INFO_DOUBLE_FP_SUPPORT,
                       OL_DEVICE_INFO_HALF_FP_SUPPORT};
DeviceInfoProperties BoolProperties = createPropertyTuples(sizeof(bool), PropBool);

DeviceInfoProp PropUint32{OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE,
                         OL_DEVICE_INFO_MAX_WORK_SIZE,
                         OL_DEVICE_INFO_VENDOR_ID,
                         OL_DEVICE_INFO_NUM_COMPUTE_UNITS,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_CHAR,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_SHORT,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_INT,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_LONG,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_FLOAT,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_DOUBLE,
                         OL_DEVICE_INFO_NATIVE_VECTOR_WIDTH_HALF,
                         OL_DEVICE_INFO_MAX_CLOCK_FREQUENCY,
                         OL_DEVICE_INFO_MEMORY_CLOCK_RATE,
                         OL_DEVICE_INFO_ADDRESS_BITS};
DeviceInfoProperties Uint32Properties =
    createPropertyTuples(sizeof(uint32_t), PropUint32);

DeviceInfoProp PropUint64{OL_DEVICE_INFO_MAX_MEM_ALLOC_SIZE,
                         OL_DEVICE_INFO_GLOBAL_MEM_SIZE,
                         OL_DEVICE_INFO_WORK_GROUP_LOCAL_MEM_SIZE};
DeviceInfoProperties Uint64Properties =
    createPropertyTuples(sizeof(uint64_t), PropUint64);

DeviceInfoProp PropCapabilitiesFlags{OL_DEVICE_INFO_SINGLE_FP_CONFIG,
                                    OL_DEVICE_INFO_HALF_FP_CONFIG,
                                    OL_DEVICE_INFO_DOUBLE_FP_CONFIG};
// sizeof(ol_device_fp_capability_flags_t) == sizegof(uint32_t)
DeviceInfoProperties CapabilitesFlagsProperties = createPropertyTuples(
    sizeof(ol_device_fp_capability_flags_t), PropCapabilitiesFlags);

DeviceInfoProp PropDeviceType{OL_DEVICE_INFO_TYPE};
DeviceInfoProperties DeviceTypeProperties =
    createPropertyTuples(sizeof(ol_device_type_t), PropDeviceType);

DeviceInfoProp PropPlatform{OL_DEVICE_INFO_PLATFORM};
DeviceInfoProperties PlatformProperties =
    createPropertyTuples(sizeof(ol_platform_handle_t), PropPlatform);

DeviceInfoProp PropNames{
    OL_DEVICE_INFO_NAME, OL_DEVICE_INFO_PRODUCT_NAME, OL_DEVICE_INFO_UID,
    OL_DEVICE_INFO_VENDOR, OL_DEVICE_INFO_DRIVER_VERSION};
DeviceInfoProperties NamesProperties =
    createPropertyTuples(0, PropNames);

DeviceInfoProp PropDimensions{OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE_PER_DIMENSION, OL_DEVICE_INFO_MAX_WORK_SIZE_PER_DIMENSION};
DeviceInfoProperties DimensionsProperties = createPropertyTuples(sizeof(ol_dimensions_t), PropDimensions);

DeviceInfoPropertiesTypes propertiesTypes =
    createTypesMap({BoolProperties, Uint32Properties, Uint64Properties,
                    CapabilitesFlagsProperties});

// ol_symbol_info_t
SymbolInfoProp PropSymbolInfoGlobal{OL_SYMBOL_INFO_KIND, OL_SYMBOL_INFO_GLOBAL_VARIABLE_ADDRESS, OL_SYMBOL_INFO_GLOBAL_VARIABLE_SIZE};
SymbolInfoProperties SymbolGlobalProperties{
{sizeof(ol_symbol_kind_t), OL_SYMBOL_INFO_KIND},
{sizeof(void *), OL_SYMBOL_INFO_GLOBAL_VARIABLE_ADDRESS},
{sizeof(size_t), OL_SYMBOL_INFO_GLOBAL_VARIABLE_SIZE}
};

// ol_platform_info_t
ol_platform_info_t PlatformInfoNames[3] = {OL_PLATFORM_INFO_NAME, OL_PLATFORM_INFO_VENDOR_NAME, OL_PLATFORM_INFO_VERSION};

// ol_alloc_type_t
ol_alloc_type_t AllocTypes[3] = {
      OL_ALLOC_TYPE_DEVICE, OL_ALLOC_TYPE_MANAGED, OL_ALLOC_TYPE_HOST};

// ol_mem_info_t
MemInfoProp PropMemInfo{OL_MEM_INFO_DEVICE, OL_MEM_INFO_BASE, OL_MEM_INFO_SIZE, OL_MEM_INFO_TYPE};
MemInfoProperties MemInfoSizeProperties{
    {sizeof(ol_device_handle_t), OL_MEM_INFO_DEVICE},
    {sizeof(void *), OL_MEM_INFO_BASE},
    {sizeof(size_t), OL_MEM_INFO_SIZE},
    {sizeof(ol_alloc_type_t), OL_MEM_INFO_TYPE}
};