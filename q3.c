//Avery Tan

#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";

  char sql_statement[999] =  "select countries.country, COUNT(*) "\
                              "from "\
                              "(    SELECT ap2.country "\
                                    "FROM airports ap,  routes r, airports ap2 "\
                                    "WHERE r.destination_airport_ID = ap.airport_id "\
                                          "and ap.country = \"Canada\" and "\
                                          "r.source_airport_ID = ap2.airport_id "\
                                    "UNION ALL "\
                                    "SELECT ap9.country "\
                                    "FROM airports ap8,  routes r8, airports ap9 "\
                                    "WHERE r8.source_airport_ID = ap8.airport_id and "\
                                          "ap8.country = \"Canada\" and "\
                                          "r8.destination_airport_ID = ap9.airport_id ) as countries "\
                              "group by countries.country order by count(*) DESC limit 10;";


  	int rc;

  	/*if( argc!=3 ){
    	fprintf(stderr, "Usage: %s <database file> <select query>\n", argv[0]);
    	return(1);
  	}*/

  	rc = sqlite3_open(database_name, &db);
  	if( rc ){
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return(1);
  	}
  	//char *sql_stmt = sql_statement;

  	rc = sqlite3_prepare_v2(db, sql_statement, -1, &stmt, 0);

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