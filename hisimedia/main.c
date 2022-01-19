/*
 * @Description: 
 * @Autor: YURI
 * @Date: 2022-01-16 20:38:49
 * @LastEditors: YURI
 * @LastEditTime: 2022-01-19 04:20:26
 */
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
#include "hisi.h"

int main(int argc, char *argv[])
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32Index;
    HI_U32 u32VoIntfType = 0;
    HI_U32  u32ChipId;


    signal(SIGINT, SAMPLE_VIO_HandleSig);
    signal(SIGTERM, SAMPLE_VIO_HandleSig);
    u32VoIntfType = 0;
    s32Ret = vio_init(u32VoIntfType);//初始化摄像设备
    s32Ret = hifb_init();//初始化屏幕

    getchar();
    vio_exit();
    return s32Ret;
}

