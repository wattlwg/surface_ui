#ifndef TEXT_FONT_H
#define TEXT_FONT_H

#include <stdio.h>

//#include <utils/Log.h>
//#include <utils/String8.h>
//#include <ui/DisplayInfo.h>

//#include <sys/stat.h>
//#include <setjmp.h>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include "string_map.h"
//#include "fillBuffer.h"

//extern "C" {
	//#include "list.h"
//}
#define offsetof_(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/*#define container_of(ptr, type, member) ( { \
        const typeof( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - offsetof_(type,member) ); } )
*/
/*#ifndef container_of
#define container_of(ptr,type,member) ({  \
        const typeof(((type *)0)->member) *__mptr = (ptr);    \
        (type *)((char*)__mptr - offsetof_( type , member)); } )
#endif
*/
extern "C" {
	#include "list.h"
}     
struct rgb_color {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
};

struct TextTag{
	      struct list_head  i_list;
        char	name[64];
	      char	subname[64];
	      char	*str;
        int	text_size;
        struct	rgb_color color;
        int	tag_x;
        int	tag_y;
        int	width;
        int	height;
        unsigned char * textBuf;
	      unsigned char * backgroundBuf;
	      int	bufSize;
};

class textFont {
	public:
		textFont(char * xml_path, char * fontPath);//HI_HANDLE htempFont);//
		~textFont();
	
		int show_text(char* name , char*subname, char* data, HI_HANDLE hLayerSurface,HI_HANDLE hLayer,unsigned char * displayBuf);
	private:
		int addTag(struct TextTag ** pTag);
		int getTagByName(char * name, struct TextTag ** pTag);
		int getStr(char* name , char* subname, char* data, struct TextTag *pTag);
		void clearTextTagList(void);

		StringMap* mStringmap;
		struct list_head mTagList;

    HI_HANDLE hFont;
		//char mFontPath[256];
		char mLanguage[64];
};

#endif  //TEXT_FONT_H
