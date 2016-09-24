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
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = '''

SELECT al.Airline_ID
FROM airports ap, airlines al, routes r, airlines al2, routes r2
WHERE r.Airline_ID = al.Airline_ID AND r2.Airline_ID = al2.Airline_ID
      AND al.Airline_ID = al2.Airline_ID AND al.active = 'Y' 
      AND al.ICAO IS NOT NULL AND al.IATA IS NOT NULL AND al.callsign 
      IS NOT NULL AND al.country IS NOT NULL
EXCEPT
SELECT al.Airline_ID
FROM Airports ap, Airports ap2 airlines al, routes r, airlines al2, routes r2
WHERE r.Airline_ID = al.Airline_ID AND r2.Airline_ID = al2.Airline_ID
      AND al.Airline_ID = al2.Airline_ID AND r.Source_airport_ID = 
      r2.Destination_airport_ID AND r.Destination_airport_ID = r2.Source_airport_ID
      AND al.active = 'Y' AND al.ICAO IS NOT NULL AND al.IATA 
      IS NOT NULL AND al.callsign IS NOT NULL AND al.country IS NOT NULL
      ''';

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