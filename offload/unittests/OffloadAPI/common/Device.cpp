#include "Device.hpp"
// #include <OffloadAPI.h>

PropertiesSet PropBool{OL_DEVICE_INFO_SINGLE_FP_SUPPORT,
                       OL_DEVICE_INFO_DOUBLE_FP_SUPPORT,
                       OL_DEVICE_INFO_HALF_FP_SUPPORT};
PropertyTuples BoolProperties = createPropertyTuples(sizeof(bool), PropBool);

PropertiesSet PropUint32{OL_DEVICE_INFO_MAX_WORK_GROUP_SIZE,
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
PropertyTuples Uint32Properties =
    createPropertyTuples(sizeof(uint32_t), PropUint32);

PropertiesSet PropUint64{OL_DEVICE_INFO_MAX_MEM_ALLOC_SIZE,
                         OL_DEVICE_INFO_GLOBAL_MEM_SIZE,
                         OL_DEVICE_INFO_WORK_GROUP_LOCAL_MEM_SIZE};
PropertyTuples Uint64Properties =
    createPropertyTuples(sizeof(uint64_t), PropUint64);

PropertiesSet PropCapabilitiesFlags{OL_DEVICE_INFO_SINGLE_FP_CONFIG,
                                    OL_DEVICE_INFO_HALF_FP_CONFIG,
                                    OL_DEVICE_INFO_DOUBLE_FP_CONFIG};
// sizeof(ol_device_fp_capability_flags_t) == sizegof(uint32_t)
PropertyTuples CapabilitesFlagsProperties = createPropertyTuples(
    sizeof(ol_device_fp_capability_flags_t), PropCapabilitiesFlags);

PropertiesSet PropDeviceType{OL_DEVICE_INFO_TYPE};
PropertyTuples DeviceTypeProperties =
    createPropertyTuples(sizeof(ol_device_type_t), PropDeviceType);

PropertiesSet PropPlatform{OL_DEVICE_INFO_PLATFORM};
PropertyTuples PlatformProperties =
    createPropertyTuples(sizeof(ol_platform_handle_t), PropPlatform);

PropertiesSet PropNonZeroMiscellaneous{
    OL_DEVICE_INFO_NAME, OL_DEVICE_INFO_PRODUCT_NAME, OL_DEVICE_INFO_UID,
    OL_DEVICE_INFO_VENDOR, OL_DEVICE_INFO_DRIVER_VERSION};
PropertyTuples NonZeroMiscProperties =
    createPropertyTuples(0, PropNonZeroMiscellaneous);

PropertiesTypes propertiesTypes =
    createTypesMap({BoolProperties, Uint32Properties, Uint64Properties,
                    CapabilitesFlagsProperties});