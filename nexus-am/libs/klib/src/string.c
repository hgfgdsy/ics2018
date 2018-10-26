#include "klib.h"

#ifndef __ISA_NATIVE__

size_t strlen(const char *s) {
  assert(s!=NULL);
  int sum=0;
  while(*s!='\0'){
	  sum++;
	  s++;}
	return sum;
}

char *strcpy(char* dst,const char* src) {
  assert((dst!=NULL)&&(src!=NULL));
  char *adr=dst;
  while((*dst++=*src++)!='\0')
	NULL;
  return adr;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char *adr=dst;
  assert((dst!=NULL)&&(src!=NULL));
  if(strlen(src)<n){
	  strcpy(dst,src);}
  else{
	  for(int i=0;i<n;i++){
		  *dst=*src;
		  dst++;
		  src++;}
	  *dst='\0';}
	return adr;
}

char* strcat(char* dst, const char* src) {
  char *adr=dst;
  assert((dst!=NULL)&&(src!=NULL));
  while(*dst!='\0'){
	  dst++;}
  while(*src!='\0'){
	  *dst=*src;
	  dst++;
	  src++;}
  *dst='\0';
  return adr;
}

int strcmp(const char* s1, const char* s2) {
  int k=0;
  assert((s1!=NULL)&&(s2!=NULL));
  while(!(k=*(unsigned char*)s1-*(unsigned char*)s2)&&*s1){
	  s1++;s2++;}
  if(k<0)
	  return -1;
  else if(k>0)
	  return 1;
	return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  assert((s1!=NULL)&&(s2!=NULL));
  int k=0;
  if((strlen(s1)<=n)||(strlen(s2)<=n))
	  k=strcmp(s1,s2);
  else{
	  for(int i=0;i<n;i++){
          if(*s1>*s2){
		  k=1;break;}
	  else if(*s1<*s2){k=-1;break;}
	  else{s1++;s2++;}}}
	return k;
}

void* memset(void* v,int c,size_t n) {
	void* ret=v;
	while(n--){
		*(char*)v=(char)c;
		v=(char*)v+1;}
	*(char*)v='\0';
	  return ret;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert((out!=NULL)&&(in!=NULL));
  char* bin=(char*)in;
  char* bout=(char*)out;
  while(n--){
	  *bout++=*bin++;}
	return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  int k=0;
  assert((s1!=NULL)&&(s2!=NULL));
  while((n--)&&(*(char*)s1==*(char*)s2)){
	  s1=(char*)s1+1;;s2=(char*)s2+1;}
  k=*(char*)s1-*(char*)s2;
  if(k<0)
	  return -1;
  else if(k>0)
	  return 1;
	return 0;
}

#endif
