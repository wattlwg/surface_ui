#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "surface_ui.h"
#include "hi_go.h"
#include "hi_go_decoder.h"
#include "hi_unf_disp.h"
#include "hi_adp_mpi.h"
#include "hi_unf_demux.h"
#include "hi_unf_avplay.h"
#include "hi_unf_common.h"
#include "hi_unf_vo.h"
#include "hi_unf_sound.h"
#include "hi_unf_ai.h"
#include "hi_common.h"
#include "hi_unf_disp.h"
#include "hi_unf_hdmi.h"
#include "hi_audio_codec.h"
#include "hi_error_mpi.h"
#include "hi_unf_mce.h"
#include "hi_unf_pdm.h"

#include "hi_adp_mpi.h"
#include "hi_adp.h"
#include "hi_adp_data.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#define LOG_TAG "surface_ui"
//#include "cutils/log.h"
//#include "language.h"
//#include "imageDecode.h"
//#include "textFont.h"

extern "C" {
	//extern int getLollipopConf(char * param, char * value);
	extern void show_text(char* name, char* subname, char* data);
	extern void show_image(const char* addr,int x,int y);
	//extern void lollipop_init_all(void);
	extern void lollipop_ui_init(void);//HI_HANDLE htempLayer,HI_HANDLE htempLayerSurface,HI_HANDLE htempFont);
	extern void lollipop_ui_deinit(void);
	extern void lollipop_ui_update();
	extern void lollipop_ui_delete(void);
	extern void lollipop_ui_reinit(void);
	extern void lollipop_ui_update();
	extern void lollipop_ui_show_dlna_image(void);
	extern void lollipop_ui_show_audio_image(void);
	extern void lollipop_ui_show_video_image(void);
}

static SurfaceUI* ui = NULL;




void show_text(char* name, char * subname, char* data) {
	ui->show_text(name, subname, data);
}

void show_image(const char* addr,int x,int y) {
	ui->show_image(addr, x, y); 
}


/*void lollipop_ui_set_handle(HI_HANDLE htempLayer,HI_HANDLE htempLayerSurface,HI_HANDLE htempFont) {
	hLayer=htempLayer;
	hLayerSurface=htempLayerSurface;
	
}*/
//void lollipop_init_all(void)
//{
	// ui->lollipop_allinit();
//}
void lollipop_ui_init(void){//HI_HANDLE htempLayer,HI_HANDLE htempLayerSurface,HI_HANDLE htempFont) {
	
	ui = new SurfaceUI();//htempLayer,htempLayerSurface,htempFont);
}

void lollipop_ui_update() {
	ui->UpdateUI();
}

void lollipop_ui_deinit(void) {
//	printf("lollipop_ui_deinit enter\r\n");
	delete ui;
}

void lollipop_ui_reinit(void) {
//	printf("lollipop_ui_reinit");
        ui->initSurface();
}
void lollipop_ui_delete(void) {
//	printf("lollipop_ui_delete");
        ui->deletSurface();
}
void lollipop_ui_show_dlna_image(void)
{
	ui->lollipop_ui_showdlnaimage();
}

void lollipop_ui_show_video_image(void)
{
	ui->lollipop_ui_showvideoimage();
}

void lollipop_ui_show_audio_image(void)
{
	ui->lollipop_ui_showaudioimage();
}

SurfaceUI::SurfaceUI(void){//HI_HANDLE htempLayer,HI_HANDLE htempLayerSurface,HI_HANDLE htempFont) {
	HI_S32 ret;
	HIGO_LAYER_INFO_S stLayerInfo;
	//HIGO_BLTOPT_S stBlitOpt;
 // HI_RECT stRect;
  HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
	
	isInited = false;
	//sprintf(xmlPath, "%s%s.xml", FONT_PATH, mLanguage);
	//sprintf(fontPath, "%s%s.ttf", FONT_PATH, mLanguage);

	 
	 ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, DISP_WIDTH, DISP_HEIGHT);
    if (ret != HI_SUCCESS)
    {
    //    HI_PRINT("+++++++++%s %d failure\n",__FUNCTION__,__LINE__);
  //      goto ERR1;
    }

  // ret = HI_GO_CreateText("/etc/font/csl.ttf", NULL,  &hFont);
   
	 ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
       // goto ERR1;
    }
    
    // create graphic layer 
    ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
        //goto ERR2;
    }
    
      ret = HI_GO_CreateLayer(&stLayerInfo,&hLayer);
    if (HI_SUCCESS != ret)
    {
       // goto ERR2;
    }

    //get the graphic layer Surface 
    ret = HI_GO_GetLayerSurface (hLayer,&hLayerSurface);
    if (HI_SUCCESS != ret)
    {
       //goto ERR3;
    }
   // hLayer=htempLayer;
	 // hLayerSurface=htempLayerSurface;
	  isInited = true;
    mTextFont = new textFont("/etc/fonts/Chinese.xml", "/etc/fonts/csl.ttf");
	 
	 
    /** create memory Surface */
    //ret = HI_GO_CreateSurface(DISP_WIDTH,DISP_HEIGHT,HIGO_PF_1555,&hMemSurface);
   // if (HI_SUCCESS != ret)
    //{
       //goto ERR3;
   // }
    
    /** set the attribute of memory surface */
 //   ret = HI_GO_SetSurfaceAlpha(hMemSurface,128);
   // if (HI_SUCCESS != ret)
   // {
    //    goto ERR4;
  //  }
     mBufLock = PTHREAD_MUTEX_INITIALIZER;
  //     return 0;
