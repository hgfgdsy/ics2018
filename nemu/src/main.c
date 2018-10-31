#include<stdio.h>
#include<string.h>
#include<monitor/expr.h>
#include<monitor/watchpoint.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
/*  char buff[65535];
  char ah[50];
  FILE *fp=NULL;
  fp=fopen( "/home/hgfgdsy/ics2018/nemu/tools/gen-expr/input", "r");
  while(fscanf(fp,"%s",ah)==1){
	  memset(ah,'\0',sizeof(ah));
	  memset(buff,'\0',sizeof(buff));
	  fgets(buff,65535,(FILE*)fp);
	  buff[strlen(buff)-1]='\0';
	  bool k=1;
	  unsigned z=expr(buff,&k);
	  if(!k){
		  printf("URE_E!\n");
	  }
	  else
		  printf("%u\n",z);
	  k=1;
  }*/
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
