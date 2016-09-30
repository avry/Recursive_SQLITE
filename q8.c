//Avery Tan
//assignment1 CMPUT391
#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
  sqlite3 *db; //the database
  sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";


  /*
  Q8 (15 pts)

  Write a C program, in a file called q8.c that prints the list of airports 
  with commercial flights into (or out of) them, but yet cannot be reached 
  flying from from YEG regardless of the number of connections.
  */
  char sql_statement[999] = "WITH RECURSIVE "\
                            "cnt(id, name) AS ( "\
                                  "SELECT r.Destination_airport_ID, r.Destination_airport "\
                                  "FROM routes r , airlines al"\
                                  "WHERE r.source_airport = \"YEG\" "\
                                        "and r.airline_id = al.airline_id and "\
                                        "al.iata is not null and al.icao is not null "\
                                        "and al.callsign is not null and al.country is not null "\
                                        "UNION "\
                                  "SELECT r1.Destination_airport_ID, r1.Destination_airport "\
                                  "FROM cnt cn, routes r1, airlines al6 "\
                                  "WHERE r1.source_airport_ID = cn.id and r1.airline_id = al6.airline_id "\
                                        "and al6.iata is not null and al6.icao is not null and "\
                                        "al6.callsign is not null and al6.country is not null "\
                            ") "\
                            "SELECT ap.airport_id "\
                            "FROM airports ap "\
                            "EXCEPT "\
                            "SELECT id from cnt;";
              


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
  
  //printing our results
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