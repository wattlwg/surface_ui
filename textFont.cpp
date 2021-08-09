/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
//#define LOG_TAG "textFont"
//#include "cutils/log.h"
//#include "language.h"
#include "textFont.h"

#include "hi_go.h"
//#include "imageDecode.h"
//#include "fillBuffer.h"

#define DBG 0

int str_startsWith(char * str, char * search_str) {
    if ((str == NULL) || (search_str == NULL)) return 0;
    return (strstr(str, search_str) == str);
}


int textFont::addTag(struct TextTag ** pTag) {

	struct list_head * pList;

	if (pTag == NULL) return -1;

	if(DBG) printf("addTag, pTag->textBuf=0x%x\n", (*pTag)->textBuf);
	if(DBG) printf("addTag, pTag->width=%d\n", (*pTag)->width);
	if(DBG) printf("addTag, pTag->height=%d\n", (*pTag)->height);

	list_for_each(pList, &mTagList) {
		if(pList == NULL) break;
		struct TextTag * tag = list_entry(pList, struct TextTag, i_list);
		if(tag == NULL) break;
		if(!strcmp(tag->name, (*pTag)->name)) {

			list_del(pList);

			if (tag->str != NULL) {
				printf("free tag->str enter\r\n");
				free(tag->str);
				tag->str = NULL;
				printf("free tag->str exit\r\n");
			}

			if (tag->textBuf != NULL) {
				printf("free tag->textBuf enter\r\n");
				free(tag->textBuf);
				printf("free tag->textBuf exit\r\n");
				tag->textBuf = NULL;
			}

			if (tag->backgroundBuf != NULL) {
				printf("free tag->backgroundBuf enter\r\n");
				free(tag->backgroundBuf);
				tag->backgroundBuf = NULL;
				printf("free tag->backgroundBuf exit\r\n");
			}

			if (tag != NULL) {
				printf("free tag enter\r\n");
				free(tag);
				printf("free tag exit\r\n");
			}

			break;
		}
	}

        list_add_tail(&(*pTag)->i_list, &mTagList);

        return 0;
}

int textFont::getTagByName(char * name, struct TextTag ** pTag) {

	struct list_head * pList;

	if (pTag == NULL) return -1;
	if (*pTag == NULL) return -1;

	list_for_each(pList, &mTagList) {
		if(pList == NULL) break;
		struct TextTag * tag = list_entry(pList, struct TextTag, i_list);
		if(tag == NULL) break;

		if(!strcmp(tag->name, name)) {
			if(DBG) printf("found (%s), buf=0x%x\n", name, tag->textBuf);
			*pTag = tag;
			return 0;
		}
	}

	return -1;
}

int textFont::getStr(char* name , char* subname, char* data, struct TextTag *pTag) {
	if(DBG) printf("getStr\n");
	if(DBG) printf("name:%s\n", name);
	if(DBG) {
		if (subname != NULL) printf("subname:%s\n", subname);
	}

	if(DBG) {
		if (data != NULL) printf("data:%s\n", data);
	}

	char result[128];
	char value[256];
	char* tempname = NULL;
	char* tempsubname = NULL;
	char* color = NULL;
	char* location = NULL;
	char* size = NULL;
	char *temp_size = NULL;
	char *temp_color = NULL;
	char *temp_location = NULL ;
	char *temp_name = NULL;
	char *temp_subname = NULL;

	pTag->height = 0;
	pTag->width = 0;	
	
	if (pTag == NULL) {
		printf("pTag is NULL\n");
		return -1;
	}

	if(mStringmap->getText(name, subname, result) == true ) {
		mStringmap->getValue(name, subname, value);
		tempname = strtok(result," ");
		char * p = NULL;
		while(1) {
			p = strtok(NULL, " ");
			if (p == NULL) break;
			if(DBG) printf("p=%s\n", p);
			if(str_startsWith(p, (char *)"subname=")) tempsubname = p;
			else if(str_startsWith(p, (char *)"size=")) size = p;
			else if(str_startsWith(p, (char *)"color=")) color = p;
			else if(str_startsWith(p, (char *)"location=")) location = p;
		}
		/***************************************/
		if (size != NULL) {
			temp_size = strtok(size,"=");
			temp_size = strtok(NULL,"=");	
			pTag->text_size = atoi(temp_size);
		}

		if (color != NULL) {
			temp_color = strtok(color,"=");
			temp_color = strtok(NULL,":");
			pTag->color.red =  atoi(temp_color);
			temp_color = strtok(NULL,":");
			pTag->color.green =  atoi(temp_color);
			temp_color = strtok(NULL,":");
			pTag->color.blue= atoi(temp_color);
		}

		if (location != NULL) {
			temp_location = strtok(location,"=");
			temp_location = strtok(NULL,":");
			pTag->tag_x =  atoi(temp_location);
			temp_location = strtok(NULL,":");
			pTag->tag_y= atoi(temp_location);
		}

		strcpy(pTag->name, name);
		if (subname != NULL) {
			strcpy(pTag->subname, subname);
		}

		if (pTag->str != NULL) {
			free(pTag->str);
		}

		if (data != NULL) {
			pTag->str = (char *)malloc(strlen(data)+1);
			strcpy(pTag->str, data);
			if(DBG) printf("data=%s\n", data);
			return strlen(data);
		} else if(strcmp(value,"NONE") == 0) {
                        if(DBG) printf("value = NONE : show nothing\n");
			return -1;
		} else {
			pTag->str = (char *)malloc(strlen(value)+1);
			if(DBG) printf("value: %s\n", value);
			strcpy(pTag->str, value);
			return strlen(value);
		}

	} else {
		printf("error: string(name=%s, subname=%s) not found in xml file\n", name, subname);
		return -1;
	}

	return -1;
}

