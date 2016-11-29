#include "function.h"
#include <glib.h>


gboolean insert_user(GHashTable * hash_tab, char * pseudo , char * id){
  
  char * key = strdup(pseudo);
  char * val = strdup(id);
  if(key == NULL || val == NULL){
    return FALSE ; 
  }
  gboolean r = g_hash_table_insert (hash_tab,key,val) ;
  return r ; 
   
}

gboolean is_in_hashTab(GHashTable * hash_tab ,char const * key){
  gboolean r ; 
  r =  g_hash_table_contains (hash_tab,key);
  return r ; 
}
