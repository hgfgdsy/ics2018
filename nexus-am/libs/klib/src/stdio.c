#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
   int sum=0;
   va_list ap;
   va_start(ap,fmt);
	while(*fmt!='\0'){
	  if(*fmt=='%'){
		  fmt++;int n=0;
		  if(*fmt=='0'){
			  fmt++;
			  while(('0'<=*fmt)&&(*fmt<='9')){
				  n=n*10+(*fmt-'0');fmt++;}}
		  switch(*fmt){
			  case 'u':fmt++;unsigned int k=va_arg(ap,unsigned int);char a[40];int label=0;
				   if(k==0) a[label++]='0';
				   else
				   while(k!=0){
					   a[label++]=(k%10)+'0';k/=10;}
				   int cnt=n-label;
				   if(cnt>0)
					   while(cnt--){
						   _putc('0');sum++;}
				   for(int i=label-1;i>=0;i--){_putc(a[i]);sum++;}break;
			  case 'd':{fmt++;
			  int k=va_arg(ap,int);
			  char a[40];int label=0;int S;
			  if(k>0){
				  while(k>0){
					  a[label++]=(k%10)+'0';
					  k=k/10;}
				  S=0;}
			  else if(k==0){a[label++]='0';S=0;}
			  else if(k<0){while(k<0){a[label++]=(-(k%10))+'0';k/=10;}S=1;}
			  int cnt=0;
			  if(S==1){_putc('-');sum++;cnt=n-label-1;}
			  else cnt=n-label;
			  if(cnt>0)
				  while(cnt--){_putc('0');sum++;}

			  for(int i=label-1;i>=0;i--){
				  _putc(a[i]);sum++;}break;
			  }
			  case 's':{fmt++;
			   char *src=va_arg(ap,char*);
			   while(*src!='\0'){
				   _putc(*src);sum++;src++;}break;
			   }
			 // default:assert(0);
	  }
		  
	  }
	  else{
		  _putc(*fmt);fmt++;sum++;}
	  
  }

	return sum;
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
		  switch(*fmt){
			  case 'd':{fmt++;
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
				  out++;}break;
			  }
			  case 's':{fmt++;
			   char *src=va_arg(ap,char*);
			   while(*src!='\0'){
				   *out=*src;sum++;
				   out++;src++;}break;
			   }
	  }
	  }
	  else{
		  *out=*fmt;sum++;
		  out++;fmt++;}
	  
  }
  *out='\0';
        /* int sum=0; 
	 va_list ap;
	 va_start(ap,fmt);
	 while(*fmt!='\0'){
		 if(*fmt=='%'){
			 fmt++;
			 if(*fmt=='d'){fmt++;
				 char a[40];int label=0;
				 int k=va_arg(ap,int);int S;
				 if(k>0){
					 while(k>0){
						 a[label++]=(k%10)+'0';k/=10;}S=0;}
				 else if(k==0){a[label++]='0';S=0;}
				 else if(k<0){while(k<0){a[label++]=(-(k%10))+'0';k/=10;}S=1;}
				 if(S==1){
					 *out='-';sum++;
					 out++;}
				 for(int i=label-1;i>=0;i--){
					 *out=a[i];
					 out++;sum++;}}
                           else if(*fmt=='s'){fmt++;
			   char *src=va_arg(ap,char*);
			   while(*src!='\0'){
				   *out=*src;sum++;
				   out++;src++;}
			   }
	  }

		 else{*out=*fmt;out++;fmt++;sum++;}}
		 va_end(ap);*/
			  return sum;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
