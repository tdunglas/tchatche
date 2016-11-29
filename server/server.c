#include<stdio.h>
#include<stdlib.h>
#include<glib.h>
#include"function.h"




int main(int arg , char * argv []){
  gboolean r ; 
  GHashTable * hash_tab =  g_hash_table_new (g_str_hash, g_str_equal);
  
  char * pseudo ="bachir";
  char * id = "kira" ;
  
  r = insert_user(hash_tab,pseudo,id);
  if(r==FALSE){
    printf("false") ;
    exit(1); 
  }
  
  r = is_in_hashTab(hash_tab,pseudo);
  printf("r = %d \n",r);

  return  0 ;
}
