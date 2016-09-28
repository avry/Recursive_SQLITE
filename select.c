//Avery Tan

#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the update statement
  char *database_name = "opeflights.db"

  char *sql_statement =   "SELECT r.airline, r.Airline_ID, r.source_airport, r.source_airport_ID,"
                         "r.Destination_airport, r.Destination_airport_ID, r.codeshare,r.stops," 
                          "r.equipment "
                    "FROM airlines al, routes r"
                    "WHERE r.Airline_ID = al.Airline_ID AND al.active = 'Y' AND al.ICAO IS NOT NULL"
                          "AND al.IATA IS NOT NULL AND al.callsign IS NOT NULL AND "
                          "al.country IS NOT NULL"
                    "EXCEPT"
                    "SELECT r8.airline, r8.Airline_ID, r8.source_airport, r8.source_airport_ID,"
                          "r8.Destination_airport, r8.Destination_airport_ID, r8.codeshare,r8.stops," 
                          "r8.equipment"
                    "FROM  routes r8, routes r9"
                    "WHERE r8.Airline_ID = r9.Airline_ID AND r8.source_airport_ID=r9.Destination_airport_ID"
                          "and r8.Destination_airport_ID = r9.source_airport_ID"
                    "EXCEPT"
                    "SELECT r6.*"
                    "FROM  airports ap6, airports ap7, routes r6"
                    "WHERE r6.destination_airport_id = ap6.airport_id and r6.source_airport_ID = ap7.airport_id" 
                          "and ap7.country = ap6.country"

  	int rc;

  	if( argc!=3 ){
    	fprintf(stderr, "Usage: %s <database file> <select query>\n", argv[0]);
    	return(1);
  	}

  	rc = sqlite3_open(database_name, &db);
  	if( rc ){
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return(1);
  	}

  	char *sql_stmt = sql_statement;

  	rc = sqlite3_prepare_v2(db, sql_stmt, -1, &stmt, 0);

    if (rc != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }    
      
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int col;
        for(col=0; col<sqlite3_column_count(stmt)-1; col++) {
          printf("%s|", sqlite3_column_text(stmt, col));
        }
        printf("%s", sqlite3_column_text(stmt, col));
        printf("\n");
    }

    sqlite3_finalize(stmt); //always finalize a statement
}