#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256,NUM = 255, HEX=254,REG=253,TK_EQ=252,TK_UEQ=251,TK_AND=250

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" +", TK_NOTYPE},    //spaces 
  {")", ')'},          //right
  {"\\(", '('},          //left
  {"\\$[a-zA-Z]+",REG},    //register
  {"0[Xx][0-9a-fA-F]+",HEX},  //HEX
  {"[0-9]+", NUM},  //number
  {"\\u", 'u'},     //unsigned
  {"\\+", '+'},         // plus
  {"-", '-'},          //minus
  {"\\*", '*'},         //mul
  {"/", '/'},          // div
  {"==", TK_EQ},         // equal
  {"!=", TK_UEQ},       //unequal
  {"&&", TK_AND}      //and
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[100];
} Token;

Token tokens[310];
int nr_token;

bool detect=1;
static bool check_parentheses(int p,int q,bool* assit){
	int count=0;
	bool temp=1;
	for(int i=p;i<=q;i++){
		if(tokens[i].type=='(')
			count++;
		if(tokens[i].type==')')
			count--;
		if(count==-1){
			temp=0;
			*assit=0;
		        break;}
		if(count==0&&i!=q){
			temp=0;
			*assit=1;}
		if(count!=0&&i==q){
			temp=0;
			*assit=0;}
	}
	return temp;}

