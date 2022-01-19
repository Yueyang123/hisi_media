/*
 * @Description: 
 * @Autor: YURI
 * @Date: 2022-01-19 04:11:05
 * @LastEditors: YURI
 * @LastEditTime: 2022-01-19 04:32:18
 */
extern "C"{
#include "sample_comm.h"
#include "hisi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <time.h>
#include "string.h"
}

int nv21_to_rgb(uint8_t *src, int srcw, int srch,AVPixelFormat srcfomart, uint8_t *dst, int dstw, int dsth,AVPixelFormat dstfomart)
{

    uint8_t *buf_rgb=dst;
    uint8_t *buf_420sp=src;
    AVFrame  *frmrgb = av_frame_alloc();
    AVFrame  *frm420sp = av_frame_alloc();
    //绑定数据缓冲区
    avpicture_fill((AVPicture *)frm420sp, buf_420sp, srcfomart, srcw, srch);
    avpicture_fill((AVPicture *)frmrgb, buf_rgb, dstfomart, dstw, dsth);

    //指定原数据格式，分辨率及目标数据格式，分辨率
    struct SwsContext *sws = sws_getContext(srcw, srch, srcfomart, dstw,dsth, dstfomart,
                                            SWS_BILINEAR, NULL, NULL, NULL);
    //转换
    int ret = sws_scale(sws, frm420sp->data, frm420sp->linesize, 0, dsth, frmrgb->data, frmrgb->linesize);
    av_frame_free(&frmrgb);
    av_frame_free(&frm420sp);
    sws_freeContext(sws);
    return  (ret == dsth) ? 0 : -1;
}
/* 用来将VIDEO_FRAME_S 转化为能用的内存数据 */
int frame_to_yuv420sp(VIDEO_FRAME_S *pVBuf,u_char* p_mat_data){
    HI_CHAR* pVBufVirt=NULL;
    PIXEL_FORMAT_E enPixelFormat = pVBuf->enPixelFormat;
    int u32Size=pVBuf->u32Stride[0] * pVBuf->u32Height * 3 / 2;
    pVBufVirt =(HI_CHAR*)HI_MPI_SYS_Mmap(pVBuf->u64PhyAddr[0], u32Size);
    memcpy(p_mat_data,pVBufVirt,u32Size);
    HI_MPI_SYS_Munmap(pVBufVirt, u32Size);
    return 0;
}
//需要事先给yuvbuf分配空间
void read_frame(unsigned char* yuvbuf)
{
    static VIDEO_FRAME_INFO_S stFrame;
    memset(&stFrame, 0, sizeof(stFrame));
	HI_MPI_VPSS_GetChnFrame(0, VPSS_CHN0, &stFrame, -1);
    frame_to_yuv420sp(&stFrame.stVFrame, yuvbuf);
    HI_MPI_VPSS_ReleaseChnFrame(0, VPSS_CHN0, &stFrame); 
}

int main()
{
    int capture_width=1920;
    int capture_height=1080;
    int save_width=1920;
    int save_height=1080;
    unsigned char* yuvbuf=(unsigned char*)malloc(capture_width*capture_height*3/2);
    unsigned char* rgbbuf=(unsigned char*)malloc(save_width*save_height*3);
    clock_t start,finish;
    float FPS;
    int index=0;
    FILE *file_fd=fopen("frame.yuv","wr");
    while(1){
        start=clock();
        read_frame(yuvbuf);
        // nv21_to_rgb(yuvbuf,capture_width,capture_height,AV_PIX_FMT_NV21,
        // rgbbuf,save_width,save_height,AV_PIX_FMT_RGB24);
        //fwrite(yuvbuf,save_width*save_height*3/2,1,file_fd);
        finish=clock();
        FPS=1000/((double)(finish-start)/CLOCKS_PER_SEC*1000);
        index++;
        printf("FPS :%f INDEX %d\r\n" ,FPS,index);
        fflush(stdout);
    }
}