//Avery Tan
//assignment 1 CMPUT391
#include <stdio.h>
#include <sqlite3.h>


int main(int argc, char **argv){
  sqlite3 *db; //the database
  sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";


  /*
  Q3 (5 pts)

  Write a C program, in a file called q3.c that prints the list of 
  top-10 countries with the most airlines flying to/from Canada.
  */
  char sql_statement[999] = "select al_country.country, COUNT(*) "\
                            "from "\
                            "(SELECT DISTINCT al66.name,al66.country "\
                            "FROM airports ap,  routes r, airports ap2, airlines al66 "\
                            "WHERE al66.airline_id = r.airline_id AND r.destination_airport_ID = "\
                                  "ap.airport_id and "\
                                  "r.source_airport_ID = ap2.airport_id "\
                                  "and (ap.country = \"Canada\" or  ap2.country = \"Canada\")) as al_country "\
                            "GROUP BY al_country.country ORDER BY COUNT(*) DESC LIMIT 10; ";


  int rc;

  
  rc = sqlite3_open(database_name, &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  rc = sqlite3_prepare_v2(db, sql_statement, -1, &stmt, 0);

  if (rc != SQLITE_OK) {  
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }    
  
  //print out the results
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