/* generated configuration header file - do not edit */
#ifndef OTA_CONFIG_H_
#define OTA_CONFIG_H_

#ifndef otaconfigLOG2_FILE_BLOCK_SIZE
#define otaconfigLOG2_FILE_BLOCK_SIZE    11UL
#endif

#ifndef otaconfigSELF_TEST_RESPONSE_WAIT_MS
#define otaconfigSELF_TEST_RESPONSE_WAIT_MS    16000U
#endif

#ifndef otaconfigFILE_REQUEST_WAIT_MS
#define otaconfigFILE_REQUEST_WAIT_MS    10000U
#endif

#ifndef otaconfigMAX_THINGNAME_LEN
#define otaconfigMAX_THINGNAME_LEN    64U
#endif

#ifndef otaconfigMAX_NUM_BLOCKS_REQUEST
#define otaconfigMAX_NUM_BLOCKS_REQUEST    1U
#endif

#ifndef otaconfigMAX_NUM_REQUEST_MOMENTUM
#define otaconfigMAX_NUM_REQUEST_MOMENTUM    32U
#endif

#ifndef otaconfigOTA_UPDATE_STATUS_FREQUENCY
#define otaconfigOTA_UPDATE_STATUS_FREQUENCY    64U
#endif

#ifndef otaconfigMAX_NUM_OTA_DATA_BUFFERS
#define otaconfigMAX_NUM_OTA_DATA_BUFFERS    4U
#endif

#ifndef otaconfigAllowDowngrade
#define otaconfigAllowDowngrade    0U
#endif

#ifndef configOTA_FIRMWARE_UPDATE_FILE_TYPE_ID
#define configOTA_FIRMWARE_UPDATE_FILE_TYPE_ID    0U
#endif

#ifndef configENABLED_CONTROL_PROTOCOL
#define configENABLED_CONTROL_PROTOCOL    ( OTA_CONTROL_OVER_MQTT )
#endif

#ifndef configENABLED_DATA_PROTOCOLS
#define configENABLED_DATA_PROTOCOLS    ( OTA_DATA_OVER_MQTT )
#endif

#ifndef configOTA_PRIMARY_DATA_PROTOCOL
#define configOTA_PRIMARY_DATA_PROTOCOL    ( OTA_DATA_OVER_MQTT )
#endif

#ifndef otaconfigOTA_FILE_TYPE
#define otaconfigOTA_FILE_TYPE    uint8_t
#endif

#ifndef configOTA_POLLING_EVENTS_TIMEOUT_MS
#define configOTA_POLLING_EVENTS_TIMEOUT_MS     ( 1000 )
#endif

#ifndef otaconfigOTA_VERION_MAJOR
#define otaconfigOTA_VERION_MAJOR 0
#endif
#ifndef otaconfigOTA_VERION_MINOR
#define otaconfigOTA_VERION_MINOR 0
#endif
#ifndef otaconfigOTA_VERION_BUILD
#define otaconfigOTA_VERION_BUILD 1
#endif
#endif /* OTA_CONFIG_H_ */
