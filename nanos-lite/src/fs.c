#include "fs.h"
#include "unistd.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void* buf, size_t offset, size_t len);
extern size_t serial_write(const void* buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);
extern size_t events_read(void* buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"/dev/events", 0, 0, 0, events_read, NULL},
  {"/proc/dispinfo", 128, 0, 0, dispinfo_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char* pathname, int flags, int mode);
int screen_width();
int screen_height();

void init_fs() {
	int width = screen_width();
	int height = screen_height();
	int fd=fs_open("/dev/fb", 0, 0);
	file_table[fd].size=width*height*4;

	// TODO: initialize the size of /dev/fb
}

size_t fs_filesz(int fd){
	return file_table[fd].size;
}


int fs_open(const char* pathname, int flags, int mode){
	for(int i=0;i<NR_FILES;i++){
		if(strcmp(file_table[i].name,pathname)==0){
			file_table[i].open_offset = 0;
			return i;}
	}
	printf("%s\n", pathname);
	assert(0);
	return 0;
}

ssize_t fs_read(int fd, void *buf, size_t len){
	if(file_table[fd].read==NULL){
	size_t fz=fs_filesz(fd);
	if(fz-file_table[fd].open_offset<len)
		len = fz - file_table[fd].open_offset;
	ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
	file_table[fd].open_offset += len;
	return len;}
	else{
		size_t l=file_table[fd].read(buf, file_table[fd].open_offset, len);
		file_table[fd].open_offset+=len;
		return l;
	}
}

ssize_t fs_write(int fd, const void *buf, size_t len){
	if(file_table[fd].write==NULL){
	size_t fz=fs_filesz(fd);
	if(fz-file_table[fd].open_offset<len)
		len = fz - file_table[fd].open_offset;
	ramdisk_write(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
	file_table[fd].open_offset += len;
	return len;}
	else{
		size_t l = file_table[fd].write(buf, file_table[fd].open_offset, len);
		file_table[fd].open_offset+=len;
		return l;
	}
}

off_t fs_lseek(int fd, off_t offset, int whence){
	size_t fz = fs_filesz(fd);
	switch(whence){
		case SEEK_SET: if(offset<=fz&&offset>=0){
				       file_table[fd].open_offset = offset;
				       return offset;
			       }
			       else{
				       return -1;}
			       break;
	       case SEEK_CUR: if(offset<=fz-file_table[fd].open_offset&&offset+file_table[fd].open_offset>=0){
				      file_table[fd].open_offset+=offset;
				      return file_table[fd].open_offset;}
			      else{
				      return -1;}
			      break;
	       case SEEK_END: if(offset<=0&&fz+offset>=0){
				     file_table[fd].open_offset = fz + offset;
				     return file_table[fd].open_offset;
			     }
			     else{
				     return -1;}
			     break;
	     default: return -1;
	}
}



int fs_close(int fd){
	return 0;
}