int textFont::show_text(char* name , char* subname, char* data, HI_HANDLE hLayerSurface,HI_HANDLE hLayer,unsigned char * displayBuf) {

	struct TextTag * pTag = (struct TextTag *)malloc(sizeof(struct TextTag));
	struct TextTag * pOldTag;
  HI_S32 s32Ret = 0;
  HI_RECT rc = {0};
  HI_U32 color=0;
  
	pTag->str = NULL;
	pTag->textBuf = NULL;
	pTag->backgroundBuf = NULL;

	if(DBG) printf("*****show_text: name: %s\n", name);

	if (subname != NULL) {
		if(DBG) printf("show_text: subname: %s\n", subname);
	}

	if (data != NULL) {
		if(DBG) printf("show_text: data: %s\n", data);
	}


	if (getStr(name, subname, data, pTag) < 0) {
		free(pTag);
		return -1;
	}
	
	if(pTag->str!=NULL && strlen(pTag->str)>0) {
		printf("------show_text: string: %s\n", pTag->str);
		if(DBG) printf("show_text: strlen(pTag->str)=%d\n", strlen(pTag->str));
		if(DBG) printf("show_text: size=%d\n", pTag->text_size);
    //printf("------show_text: str[0]: %x\n", pTag->str[0]);
  //  printf("------show_text: str[1]: %x\n", pTag->str[1]);
   // printf("------show_text: str[2]: %x\n", pTag->str[2]);
		/*read_text(pTag->text_size,
				pTag->str,
				&(pTag->textBuf),
				&(pTag->bufSize),
				&(pTag->width),
				&(pTag->height),
				mFontPath);*/
    //s32Ret = HI_GO_CreateText(NULL, "/etc/fonts/csl.ttf", &hFont);
	//	printf("width=%d, height=%d\n", pTag->width, pTag->height);
		//printf("pTag->color.red==%d, pTag->color.green=%d,pTag->color.blue=%d\n", pTag->color.red, pTag->color.green,pTag->color.blue);
		//color=pTag->color.red+pTag->color.green<<8+pTag->color.blue<<16+0xff000000;
		//color=pTag->color.blue+pTag->color.red<<8+pTag->color.green<<16+0xff000000;
		color=0xff000000;
		color+=pTag->color.blue;
		color+=pTag->color.red*256*256;
		color+=pTag->color.green*256;
		//color=106+191<<8+227<<16+0xff000000;
		//printf("color==%x\n",color);
		// color=0xff000000;//pTag->color.blue+pTag->color.red<<16+pTag->color.green<<8;
		 s32Ret = HI_GO_SetTextColor(hFont, color);//0xff0000ff);
     
    rc.x = pTag->tag_x;
    rc.y = pTag->tag_y;
    rc.w = 400;//pTag->width;
    rc.h = 200;//pTag->height;
   // printf("HI_GO_TextOut enter\r\n");
    s32Ret = HI_GO_TextOut(hFont, hLayerSurface, pTag->str, &rc);
   // HI_GO_RefreshLayer(hLayer, NULL);
  //  HI_GO_DestroyText(hFont);
	}

	/*if (0 == getTagByName(pTag->name, &pOldTag)) {
		if(DBG) printf("found buffer for (%s)\n", pTag->name);
		if (pOldTag->textBuf != NULL) {
			if(DBG) printf("clear buffer for (%s)...\n",	pOldTag->name);
			if(DBG) printf("pOldTag->textBuf=0x%x\n",		pOldTag->textBuf);
			if(DBG) printf("pOldTag->p->bufSize=%d\n",		pOldTag->bufSize);
			if(DBG) printf("pOldTag->width=%d\n\n",		pOldTag->width);
			if(DBG) printf("pOldTag->height=%d\n\n",		pOldTag->height);

			memset(pOldTag->textBuf, 0, pOldTag->bufSize);
			pOldTag->color.red = 0;
			pOldTag->color.green = 0;
			pOldTag->color.blue = 0;
		}

		//recover background
		for (int y = 0; y < pOldTag->height; y++) {
			memcpy(displayBuf + (4 * ((y + pOldTag->tag_y) * DISP_WIDTH + pOldTag->tag_x)),
				pOldTag->backgroundBuf + (y * pOldTag->width * 4),
				pOldTag->width * 4);
		}
	}

	if (pTag->textBuf != NULL) {
		//save background
		pTag->backgroundBuf = (unsigned char *)malloc(pTag->width * pTag->height * 4);
		for (int y = 0; y < pTag->height; y++) {
			memcpy(pTag->backgroundBuf + (y * pTag->width * 4),
				displayBuf + (4 * ((y + pTag->tag_y) * DISP_WIDTH + pTag->tag_x)),
				pTag->width * 4);
		}

     s32Ret = HI_GO_SetTextColor(hFont, 0xffff0000);
     
    rc.x = pTag->tag_x;
    rc.y = pTag->tag_y;
    rc.w = 400;//pTag->width;
    rc.h = 30;//pTag->height;
    printf("HI_GO_TextOut enter\r\n");
    s32Ret = HI_GO_TextOut(hFont, hLayerSurface, pTag->textBuf, &rc);
		//fill new buf
	
	}*/

	addTag(&pTag);
	return 0;
}

