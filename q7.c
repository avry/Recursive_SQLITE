//Avery Tan
//3388 airports reacheable from YEG
#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";

  char sql_statement[999] =  "WITH RECURSIVE "\
            						      "cnt(id, iata) AS ( "\
            						            "SELECT r.Destination_airport_ID, r.Destination_airport "\
            						            "FROM routes r "\
            						            "WHERE r.source_airport = \"YEG\" "\
            						                  "UNION "\
            						            "SELECT r1.Destination_airport_ID, r1.Destination_airport "\
            						            "FROM cnt cn, routes r1 "\
            						            "WHERE r1.source_airport_ID = cn.id "\
            						      ") "\
            						      "SELECT COUNT(*) FROM (SELECT id from cnt);";


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