#include "string_map.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define LOG_TAG "string_map"
//#include "cutils/log.h"

#define DBG 0

StringMap::StringMap(const char * file):
m_size(0)
{
	mListHead = NULL;
	mCurIndex = NULL;
	readFromXml(file);
}

StringMap::~StringMap() {

}

void StringMap::readFromXml(const char* file)
{
	char line[512];
	char text[256];
	char name[128];
	char subname[128];
	char value[256];
	char * temp;

	FILE *fp = fopen(file, "r");
	if (NULL == fp) return ;
    
	while (! feof(fp)) {
		memset(line, 0, 512);
		if (fgets(line, sizeof line, fp) == NULL) continue;
		memset(text, 0, 256);
		memset(name, 0, 128);
		memset(subname, 0, 128);
		memset(value, 0, 256);
	
		sscanf(line,"%*[^<]<string%*[^\"]\"\%[^\"]", text);       
		if(DBG) printf("text=%s\n", text);

		temp = strstr(line, "name=");
		if(DBG) printf("temp=%s\n", temp);
		if(temp != NULL) {
			sscanf(temp, "name=%[^\" \"]\n", name);
		} else {
			if (strlen(text)) {
				printf("no string name found\n");
			}
			continue;
		}

		temp = strstr(line, "subname=");
		if(temp != NULL) sscanf(temp, "subname=%[^\" \"]", subname);

		sscanf(line,"%*[^<]<string%*[^>]>\%[^<]",value);

		if(DBG) printf("StringMap::readFromXml name=%s \n",name);
		if(DBG) printf("StringMap::readFromXml subname=%s \n",subname);
		if(DBG) printf("StringMap::readFromXml value=%s\n",value);
//		printf("StringMap::readFromXml value[0]=%x\n",value[0]);	
		if(DBG) printf("StringMap::readFromXml text=%s\n",text);

		if(strlen(name)>0) {
			insert(name, subname, text, value);
		}
	}
	fclose(fp);
}

bool StringMap::insert( const char*name, const char * subname, const char * text, const char*value)
{
	if(DBG) printf("####### insert(%d): name=%s; subname=%s\n", m_size, name, subname);
	if(DBG) printf("####### insert value = %s \n",value);

	if(NULL==mCurIndex){
		mListHead = new struct str_list;
		mCurIndex = mListHead;
		mCurIndex->pNext = NULL;
		m_size = 1;
	}else{
		mCurIndex->pNext = new struct str_list;
		if(NULL==mCurIndex->pNext){
			return false;
		}
		mCurIndex = mCurIndex->pNext;		
		mCurIndex->pNext = NULL;
		m_size++;
	}

	sprintf(mCurIndex->name,"%s",name);
	if((subname!=NULL) && (strlen(subname)>0)) {
		strcpy(mCurIndex->subname, subname);
	}
	sprintf(mCurIndex->value,"%s",value);
	sprintf(mCurIndex->text,"%s",text);
	return true;
}


//bool StringMap::remove( const char*name )
//{
//  m_Map.erase( index );
//  return true;
//}


//bool StringMap::isEmpty()
//{
//    return m_Map.empty();
//}


//bool StringMap::clear()
//{
//    m_Map.clear();
//    return true;
//}

bool StringMap::getValue(const char* name, const char*subname, char *result)
{
	if(DBG) printf("getValue: name = %s, subname = %s\n",name, subname);
	struct str_list* p = mListHead;
	while(NULL!=p){
		if(0==strcmp(p->name,name)){
			if(DBG) printf("p->name = %s, p->subname = %s\n", p->name, p->subname);
			if (subname == NULL) {
				sprintf(result,"%s",p->value);			
				if(DBG) printf("StringMap::getValue  p=%s\n", p->value);
            			return true;
			} else if(0 == strcmp(p->subname, subname)) {
				sprintf(result,"%s",p->value);			
				if(DBG) printf("StringMap::getValue  p=%s\n", p->value);
            			return true;
			}
        	}
		p = p->pNext;
	}
	return false;
}

bool StringMap::getText(const char* name, const char * subname, char *result)
{
	if(DBG) printf("getText: name = %s, subname = %s\n",name, subname);
	struct str_list* p = mListHead;
	while(NULL!=p){
		if(DBG) printf("p->name = %s, p->subname = %s\n", p->name, p->subname);
        	if(0==strcmp(p->name,name)) {
			if(subname == NULL) {
				sprintf(result,"%s",p->text);			
            			return true;

			} else if(0==strcmp(p->subname, subname)) {
				sprintf(result,"%s",p->text);			
            			return true;
			}
        	}
		p = p->pNext;
    	}

	return false;
}

int StringMap::size()
{
    return m_size;
}
