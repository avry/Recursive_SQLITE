//Avery Tan

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){


	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the update statement
    char database_name[25] = "openflights.db";
    char sql_statement[999];

    char sql_statement1[] = "WITH RECURSIVE "\
                              "cnt(id, iata) AS ( "\
                                    "SELECT r.Destination_airport_ID, r.Destination_airport "\
                                    "FROM routes r , airports ap "\
                                    "WHERE r.source_airport_id = ap.airport_id AND ap.IATA_FAA = \"";

    //char *sql_iata = argv[1];                                

    char sql_statement2[] =          "\" UNION "\
                                    "SELECT r1.Destination_airport_ID, r1.Destination_airport "\
                                    "FROM cnt cn, routes r1 "\
                                    "WHERE r1.source_airport_ID = cn.id "\
                              ") "\
                              //"SELECT COUNT(*) FROM ( "\
                              "SELECT ap1.airport_id, ap1.IATA_FAA "\
                              "FROM airports ap1 "\
                              "EXCEPT "\
                              "SELECT id, iata from cnt;";
                        							
    //strcpy(sql_statement, argv[1]);
    strcpy(sql_statement, sql_statement1);
    //printf("\n %s \n", sql_statement);
    strncat(sql_statement, argv[1], sizeof(argv[1]));
    //printf("\n %s \n", sql_statement);
    strncat(sql_statement, sql_statement2, sizeof(sql_statement2));
    //printf("\n %s \n", sql_statement);

    //printf("\n\nsize is %lu \n", sizeof(sql_statement));

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