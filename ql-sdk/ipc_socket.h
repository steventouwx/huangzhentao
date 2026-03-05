/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: ipc_socket.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义ipc_socket相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef _IPC_SOCKET_H_
#define _IPC_SOCKET_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MSG_LEN 1023

typedef struct
{
    uint8_t type; /** 0-gyroscope 1-accelerometer */
    uint8_t is_buffer;
    int64_t timestamp;
    float data[3];
} __attribute__ ((packed)) msg_sensor_data_t;

int ipc_create_server(const char *ipc_file);
int ipc_close_server(int fd);
int ipc_create_client(const char *ipc_file);
int ipc_msg_send(int fd, void *msg_buf, int msg_len);
int ipc_msg_recv(int fd, void *msg_buf, int msg_len);

#ifdef __cplusplus
}
#endif

#endif

