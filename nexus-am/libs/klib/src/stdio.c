#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  int sum=0;
  va_start(ap,fmt);
  while(*fmt!='\0'){
	  if(*fmt=='%'){
		  fmt++;
		  if(*fmt=='d'){fmt++;
			  int k=va_arg(ap,int);
			  char a[40];int label=0;int S;
			  if(k>0){
				  while(k>0){
					  a[label++]=(k%10)+'0';
					  k=k/10;}
				  S=0;}
			  else if(k==0){a[label++]='0';S=0;}
			  else if(k<0){while(k<0){a[label++]=(-(k%10))+'0';k/=10;}S=1;}
			  if(S==1){*out='-';out++;sum++;}
			  for(int i=label-1;i>=0;i--){
				  *out=a[i];sum++;
				  out++;}
			  }
                   else if(*fmt=='s'){fmt++;
			   char *src=va_arg(ap,char*);
			   while(*src!='\0'){
				   *out=*src;sum++;
				   out++;src++;}
			   }
	  }
	  else{
		  *out=*fmt;sum++;
		  out++;fmt++;}
  }
  va_end(ap);
	
			  return sum;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
