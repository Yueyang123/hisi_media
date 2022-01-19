/*
 * @Description: 
 * @Autor: YURI
 * @Date: 2022-01-18 21:34:07
 * @LastEditors: YURI
 * @LastEditTime: 2022-01-19 04:16:22
 */
#ifndef HISI_YY_H
#define HISI_YY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "hi_common.h"
#include "sample_vio.h"
#include "mpi_sys.h"
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <time.h>
//初始化 VI VO VPSS
HI_S32 vio_init(HI_U32 u32VoIntfType);
void vio_exit();
int get_enPicSize(void);
void SAMPLE_VIO_HandleSig(HI_S32 signo);
#endif