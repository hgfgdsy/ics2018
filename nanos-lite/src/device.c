#include "common.h"
#include <amdev.h>
#include "unistd.h"

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i=0;i<len;i++){
      _putc(*(char*)(buf+i));
  }  
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

void draw_rect(uint32_t *pixels, int x, int y, int w, int h);
int screen_width();
int screen_height();
ssize_t fs_write(int fd, const void *buf, size_t len);
int fs_open(const char *pathname, int flags, int mode);

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int i=0;
  for(i=0;1;i++){
	  if(dispinfo[i]=='\0') break;
  }
  len=i+1;
  memcpy(buf,(void*)(&dispinfo[0]+offset),len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int width = screen_width();
  int height = screen_height();
  int x = offset%width;
  int y = offset/width;
  draw_rect((uint32_t*)buf, x, y, width, height);   
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  int w = screen_width();
  int h = screen_height();
  char width[20],height[20];
  int a=0,b=0;
  if(w==0) assert(0);
  while(w>0){
	  width[a++]=((w%10)+'0');
	  w/=10;
  }
  while(h>0){
	  height[b++]=((h%10)+'0');
	  h/=10;
  }
  int label=0;
  dispinfo[label++]='W';
  dispinfo[label++]='I';
  dispinfo[label++]='D';
  dispinfo[label++]='T';
  dispinfo[label++]='H';
  dispinfo[label++]=':';
  for(int i=w-1;i>=0;i--){
	  dispinfo[label++] = width[i];
  }
  dispinfo[label++] = '\n';
  dispinfo[label++]='H';
  dispinfo[label++]='E';
  dispinfo[label++]='I';
  dispinfo[label++]='G';
  dispinfo[label++]='H';
  dispinfo[label++]='T';
  dispinfo[label++]=':';
  for(int i=h-1;i>=0;i--){
	  dispinfo[label++] = height[i];
  }
  dispinfo[label]='\0';
  int fd=fs_open("/proc/dispinfo", 0, 0);
  fs_write(fd,(void*)dispinfo,label+1);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
