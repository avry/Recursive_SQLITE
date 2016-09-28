#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 


static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("openflights.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "select countries.country, COUNT(*)"\
"from"\
"(      SELECT ap2.country"\
"      FROM airports ap,  routes r, airports ap2"\
"      WHERE r.destination_airport_ID = ap.airport_id and ap.country = \"Canada\" and "\
"            r.source_airport_ID = ap2.airport_id "\
"      UNION ALL"\
"      SELECT ap9.country "\
"      FROM airports ap8,  routes r8, airports ap9"\
"      WHERE r8.source_airport_ID = ap8.airport_id and ap8.country = \"Canada\" and "\
"            r8.destination_airport_ID = ap9.airport_id ) as countries"\
"group by countries.country";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
}