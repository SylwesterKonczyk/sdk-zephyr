/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file zperf.h
 *
 * @brief Zperf API
 * @defgroup zperf Zperf API
 * @since 3.3
 * @version 0.8.0
 * @ingroup networking
 * @{
 */

#ifndef ZEPHYR_INCLUDE_NET_ZPERF_H_
#define ZEPHYR_INCLUDE_NET_ZPERF_H_

#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @cond INTERNAL_HIDDEN */

enum zperf_status {
	ZPERF_SESSION_STARTED,
	ZPERF_SESSION_PERIODIC_RESULT,
	ZPERF_SESSION_FINISHED,
	ZPERF_SESSION_ERROR
} __packed;

struct zperf_upload_params {
	struct sockaddr peer_addr;
	uint32_t duration_ms;
	uint32_t rate_kbps;
	uint16_t packet_size;
	char if_name[IFNAMSIZ];
	struct {
		uint8_t tos;
		int tcp_nodelay;
		int priority;
#ifdef CONFIG_ZPERF_SESSION_PER_THREAD
		int thread_priority;
		bool wait_for_start;
#endif
		uint32_t report_interval_ms;
	} options;
};

struct zperf_download_params {
	uint16_t port;
	struct sockaddr addr;
	char if_name[IFNAMSIZ];
};

/** @endcond */

/** Performance results */
struct zperf_results {
	uint32_t nb_packets_sent;     /**< Number of packets sent */
	uint32_t nb_packets_rcvd;     /**< Number of packets received */
	uint32_t nb_packets_lost;     /**< Number of packets lost */
	uint32_t nb_packets_outorder; /**< Number of packets out of order */
	uint64_t total_len;           /**< Total length of the transferred data */
	uint64_t time_in_us;          /**< Total time of the transfer in microseconds */
	uint32_t jitter_in_us;        /**< Jitter in microseconds */
	uint64_t client_time_in_us;   /**< Client connection time in microseconds */
	uint32_t packet_size;         /**< Packet size */
	uint32_t nb_packets_errors;   /**< Number of packet errors */
};

/**
 * @brief Zperf callback function used for asynchronous operations.
 *
 * @param status Session status.
 * @param result Session results. May be NULL for certain events.
 * @param user_data A pointer to the user provided data.
 */
typedef void (*zperf_callback)(enum zperf_status status,
			       struct zperf_results *result,
			       void *user_data);

/**
 * @brief Synchronous UDP upload operation. The function blocks until the upload
 *        is complete.
 *
 * @param param Upload parameters.
 * @param result Session results.
 *
 * @return 0 if session completed successfully, a negative error code otherwise.
 */
int zperf_udp_upload(const struct zperf_upload_params *param,
		     struct zperf_results *result);

/**
 * @brief Synchronous TCP upload operation. The function blocks until the upload
 *        is complete.
 *
 * @param param Upload parameters.
 * @param result Session results.
 *
 * @return 0 if session completed successfully, a negative error code otherwise.
 */
int zperf_tcp_upload(const struct zperf_upload_params *param,
		     struct zperf_results *result);

/**
 * @brief Asynchronous UDP upload operation.
 *
 * @note Only one asynchronous upload can be performed at a time.
 *
 * @param param Upload parameters.
 * @param callback Session results callback.
 * @param user_data A pointer to the user data to be provided with the callback.
 *
 * @return 0 if session was scheduled successfully, a negative error code
 *         otherwise.
 */
int zperf_udp_upload_async(const struct zperf_upload_params *param,
			   zperf_callback callback, void *user_data);

/**
 * @brief Asynchronous TCP upload operation.
 *
 * @note Only one asynchronous upload can be performed at a time.
 *
 * @param param Upload parameters.
 * @param callback Session results callback.
 * @param user_data A pointer to the user data to be provided with the callback.
 *
 * @return 0 if session was scheduled successfully, a negative error code
 *         otherwise.
 */
int zperf_tcp_upload_async(const struct zperf_upload_params *param,
			   zperf_callback callback, void *user_data);

/**
 * @brief Start UDP server.
 *
 * @note Only one UDP server instance can run at a time.
 *
 * @param param Download parameters.
 * @param callback Session results callback.
 * @param user_data A pointer to the user data to be provided with the callback.
 *
 * @return 0 if server was started, a negative error code otherwise.
 */
int zperf_udp_download(const struct zperf_download_params *param,
		       zperf_callback callback, void *user_data);

/**
 * @brief Start TCP server.
 *
 * @note Only one TCP server instance can run at a time.
 *
 * @param param Download parameters.
 * @param callback Session results callback.
 * @param user_data A pointer to the user data to be provided with the callback.
 *
 * @return 0 if server was started, a negative error code otherwise.
 */
int zperf_tcp_download(const struct zperf_download_params *param,
		       zperf_callback callback, void *user_data);

/**
 * @brief Stop UDP server.
 *
 * @return 0 if server was stopped successfully, a negative error code otherwise.
 */
int zperf_udp_download_stop(void);

/**
 * @brief Stop TCP server.
 *
 * @return 0 if server was stopped successfully, a negative error code otherwise.
 */
int zperf_tcp_download_stop(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_NET_ZPERF_H_ */
