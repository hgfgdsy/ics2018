#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      int k=inl(0x100);
      info->width = (k>>16)&0xffff;
      info->height = k&0xffff;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
      
      /*int i;
      int size=screen_width() * screen_height();
      for (i=0;i<size;i++) fb[i]=i;  */

      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      else{
	      
	      int z=(ctl->y*screen_width())+ctl->x;
	      int k=0;
	      for(int i=0;i<ctl->h;i++){
	      for(int j=0;j<ctl->w;j++){
	      fb[z+j]=*(ctl->pixels+k++);
	      }
	      z+=screen_width();}
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
	memset((void*)0x40000,0,0x40000);
}
