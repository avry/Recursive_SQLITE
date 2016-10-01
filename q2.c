//Avery Tan
//q2.c
//assignment 1 CMPUT391

#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
  sqlite3 *db; //the database
  sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";

  /*Q2 (5 pts)

  Write a C program, in a file called q2.c that prints a list of all 
  international flights by active and commercial (i.e., not user-added) 
  airlines that do not have a reciprocal return. (A reciprocal return flight 
  for YEG->LHR would be LHR->YEG by the same airline).
  */
  char sql_statement[999] =  "SELECT r.airline, r.Airline_ID, r.source_airport, r.source_airport_ID," \
                                    "r.Destination_airport, r.Destination_airport_ID, r.codeshare, "\
                                    "r.stops, r.equipment  FROM airlines al, routes r WHERE r.Airline_ID = " \
                                    "al.Airline_ID AND al.active = 'Y' AND al.ICAO IS NOT NULL AND " \
                                    "al.IATA IS NOT NULL AND  al.callsign  IS NOT NULL AND al.country "\
                                    "IS NOT NULL " \
                            "EXCEPT " \
                            "SELECT r8.airline, r8.Airline_ID, r8.source_airport, " \
                                  "r8.source_airport_ID, r8.Destination_airport, " \
                                  "r8.Destination_airport_ID, r8.codeshare,r8.stops, " \
                                  "r8.equipment FROM  routes r8, routes r9 WHERE r8.Airline_ID = "\
                                  "r9.Airline_ID AND r8.source_airport_ID=r9.Destination_airport_ID "\
                                  "and r8.Destination_airport_ID = r9.source_airport_ID "\
                            "EXCEPT "\
                            "SELECT r6.* "\
                            "FROM  airports ap6, airports ap7, routes r6 "\
                            "WHERE r6.destination_airport_id = ap6.airport_id and "\
                                  "r6.source_airport_ID = ap7.airport_id "\
                                  "and ap7.country = ap6.country;";


  int rc;

  //open the database
  rc = sqlite3_open(database_name, &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  //prepare the query statement
  rc = sqlite3_prepare_v2(db, sql_statement, -1, &stmt, 0);

  if (rc != SQLITE_OK) {  
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }    
  

  //prints out the results
  while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int col;
    for(col=0; col<sqlite3_column_count(stmt)-1; col++) {
      printf("%s|", sqlite3_column_text(stmt, col));
    }
    printf("%s", sqlite3_column_text(stmt, col));
    printf("\n");
  }

  sqlite3_finalize(stmt); //finalizing statement
}