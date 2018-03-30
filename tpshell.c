#include "tpshell.h"

int main(){
  initiate_globals();
  prompt();

    if(!yyparse()){
      printf("ERROR: parser failed.\n");
  }
}
