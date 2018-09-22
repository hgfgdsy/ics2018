#include<stdio.h>
#include<string.h>
#include<monitor/expr.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  char buff[65535];
  char ah[50];
  FILE *fp=NULL;
  fp=fopen( "/home/hgfgdsy/ics2018/nemu/tools/gen-expr/input", "r");
  while(!feof(fp)){
	  memset(ah,'\0',sizeof(ah));
	  memset(buff,'\0',sizeof(buff));
	  fscanf(fp,"%s",ah);
	  fgets(buff,65535,(FILE*)fp);
	  bool k=1;
	  int z=expr(buff,&k);
	  if(!k){
		  printf("URE_E!\n");
	  }
	  else
		  printf("%d\n",z);
  }
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
