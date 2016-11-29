#include"function.h" 
/*la cle -> c'est le pseudo
  la valeur -> c'est l'identifiant
  gboolean -> (TRUE,FALSE)
  TRUE -> !FALSE
  FALSE -> 0 
*/

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

gboolean remove_user(GHashTable * hash_tab ,char const * key){

  gboolean r ; 
  r = g_hash_table_remove (hash_tab, key);
  return r ; 
}
