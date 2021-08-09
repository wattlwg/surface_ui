#############################################################################
## Customizable Section: adapt those variables to suit your program.
##==========================================================================
include ../config.mak
include $(SDK_DIR)/base.mak

RPATH            := ../..
GROUPNAME        :=
PROJECTNAME      := libsurface_ui
INCLUDEDIR       := -I../../../../framecore/poco/linux/${ARCHTAG}/include \
					-I../../../../include \
					-I../../../../common

ifeq (YES, ${USE_HS_SDK})
INCLUDEDIR += -I$(HI_INCLUDE_DIR)
INCLUDEDIR += -I$(MSP_DIR)/api/higo/include
INCLUDEDIR += -I$(SDK_DIR)/sample/common/
LDFLAGS := -L$(HI_SHARED_LIB_DIR) -L$(HI_EXTERN_LIB_DIR) -L$(HI_LIBS) -DUSE_HS_SDK -leffect -lhigo -lhi_charsetMgr -lplayer -lsubdec -lhiplayer_utils -lhi_common -lhi_msp -lfreetype -lharfbuzz -lhi_so -ljpeg -lhigo -lhigoadp
CODE_SRC  = $(SDK_DIR)/sample/surface_ui/surface_ui.cpp  $(SDK_DIR)/sample/surface_ui/textFont.cpp $(SDK_DIR)/sample/surface_ui/string_map.cpp



endif

LDFLAGS += -L../../../../framecore/poco/linux/${ARCHTAG}/lib -L../../../../lib/linux/${ARCHTAG} -L../../../../third/mdns/linux/bin/${ARCHTAG} -L../../../../third/airplay/linux/lib/${ARCHTAG} \
 -lstdc++ -lpthread -lm -lrt -ldl -L$(HI_LIBS) -leffect

OUTPUT   = $(SDK_DIR)/out/hi3716mv430/hi3716mv430_dmd_dongle_loader/lib/share/${PROJECTNAME}.so

SHELL   = /bin/sh
all :
	$(CC) -g -rdynamic -funwind-tables $(CODE_SRC) $(INCLUDEDIR) $(LDFLAGS) $(CXXFLAGS) -shared -fPIC -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