/*ERR4:
    HI_GO_FreeSurface(hMemSurface);
ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit(); */   
}

void SurfaceUI::initSurface(void) {
	HI_S32 ret;
	HIGO_LAYER_INFO_S stLayerInfo;
//  printf("initSurface enter\r\n");
  HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
  //add by watt 2020.02.24
  /* ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
       // goto ERR1;
    }*/
	if (isInited) {
	//	printf("initSurface isInited enter\r\n");
		return;
	}
	pthread_mutex_lock(&mBufLock);
	isInited = true;
   ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
   // 	printf("initSurface isInited enter err2\r\n");
        //goto ERR2;
    }
  
    ret = HI_GO_CreateLayer(&stLayerInfo,&hLayer);
    if (HI_SUCCESS != ret)
    {
    	printf("initSurface isInited enter err2 22222\r\n");
       // goto ERR2;
    }

    // get the graphic layer Surface 
    ret = HI_GO_GetLayerSurface (hLayer,&hLayerSurface);
    if (HI_SUCCESS != ret)
    {
       //goto ERR3;
       printf("initSurface isInited enter err2 3333\r\n");
    }
  mTextFont = new textFont("/etc/fonts/Chinese.xml", "/etc/fonts/csl.ttf");
  //HI_GO_FillRect(hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
	//printf("deletSurface enter HI_GO_RefreshLayer enter\r\n");
	//HI_GO_RefreshLayer(hLayer, NULL);    
//	sleep(3);  
	pthread_mutex_unlock(&mBufLock);
//	printf("initSurface enter mBufLock\r\n");
}

void SurfaceUI::show_image(const char* filename,int x,int y) {
    HI_S32 ret         = HI_SUCCESS;
    HI_HANDLE hDecoder = 0;
    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;
    HI_HANDLE hDecSurface;
    HI_RECT stRect;
    HIGO_BLTOPT_S stBltOpt = {0};
    
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = filename;
    
	pthread_mutex_lock(&mBufLock);
	
	ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
	ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
	stAttr.Width = stImgInfo.Width;
	stAttr.Height = stImgInfo.Height;
	stAttr.Format = HIGO_PF_8888;
	ret  = HI_GO_DecImgData(hDecoder, 0, NULL, &hDecSurface);
	ret |= HI_GO_DestroyDecoder(hDecoder);
	
	
	  stRect.x = x;
	  stRect.y = y;
    stRect.w = stImgInfo.Width;
    stRect.h = stImgInfo.Height;

    //stBltOpt.EnableScale = HI_TRUE;
      stBltOpt.EnableScale = HI_TRUE;
    /** clean the graphic layer Surface */
    // ret = HI_GO_FillRect(hLayerSurface, NULL, 0xff0000ff, HIGO_COMPOPT_NONE); 
     ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
	//memset(mDisplayBuf,0,DISP_WIDTH*DISP_HEIGHT*4*sizeof(unsigned char)); 
	//decode_image(filename, x, y, mDisplayBuf, DISP_WIDTH, DISP_HEIGHT); //mSurfaceInfo.w, mSurfaceInfo.h);
	 
	 
	pthread_mutex_unlock(&mBufLock);

	//UpdateUI();
}

void SurfaceUI::show_text(char* name , char* subname, char* data) {

	pthread_mutex_lock(&mBufLock);
	mTextFont->show_text(name, subname, data, hLayerSurface,hLayer,NULL);
	pthread_mutex_unlock(&mBufLock);

	//UpdateUI();

}