void textFont::clearTextTagList(void) {
	struct list_head *pList;
	struct list_head *pListTemp;

	list_for_each_safe(pList, pListTemp, &mTagList) {
		struct TextTag * tag = list_entry(pList, struct TextTag, i_list);
		list_del(pList);
		if (tag) {
			if (tag->str != NULL) free(tag->str);
			if (tag->textBuf != NULL) free(tag->textBuf);
			if (tag->backgroundBuf != NULL) free(tag->backgroundBuf);
			free(tag);
		}
	}
}


textFont::textFont(char * xml_path, char * font_path) //HI_HANDLE htempFont)//
	{
	HI_S32 ret;
	HI_RECT rc = {0};
	//HI_CHAR szText[] = "中国人民";
//	strcpy(mFontPath, font_path);

  
  ret = HI_GO_CreateText(NULL, font_path, &hFont);//"/etc/fonts/higo_gb2312.ubf"
   if (HI_SUCCESS !=ret)
    {
       //goto ERR3;
       printf("HI_GO_CreateText create error\r\n");
    }
    else
    	printf("HI_GO_CreateText create success\r\n");
  // hFont=htempFont; 	
  
 /* ret = HI_GO_SetTextColor(hFont, 0xffff0000);
 // ret = HI_GO_SetTextStyle(hFont, HIGO_TEXT_STYLE_ITALIC|HIGO_TEXT_STYLE_BOLD);   
    rc.x = 35;
    rc.y = 70;
    rc.w = 400;//pTag->width;
    rc.h = 30;//pTag->height;
    printf("HI_GO_TextOut enter\r\n");
    //ret = HI_GO_DrawRect(hLayerSurface, &rc, 0xffffffff);
    ret = HI_GO_TextOut(hFont, hLayerSurface, szText, &rc);*/
	mStringmap= new StringMap(xml_path);
	INIT_LIST_HEAD(&mTagList);
}

textFont::~textFont() {
 	//clearTextTagList();
        delete mStringmap;
        HI_GO_DestroyText(hFont);
        //HI_GO_DeinitText();
}

       
