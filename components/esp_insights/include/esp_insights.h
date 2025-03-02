// Copyright 2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once
#include <stdint.h>
#include <esp_err.h>
#include <esp_event_base.h>
#include <esp_diagnostics.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Insights event base
 */
ESP_EVENT_DECLARE_BASE(INSIGHTS_EVENT);

/**
 * @brief ESP Insights configuration
 */
typedef struct {
    /** Log types to enable, bitwise OR the values from \ref esp_diag_log_type_t */
    uint32_t log_type;
    /** Node id for insights. If NULL then insights agent uses MAC address as node id */
    const char *node_id;
    /** Authentication key, valid only for https transport */
    const char *auth_key;
} esp_insights_config_t;

/**
 * @brief Insights events
 *
 * Transport layer emits events using default event loop, every transport
 * event has event data of type \ref esp_insights_transport_event_data_t;
 */
typedef enum {
    /** Asynchronous data send succeded. Event data contains the msg_id of the data. */
    INSIGHTS_EVENT_TRANSPORT_SEND_SUCCESS,
    /** Asynchronous data send failed. Event data contains the msg_id of the data. */
    INSIGHTS_EVENT_TRANSPORT_SEND_FAILED,
    /** Data received. Event data contains received data and data_len. */
    INSIGHTS_EVENT_TRANSPORT_RECV,
} esp_insights_event_t;

/**
 * @brief Insights transport event data
 */
typedef struct {
    uint8_t *data;          /*!< Data associated with the event */
    size_t data_len;        /*!< Length of the data for the event */
    int msg_id;             /*!< Message id */
} esp_insights_transport_event_data_t;

/**
 * @brief Insights transport init callback prototype
 *
 * @param[in] userdata User data
 *
 * @return ESP_OK on success, appropriate error code otherwise.
 */
typedef esp_err_t(*esp_insights_transport_init_t)(void *userdata);

/**
 * @brief Insights transport deinit callback prototype
 */
typedef void(*esp_insights_transport_deinit_t)(void);

/**
 * @brief Insights transport connect callback prototype
 *
 * @return ESP_OK on success, appropriate error code otherwise.
 */
typedef esp_err_t(*esp_insights_transport_connect_t)(void);

/**
 * @brief Insights transport disconnect callback prototype
 */
typedef void(*esp_insights_transport_disconnect_t)(void);

/**
 * @brief Insights transport data send callback prototype
 *
 * @param[in] data Data to send
 * @param[in] len  Length of data
 *
 * @return msg_id  Message_id of the sent data.
 *                 On failure, -1
 *                 On success, 0 if data send happens synchronously.
 *                 On success, message-id(positive integer) if data send happened asynchronously.
 *
 * @note If data send happened asynchronously then appropriate events in \ref esp_insights_event_t must be emitted.
 */
typedef int(*esp_insights_transport_data_send_t)(void *data, size_t len);

/**
 * @brief Insights transport configurations
 */
typedef struct {
    /** Insights transport callback functions */
    struct {
        /** Transport init function */
        esp_insights_transport_init_t init;
        /** Transport deinit function */
        esp_insights_transport_deinit_t deinit;
        /** Transport connect function */
        esp_insights_transport_connect_t connect;
        /** Transport disconnect function */
        esp_insights_transport_disconnect_t disconnect;
        /** Function to send data */
        esp_insights_transport_data_send_t data_send;
    } callbacks;
    /** User data */
    void *userdata;
} esp_insights_transport_config_t;

/**
 * @brief Initialize ESP Insights
 *
 * @param[in] config Configuration for ESP Insights.
 *
 * @return ESP_OK on success, appropriate error code otherwise
 */
esp_err_t esp_insights_init(esp_insights_config_t *config);

/**
 * @brief Deinitialize ESP Insights
 */
void esp_insights_deinit(void);

/**
 * @brief Register insights transport
 *
 * @param[in] config Configurations of type \ref esp_insights_transport_config_t
 *
 * @return ESP_OK on success, appropriate error code otherwise
 */
esp_err_t esp_insights_transport_register(esp_insights_transport_config_t *config);

/**
 * @brief Unregister insights transport
 */
void esp_insights_transport_unregister(void);

#ifdef __cplusplus
}
#endif