static int eval(int p,int q){
	bool as=1;
	if(p>q){
	        if(tokens[p].type!='-')	
		  detect=0;
		return 0;
	}
	else if(p==q){
		if(tokens[p].type==255){
			int n=0;
			int len=strlen(tokens[p].str);
			for(int j=0;j<len;j++)
				n=n*10+(tokens[p].str[j]-'0');
			return n;
		}
		if(tokens[p].type==254){
			int n=0;
			int len=strlen(tokens[p].str);
			for(int j=0;j<len;j++){
				if(tokens[p].str[j]>='0'&&tokens[p].str[j]<='9')
					n=n*16+(tokens[p].str[j]-'0');
                                if(tokens[p].str[j]>='a'&&tokens[p].str[j]<='f')
					n=n*16+(tokens[p].str[j]-'a'+10);
                                if(tokens[p].str[j]>='A'&&tokens[p].str[j]<='F')
					n=n*16+(tokens[p].str[j]-'A'+10);
			}
			return n;}
                if(tokens[p].type==253){
			int n=0;
			if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='a'||tokens[p].str[1]=='A')&&(tokens[p].str[2]=='x'||tokens[p].str[2]=='X'))
		        n=cpu.eax;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='c'||tokens[p].str[1]=='C')&&(tokens[p].str[2]=='x'||tokens[p].str[2]=='X'))
	                n=cpu.ecx;   
	                if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='d'||tokens[p].str[1]=='D')&&(tokens[p].str[2]=='x'||tokens[p].str[2]=='X'))
		        n=cpu.edx;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='b'||tokens[p].str[1]=='B')&&(tokens[p].str[2]=='x'||tokens[p].str[2]=='X'))
		        n=cpu.ebx;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='s'||tokens[p].str[1]=='S')&&(tokens[p].str[2]=='s'||tokens[p].str[2]=='P'))
		        n=cpu.esp;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='b'||tokens[p].str[1]=='B')&&(tokens[p].str[2]=='p'||tokens[p].str[2]=='P'))
		        n=cpu.ebp;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='s'||tokens[p].str[1]=='S')&&(tokens[p].str[2]=='i'||tokens[p].str[2]=='I'))
		        n=cpu.esi;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='d'||tokens[p].str[1]=='D')&&(tokens[p].str[2]=='i'||tokens[p].str[2]=='I'))
		        n=cpu.edi;
                        if((tokens[p].str[0]=='e'||tokens[p].str[0]=='E')&&(tokens[p].str[1]=='i'||tokens[p].str[1]=='I')&&(tokens[p].str[2]=='p'||tokens[p].str[2]=='P'))
		        n=cpu.eip;

		       return n;}	
			detect=0;
			return 0;
	}
	else if(check_parentheses(p,q,&as)==true){
		return eval(p+1,q-1);}
	else{
		if(as==1){
			if(tokens[p].type==249){
			     int d=eval(p+1,q);
			     int n=pmem[d];
			     return n;
			}
			else{
			int count1=0;
			int T=0;
			int op=0;
			for(int j=p;j<=q;j++){
				if(tokens[j].type=='('){
					count1++;
					continue;}
				if(tokens[j].type==')'){
					count1--;
					continue;}
				if(tokens[j].type!=255&&count1==0&&tokens[j].type!=254&&tokens[j].type!=253&&tokens[j].type!=249){
						if(T==0||T==250){
							T=tokens[j].type;op=j;continue;}
					        if((T==251||T==252)&&tokens[j].type!=0&&tokens[j].type!=250){
							T=tokens[j].type;op=j;continue;}
						if((T=='*'||T=='/')&&tokens[j].type!=0&&tokens[j].type!=250&&tokens[j].type!=251&&tokens[j].type!=252)
						{T=tokens[j].type;op=j;continue;}
						if(tokens[j].type=='+'||tokens[j].type=='-'){
							T=tokens[j].type;op=j;continue;}
				                      }
			
			}
			int k=op;
			if(T=='-'){
				while(tokens[k].type=='-'&&k>=p+1){
					k--;}
				if(tokens[k].type=='+'||tokens[k].type=='-'||tokens[k].type=='*'||tokens[k].type=='/'){
					op=k;}
				else{
					op=k+1;}
			}
			int val1=eval(p,op-1);
			int val2=eval(op+1,q);
			switch(tokens[op].type){
				case 250: if(val1!=0&&val2!=0)
						  return 1;
					  else
						  return 0;
				case 251: return val1!=val2;
				case 252: return val1==val2;
				case '+': return val1+val2;
                                case '-': return val1-val2;
				case '*': return val1*val2;
				case '/': if(val2==0){
						  detect=0;
						  return 0;}
					  else{
						  return val1/val2;}
				default: assert(0);
			}
			}
		}
		else{
			detect=0;
			return 0;}
		}
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        /*Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);*/
        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
		case 250:
			 tokens[nr_token].type=250;nr_token++;break;
		case 251:
			 tokens[nr_token].type=251;nr_token++;break;
		case 252:
			 tokens[nr_token].type=252;nr_token++;break;
		case 253:
			 tokens[nr_token].type=253;
			 tokens[nr_token].str[0]=*(substr_start+1);
                         tokens[nr_token].str[1]=*(substr_start+2);
                         tokens[nr_token].str[2]=*(substr_start+3);
                         tokens[nr_token].str[3]='\0';
                         nr_token++;break;
		case 254:
			 tokens[nr_token].type=254;
			 for(int j=0;j<substr_len-2;j++)
			        tokens[nr_token].str[j]=*(substr_start+j+2);
		          tokens[nr_token].str[substr_len-2]='\0';	 
				nr_token++;break;

		case 255: 
			 tokens[nr_token].type=255;
		        	for(int j=0;j<substr_len;j++)
			        	tokens[nr_token].str[j]=*(substr_start+j);
		                           tokens[nr_token].str[substr_len]='\0';
		                           nr_token++;break;
	        case '+':
	                 tokens[nr_token].type='+';nr_token++;break;
		case '-':
			 tokens[nr_token].type='-';nr_token++;break;
		case '*':
			 tokens[nr_token].type='*';nr_token++;break;
	        case '/':
			 tokens[nr_token].type='/';nr_token++;break;
		case 256:
			 break;
		case '(':
			 tokens[nr_token].type='(';nr_token++;break;
		case ')':
			 tokens[nr_token].type=')';nr_token++;break;
	        case 'u':break;
          default: TODO();
        }
        break;
      }
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }}
  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  for(int i=0;i<nr_token;i++){
         if(tokens[i].type=='*'&&(i==0||tokens[i-1].type=='+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'||tokens[i-1].type=='/'||tokens[i-1].type==250||tokens[i-1].type==251||tokens[i-1].type==252))
       tokens[i].type=249;}		 
  detect=1;
  int temp=eval(0,nr_token-1);
  if(detect==0){
	  printf("presentation error!");
           return 0;}
  else
          return temp;
  /* TODO: Insert codes to evaluate the expression. */
  TODO();
}
