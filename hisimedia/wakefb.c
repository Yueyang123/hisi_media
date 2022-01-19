/*
 * @Description: 
 * @Autor: YURI
 * @Date: 2022-01-19 03:44:04
 * @LastEditors: YURI
 * @LastEditTime: 2022-01-19 04:04:12
 */
#include <stdio.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/mman.h> 
#include <linux/fb.h> 
#include "hifb.h"
#include "hi_comm_vo.h"
#include "sample_comm.h"
#include "hisi.h"
#define IMAGE_WIDTH 1920 
#define IMAGE_HEIGHT 1080 
#define IMAGE_SIZE (IMAGE_WIDTH*IMAGE_HEIGHT*4) 

static struct fb_bitfield g_a32 = {24, 8, 0};
static struct fb_bitfield g_r32 = {16, 8, 0};
static struct fb_bitfield g_g32 = {8,  8, 0};
static struct fb_bitfield g_b32 = {0,  8, 0};

typedef struct hiPTHREAD_HIFB_SAMPLE
{
    HI_S32           fd;          /* fb's file describ */
    HI_S32           layer;       /* which graphic layer */
    HI_S32           ctrlkey;     /* {0,1,2,3}={1buffer, 2buffer, 0buffer pan display, 0buffer refresh} */
    HI_BOOL          bCompress;   /* image compressed or not */
    HIFB_COLOR_FMT_E enColorFmt;  /* color format. */
} PTHREAD_HIFB_SAMPLE_INFO;

typedef struct hiVO_DEVICE_INFO
{
    VO_DEV           VoDev;
    VO_INTF_TYPE_E   enVoIntfType;
 }VO_DEVICE_INFO;
int hifb_init()
{
    int fd=-1;
    struct fb_fix_screeninfo fix; 
    struct fb_var_screeninfo var; 
    unsigned char *pShowScreen; 
    unsigned char *pHideScreen; 
    HIFB_POINT_S stPoint = {0, 0}; 
    VO_PUB_ATTR_S stPubAttr = {0}; 

    SAMPLE_VO_CONFIG_S      pstVoDevInfo;
    HI_S32                   s32Ret = HI_SUCCESS;

    // VB_CONFIG_S              stVbConf;
    // memset(&stVbConf, 0, sizeof(VB_CONFIG_S));
    // s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);

    s32Ret = SAMPLE_COMM_VO_GetDefConfig(&pstVoDevInfo);
    VO_DEV         VoDev        = pstVoDevInfo.VoDev;
    VO_INTF_TYPE_E enVoIntfType  = pstVoDevInfo.enVoIntfType;

    pstVoDevInfo.enDstDynamicRange = DYNAMIC_RANGE_SDR8;
    pstVoDevInfo.enVoIntfType = VO_INTF_HDMI;
    pstVoDevInfo.enPicSize = get_enPicSize();
    s32Ret = SAMPLE_COMM_VO_StartVO(&pstVoDevInfo);


    /*0. open VO device 0 */ 
    /* …… initialize the attributes for stPubAttr */ 
    HI_MPI_VO_SetPubAttr(0, &stPubAttr); 
    HI_MPI_VO_Enable(0); 
    /*1. open Framebuffer device overlay 0*/ 
    fd = open("/dev/fb0", O_RDWR); 
    if(fd < 0) 
    { 
        printf("open fb0 failed!\n"); 
        return -1; 
    } 
    /*2. set the screen original position*/ 
    if (ioctl(fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0) 
    { 
        printf("set screen original show position failed!\n"); 
        return -1; 
    } 
    /*3. get the variable screen info*/ 
    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0) 
    { 
        printf("Get variable screen info failed!\n"); 
        close(fd); 
        return -1; 
    } 
    /*4. modify the variable screen info 
    the screen size: IMAGE_WIDTH*IMAGE_HEIGHT 
    the virtual screen size: IMAGE_WIDTH*(IMAGE_HEIGHT*4) 
    the pixel format: ARGB8888
    */ 
    var.xres = var.xres_virtual = IMAGE_WIDTH; 
    var.yres = IMAGE_HEIGHT; 
    var.yres_virtual = IMAGE_HEIGHT*2; 
    
    var.transp= g_a32; 
    var.red = g_r32; 
    var.green = g_g32; 
    var.blue = g_b32; 
    var.bits_per_pixel = 32; 
    /*5. set the variable screeninfo*/ 
    if (ioctl(fd, FBIOPUT_VSCREENINFO, &var) < 0) 
    { 
        printf("Put variable screen info failed!\n"); 
        close(fd); 
        return -1; 
    } 
 
    /*6. get the fix screen info*/ 
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fix) < 0) 
    { 
        printf("Get fix screen info failed!\n"); 
        close(fd); 
        return -1; 
    } 
    //s32Ret = SAMPLE_COMM_VPSS_Bind_VO(0, 0, pstVoDevInfo.VoDev, 0);
}
