SRCS := $(wildcard ./*.c)
TARGET := hisi_media
TARGET_PATH := $(PWD)
MPPDIR :=/home/swann/SDK/HI3516/MYMMP/mpp
COMMON_DIR = $(MPPDIR)/sample/common
COMMON_SRC := $(wildcard $(COMMON_DIR)/*.c)
COMMON_OBJ := $(COMMON_SRC:%.c=%.o)

HISILIB:= $(MPPDIR)/lib
OPENCVLIB =-I /home/swann/SDK/HI3516/MYMMP/mpp/opencv/include -L /home/swann/SDK/HI3516/MYMMP/mpp/opencv/lib/lib*
FFMPEGLIB =-I /home/swann/SDK/HI3516/MYMMP/mpp/ffmpeg/include -L /home/swann/SDK/HI3516/MYMMP/mpp/ffmpeg/lib/lib*


INCLUDE :=-I./ -I$(MPPDIR)/include  -I$(MPPDIR)/sample/audio/adp -I$(MPPDIR)/sample/common \
	-I/home/swann/SDK/HI3516/MYMMP/mpp/sample/vio
CFLAGS:=-lpthread -lm -ldl -DISP_V2 -lstdc++ -Wall -mcpu=cortex-a7 -mfloat-abi=softfp \
-mfpu=neon-vfpv4 -fno-aggressive-loop-optimizations -ldl -ffunction-sections -fdata-sections \
-O2 -fstack-protector-strong -fPIC -Wall  -DHI_RELEASE -Wno-error=implicit-function-declaration \
-DVER_X=1 -DVER_Y=0 -DVER_Z=0 -DVER_P=0 -DVER_B=10 -DUSER_BIT_32 -DKERNEL_BIT_32 -Wno-date-time	\
-DSENSOR0_TYPE=GALAXYCORE_GC2053_MIPI_2M_30FPS_10BIT -DSENSOR1_TYPE=GALAXYCORE_GC2053_MIPI_2M_30FPS_10BIT	\
-DHI_ACODEC_TYPE_INNER -DHI_ACODEC_TYPE_HDMI 
OBJS  := $(SRCS:%.c=%.o)
$(TARGET):$(COMMON_OBJ) $(OBJS)
	arm-himix200-linux-gcc -Wall -g $(INCLUDE)  $(CFLAGS)  -o $(TARGET_PATH)/$@ $^ -Wl,--start-group 	\
	$(HISILIB)/libmpi.a		 	$(HISILIB)/libhdmi.a \
	$(HISILIB)/lib_hiae.a 	 	$(HISILIB)/libisp.a \
	$(HISILIB)/lib_hidehaze.a 	$(HISILIB)/lib_hidrc.a \
	$(HISILIB)/lib_hildci.a 	$(HISILIB)/lib_hicalcflicker.a \
	$(HISILIB)/lib_hiawb.a 	 	$(HISILIB)/libsns_gc2053.a  \
	$(HISILIB)/libVoiceEngine.a $(HISILIB)/libupvqe.a \
	$(HISILIB)/libdnvqe.a		$(HISILIB)/libsecurec.a \
	$(HISILIB)/libsns_imx327.a  $(HISILIB)/libsns_imx327_2l.a \
	$(HISILIB)/libsns_imx307.a  $(HISILIB)/libsns_imx307_2l.a \
	$(HISILIB)/libsns_imx458.a  $(HISILIB)/libsns_mn34220.a \
	$(HISILIB)/libsns_os05a.a   $(HISILIB)/libsns_os08a10.a \
	$(HISILIB)/libsns_sc4210.a  $(HISILIB)/libsns_ov12870.a \
	$(HISILIB)/libsns_os04b10.a $(HISILIB)/libsns_imx415.a \
	$(HISILIB)/libsns_imx219.a  $(HISILIB)/libsns_imx274.a \
 	$(HISILIB)/libsecurec.a 	$(HISILIB)/libsns_imx335.a \
	-Wl,--end-group  $(FFMPEGLIB) 
$(OBJS):$(COMMON_SRC) $(SRCS)
	arm-himix200-linux-gcc -Wall -g   $(INCLUDE) $(CFLAGS)  $(FFMPEGLIB)  -c  $@ $^
clean:
	rm -f $(TARGET_PATH)/$(TARGET)
	rm -f $(OBJS)