void SurfaceUI::UpdateUI() {
HI_PIXELDATA pData;  
	if (!isInited) {
		return;
	}
	pthread_mutex_lock(&mBufLock);
//	HI_GO_LockSurface(hLayerSurface, pData, HI_FALSE);
	HI_GO_RefreshLayer(hLayer, NULL);
	//HI_GO_UnlockSurface(hLayerSurface);
	pthread_mutex_unlock(&mBufLock);
}

void SurfaceUI::deletSurface(void) {
	//char* tmpCmd = "himm 0xf8cc5000 0x10;himm 0xf8cc5004 0x1";
 
	if (!isInited) {
		return;
	}
	//printf("deletSurface enter\r\n");
	pthread_mutex_lock(&mBufLock);
	isInited = false;
	//delete mTextFont;
	//printf("deletSurface enter HI_GO_FillRect \r\n");
	HI_GO_FillRect(hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
//	printf("deletSurface enter HI_GO_RefreshLayer enter\r\n");
	HI_GO_RefreshLayer(hLayer, NULL);
	//printf("deletSurface enter HI_GO_RefreshLayer exit\r\n");
	//hLayerSurface=NULL;
	
	//HI_GO_DestroyLayer(hLayer);
	//printf("deletSurface enter HI_GO_DestroyLayer exit\r\n");
	//hLayer=NULL;
	pthread_mutex_unlock(&mBufLock);
	
//	HI_GO_Deinit();
	// system(tmpCmd);
}


void SurfaceUI::lollipop_ui_showdlnaimage(void)
{
	  HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;
    HI_HANDLE hDecSurface;
    HI_RECT stRect;
    HI_RECT stRect1,stRect2;
    HI_S32 ret         = HI_SUCCESS;
    int tmp;
    HI_HANDLE hDecoder = 0;
    
    HIGO_BLTOPT_S stBltOpt = {0};
    const char* filename="/dev/1.jpg";
    
	  
    
     
    
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = filename;
    
	pthread_mutex_lock(&mBufLock);
	
	ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
	ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
	if(stImgInfo.Width > 4095){
       stAttr.Width = 4095;
    }else{
       stAttr.Width = stImgInfo.Width;
    }
	
	 if(stImgInfo.Height > 4095){
        stAttr.Height = 4095;
    }else{
        stAttr.Height = stImgInfo.Height;
    }
	stAttr.Format = HIGO_PF_8888;
	//	if(stImgInfo.Width > 4095 || stImgInfo.Height > 4095){
		 //   printf("stImgInfo.Width > 4095\r\n");
    //    ret = HI_GO_DecImgData(hDecoder, 0, &stAttr, &hDecSurface);
   // }else{
        ret  = HI_GO_DecImgData(hDecoder, 0, NULL, &hDecSurface);
   // }
	//ret |= HI_GO_DestroyDecoder(hDecoder);
	//HI_GO_FillRect(hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
//	printf("deletSurface enter HI_GO_RefreshLayer enter\r\n");
	//   HI_GO_RefreshLayer(hLayer, NULL);
	if ((stImgInfo.Height) * DISP_WIDTH <= (stImgInfo.Width) * DISP_HEIGHT) { // long screen
        tmp = (DISP_WIDTH * (stImgInfo.Height)) / (stImgInfo.Width);
        stRect.x = 0;
        stRect.y = (DISP_HEIGHT - tmp) / 2;
        stRect.w = DISP_WIDTH;
        stRect.h = tmp;
        if(stRect.y!=0)
        	{
        stRect1.y = 0;
        stRect1.x = 0;
        stRect1.w = DISP_WIDTH;
        stRect1.h = stRect.y;
        HI_GO_FillRect(hLayerSurface, &stRect1, 0x00000000, HIGO_COMPOPT_NONE);
        stRect2.y = stRect.h + stRect.y;
        stRect2.x = 0;
        stRect2.w = DISP_WIDTH;
        stRect2.h = DISP_HEIGHT-stRect.y-stRect.h;
        HI_GO_FillRect(hLayerSurface, &stRect2, 0x00000000, HIGO_COMPOPT_NONE);
      }
      stBltOpt.EnableScale = HI_TRUE;
      ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
      HI_GO_RefreshLayer(hLayer, NULL);
        
      
    } else { // high screen
        tmp = (DISP_HEIGHT * (stImgInfo.Width)) / (stImgInfo.Height);
        stRect.x = (DISP_WIDTH - tmp) / 2;
        stRect.y = 0;
        stRect.w = tmp;
        stRect.h = DISP_HEIGHT;
        
        if(stRect.x!=0)
        	{
        stRect1.y = 0;
        stRect1.x = 0;
        stRect1.w = stRect.x;
        stRect1.h = DISP_HEIGHT;
        HI_GO_FillRect(hLayerSurface, &stRect1, 0x00000000, HIGO_COMPOPT_NONE);
        stRect2.y = 0;
        stRect2.x = stRect.w + stRect.x;
        stRect2.w = DISP_WIDTH-stRect.x-stRect.w;;
        stRect2.h = DISP_HEIGHT;
        HI_GO_FillRect(hLayerSurface, &stRect2, 0x00000000, HIGO_COMPOPT_NONE);
      }
       stBltOpt.EnableScale = HI_TRUE;
      ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
      HI_GO_RefreshLayer(hLayer, NULL); 
    }
    

	 
	 
	pthread_mutex_unlock(&mBufLock);
      
}

void SurfaceUI::lollipop_ui_showaudioimage(void)
{
	  HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;
    HI_HANDLE hDecSurface;
    HI_RECT stRect;
    HI_RECT stRect1,stRect2;
    HI_S32 ret         = HI_SUCCESS;
    int tmp;
    HI_HANDLE hDecoder = 0;
    
    HIGO_BLTOPT_S stBltOpt = {0};
    const char* filename="/etc/jpg/music.jpg";
    
	  
    
     
    
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = filename;
    
	pthread_mutex_lock(&mBufLock);
	
	ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
	ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
	if(stImgInfo.Width > 4095){
       stAttr.Width = 4095;
    }else{
       stAttr.Width = stImgInfo.Width;
    }
	
	 if(stImgInfo.Height > 4095){
        stAttr.Height = 4095;
    }else{
        stAttr.Height = stImgInfo.Height;
    }
	stAttr.Format = HIGO_PF_8888;
	//	if(stImgInfo.Width > 4095 || stImgInfo.Height > 4095){
		 //   printf("stImgInfo.Width > 4095\r\n");
    //    ret = HI_GO_DecImgData(hDecoder, 0, &stAttr, &hDecSurface);
   // }else{
        ret  = HI_GO_DecImgData(hDecoder, 0, NULL, &hDecSurface);
   // }
	//ret |= HI_GO_DestroyDecoder(hDecoder);
	//HI_GO_FillRect(hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
//	printf("deletSurface enter HI_GO_RefreshLayer enter\r\n");
	//   HI_GO_RefreshLayer(hLayer, NULL);
	if ((stImgInfo.Height) * DISP_WIDTH <= (stImgInfo.Width) * DISP_HEIGHT) { // long screen
        tmp = (DISP_WIDTH * (stImgInfo.Height)) / (stImgInfo.Width);
        stRect.x = 0;
        stRect.y = (DISP_HEIGHT - tmp) / 2;
        stRect.w = DISP_WIDTH;
        stRect.h = tmp;
        if(stRect.y!=0)
        	{
        stRect1.y = 0;
        stRect1.x = 0;
        stRect1.w = DISP_WIDTH;
        stRect1.h = stRect.y;
        HI_GO_FillRect(hLayerSurface, &stRect1, 0x00000000, HIGO_COMPOPT_NONE);
        stRect2.y = stRect.h + stRect.y;
        stRect2.x = 0;
        stRect2.w = DISP_WIDTH;
        stRect2.h = DISP_HEIGHT-stRect.y-stRect.h;
        HI_GO_FillRect(hLayerSurface, &stRect2, 0x00000000, HIGO_COMPOPT_NONE);
      }
      stBltOpt.EnableScale = HI_TRUE;
      ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
      HI_GO_RefreshLayer(hLayer, NULL);
        
      
    } else { // high screen
        tmp = (DISP_HEIGHT * (stImgInfo.Width)) / (stImgInfo.Height);
        stRect.x = (DISP_WIDTH - tmp) / 2;
        stRect.y = 0;
        stRect.w = tmp;
        stRect.h = DISP_HEIGHT;
        
        if(stRect.x!=0)
        	{
        stRect1.y = 0;
        stRect1.x = 0;
        stRect1.w = stRect.x;
        stRect1.h = DISP_HEIGHT;
        HI_GO_FillRect(hLayerSurface, &stRect1, 0x00000000, HIGO_COMPOPT_NONE);
        stRect2.y = 0;
        stRect2.x = stRect.w + stRect.x;
        stRect2.w = DISP_WIDTH-stRect.x-stRect.w;;
        stRect2.h = DISP_HEIGHT;
        HI_GO_FillRect(hLayerSurface, &stRect2, 0x00000000, HIGO_COMPOPT_NONE);
      }
       stBltOpt.EnableScale = HI_TRUE;
      ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
      HI_GO_RefreshLayer(hLayer, NULL); 
    }
    

	 
	 
	pthread_mutex_unlock(&mBufLock);
      
}

void SurfaceUI::lollipop_ui_showvideoimage(void)
{
	  HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;
    HI_HANDLE hDecSurface;
    HI_RECT stRect;
    HI_RECT stRect1,stRect2;
    HI_S32 ret         = HI_SUCCESS;
    int tmp;
    HI_HANDLE hDecoder = 0;
    
    HIGO_BLTOPT_S stBltOpt = {0};
    const char* filename="/etc/jpg/start.jpg";
    
	  
    
     
    
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = filename;
    
	pthread_mutex_lock(&mBufLock);
	
	ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
	ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
	if(stImgInfo.Width > 4095){
       stAttr.Width = 4095;
    }else{
       stAttr.Width = stImgInfo.Width;
    }
	
	 if(stImgInfo.Height > 4095){
        stAttr.Height = 4095;
    }else{
        stAttr.Height = stImgInfo.Height;
    }
	stAttr.Format = HIGO_PF_8888;
	//	if(stImgInfo.Width > 4095 || stImgInfo.Height > 4095){
		 //   printf("stImgInfo.Width > 4095\r\n");
    //    ret = HI_GO_DecImgData(hDecoder, 0, &stAttr, &hDecSurface);
   // }else{
        ret  = HI_GO_DecImgData(hDecoder, 0, NULL, &hDecSurface);
   // }
	//ret |= HI_GO_DestroyDecoder(hDecoder);
	//HI_GO_FillRect(hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
//	printf("deletSurface enter HI_GO_RefreshLayer enter\r\n");
	//   HI_GO_RefreshLayer(hLayer, NULL);
	if ((stImgInfo.Height) * DISP_WIDTH <= (stImgInfo.Width) * DISP_HEIGHT) { // long screen
        tmp = (DISP_WIDTH * (stImgInfo.Height)) / (stImgInfo.Width);
        stRect.x = 0;
        stRect.y = (DISP_HEIGHT - tmp) / 2;
        stRect.w = DISP_WIDTH;
        stRect.h = tmp;
        if(stRect.y!=0)
        	{
        stRect1.y = 0;
        stRect1.x = 0;
        stRect1.w = DISP_WIDTH;
        stRect1.h = stRect.y;
        HI_GO_FillRect(hLayerSurface, &stRect1, 0x00000000, HIGO_COMPOPT_NONE);
        stRect2.y = stRect.h + stRect.y;
        stRect2.x = 0;
        stRect2.w = DISP_WIDTH;
        stRect2.h = DISP_HEIGHT-stRect.y-stRect.h;
        HI_GO_FillRect(hLayerSurface, &stRect2, 0x00000000, HIGO_COMPOPT_NONE);
      }
      stBltOpt.EnableScale = HI_TRUE;
      ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
      HI_GO_RefreshLayer(hLayer, NULL);
        
      
    } else { // high screen
        tmp = (DISP_HEIGHT * (stImgInfo.Width)) / (stImgInfo.Height);
        stRect.x = (DISP_WIDTH - tmp) / 2;
        stRect.y = 0;
        stRect.w = tmp;
        stRect.h = DISP_HEIGHT;
        
        if(stRect.x!=0)
        	{
        stRect1.y = 0;
        stRect1.x = 0;
        stRect1.w = stRect.x;
        stRect1.h = DISP_HEIGHT;
        HI_GO_FillRect(hLayerSurface, &stRect1, 0x00000000, HIGO_COMPOPT_NONE);
        stRect2.y = 0;
        stRect2.x = stRect.w + stRect.x;
        stRect2.w = DISP_WIDTH-stRect.x-stRect.w;;
        stRect2.h = DISP_HEIGHT;
        HI_GO_FillRect(hLayerSurface, &stRect2, 0x00000000, HIGO_COMPOPT_NONE);
      }
       stBltOpt.EnableScale = HI_TRUE;
      ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
      HI_GO_FreeSurface(hDecSurface);
      HI_GO_RefreshLayer(hLayer, NULL); 
    }
    

	 
	 
	pthread_mutex_unlock(&mBufLock);
      
}

SurfaceUI::~SurfaceUI() {
	//pthread_mutex_lock(&mBufLock);
	//printf("~SurfaceUI enter\r\n");
	delete mTextFont;
	hLayerSurface=NULL;
	HI_GO_DestroyLayer(hLayer);
	hLayer=NULL;
	HI_GO_Deinit();
	///pthread_mutex_unlock(&mBufLock);
	
}