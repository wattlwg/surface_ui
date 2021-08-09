#ifndef SURFACE_UI_H
#define SURFACE_UI_H
///////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hi_go.h"
//#include <gui/ISurfaceComposer.h>
//#include <gui/SurfaceComposerClient.h>

//#include <utils/Log.h>
//#include <utils/String8.h>
//#include <ui/DisplayInfo.h>

//#include <sys/stat.h>
//#include <setjmp.h>
#include "textFont.h"

//using namespace android ;

//#define ASSERT_TRUE(condition)  ;//if(!(condition)){ALOGE("value error!,return -1..");return -1;}
//#define ASSERT_EQ ;
#define FONT_PATH	"/etc/fonts/"

//#define COMMON_LAYER 1000
#define DISP_WIDTH 1280  //1920
#define DISP_HEIGHT 720  //1080

class SurfaceUI {
 	public:
		SurfaceUI();//HI_HANDLE htempLayer,HI_HANDLE htempLayerSurface,HI_HANDLE htempFont);
		~SurfaceUI();
		void deletSurface();
		void initSurface();
		//void lollipop_allinit(void);
		void lollipop_ui_showdlnaimage(void);
    void lollipop_ui_showaudioimage(void);
     void lollipop_ui_showvideoimage(void);
		void show_image(const char* addr,int x,int y);
		void show_text(char* name , char*subname, char* data );
		void UpdateUI();
	
	private:
		pthread_mutex_t mBufLock;
		HI_HANDLE hLayer;
		HI_HANDLE hLayerSurface;
	//	HI_HANDLE hMemSurface;
		bool isInited;
		textFont * mTextFont;
	/*	
		sp<SurfaceComposerClient> mComposerClient;
		sp<SurfaceControl> mSurfaceControl_bg;
		Surface::SurfaceInfo mSurfaceInfo;
		sp<Surface> mSurface;
		uint8_t *mSurfaceBuf;
		unsigned char mDisplayBuf[DISP_WIDTH * DISP_HEIGHT * 4];
		
		char mLanguage[64];
		pthread_t update_pid;

		int getFbScaleConfig(char *scale);
		int setFbScale(char* scale);*/
};

#endif  // SURFACE_UI_H
