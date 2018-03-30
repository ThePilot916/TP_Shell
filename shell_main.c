#include "tpshell.h"

int main(){
  initiate_globals();
  while(1){
    prompt();
    if(!yyparse())
      printf("ERROR: parser failed.\n");
  }
}
