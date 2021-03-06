#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
int label=0;
int token_num=0;
static void gen_num(){
       char s[11];
       unsigned temp=rand()%2000 +1;
       unsigned k=0;
       int count=0;
       if(temp==0){
	       count=1;
	       s[0]='0';}
       else{
       while(temp!=0){
         k=temp%10;
	 temp=(temp-k)/10;
         s[count]=(unsigned char)k+'0';
	 count++;
       }}
       token_num+=count;
             for(int i=count-1;i>=0;i--){
			 buf[label++]=s[i];}
       buf[label++]='u';       
}

static void gen_space(){
	buf[label++]=' ';}

static int choose(int a){
	int z=3;
	if(a>1000)
		z=a/1000;
       return rand()%z +0;}
static inline void gen_rand_op(){
       int what=rand()%4 +1;
       switch(what){
	       case 1: buf[label++]='+';break;
	       case 2: buf[label++]='-';break;
	       case 3: buf[label++]='*';break;
	       case 4: buf[label++]='/';break;
	       }
       token_num+=1;
}

static void gen_rl(char c){
	buf[label++]=c;
	token_num+=1;}

static inline void gen_rand_expr() {
  switch(choose(label)){
          case 0:gen_rl('(');gen_rand_expr();gen_space();gen_rl(')');break;
          case 1:gen_rand_expr();gen_rand_op();gen_rand_expr();break;
          default:gen_space();gen_num();break;
}
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    label=0;
    token_num=0;
    buf[0]='\0';
	  gen_rand_expr();
	  while(token_num>300){
		  label=0;
		  token_num=0;
		  buf[0]='\0';
		  gen_rand_expr();}
	  buf[label]='\0';
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    if(fscanf(fp, "%d", &result)==1){
    pclose(fp);

    printf("%u %s\n", result, buf);}
    else{
	    pclose(fp);}
  }
  return 0;
}
