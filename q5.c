//Avery Tan
//to compile executable code, type "gcc -g q4.c sqlite3.c -lpthread -ldl -lm"
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>


/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::                                                                         :*/
/*::  This routine calculates the distance between two points (given the     :*/
/*::  latitude/longitude of those points). It is being used to calculate     :*/
/*::  the distance between two locations using GeoDataSource(TM) products.   :*/
/*::                                                                         :*/
/*::  Definitions:                                                           :*/
/*::    South latitudes are negative, east longitudes are positive           :*/
/*::                                                                         :*/
/*::  Passed to function:                                                    :*/
/*::    lat1, lon1 = Latitude and Longitude of point 1 (in decimal degrees)  :*/
/*::    lat2, lon2 = Latitude and Longitude of point 2 (in decimal degrees)  :*/
/*::    unit = the unit you desire for results                               :*/
/*::           where: 'M' is statute miles (default)                         :*/
/*::                  'K' is kilometers                                      :*/
/*::                  'N' is nautical miles                                  :*/
/*::  Worldwide cities and other features databases with latitude longitude  :*/
/*::  are available at http://www.geodatasource.com                          :*/
/*::                                                                         :*/
/*::  For enquiries, please contact sales@geodatasource.com                  :*/
/*::                                                                         :*/
/*::  Official Web site: http://www.geodatasource.com                        :*/
/*::                                                                         :*/
/*::           GeoDataSource.com (C) All Rights Reserved 2015                :*/
/*::                                                                         :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

#include <math.h>

#define pi 3.14159265358979323846

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Function prototypes                                           :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double);
double rad2deg(double);

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch(unit) {
    case 'M':
      break;
    case 'K':
      dist = dist * 1.609344;
      break;
    case 'N':
      dist = dist * 0.8684;
      break;
  }
  return (dist);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
  return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
  return (rad * 180 / pi);
}



//######################################################################################################







int main(int argc, char **argv){
  char equipment[10];
	double geo_comparator;
	double source_lat = 0;
	double source_lon = 0;
	double dest_lat = 0;
	double dest_lon = 0;
	char database_name[25] = "openflights.db";
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the update statement
  sqlite3_stmt *stmt_cr;
  sqlite3_stmt *stmt_in;
  sqlite3_stmt *stmt_re;
  
  char sql_statement[999] =  "SELECT r1.* , ap1.latitude, ap1.longitude, ap2.latitude, "\
                                      "ap2.longitude "\
                              "FROM routes r1, airports ap1, airports ap2 "\
                              "WHERE r1.source_airport_ID = ap1.airport_id and " \
                                    "r1.destination_airport_ID = ap2.airport_id ";
  char sql_create_table[999] = "CREATE TABLE long_distance_aircraft(airplane VARCHAR(10));" ;                                
	int rc;
  int rc_1;
  int rc_2;


	rc = sqlite3_open(database_name, &db);
	if( rc ){
  	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  	sqlite3_close(db);
  	return(1);
	}
	rc = sqlite3_prepare_v2(db, sql_statement, -1, &stmt, 0); //opening database to get all queries of flights
  rc_1 = sqlite3_prepare_v2(db, sql_create_table, -1, &stmt_cr, 0);


  

  if (rc != SQLITE_OK) {  
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  if (rc_1 != SQLITE_OK) {  
      fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
    }
  //creating a new long distance plane table
  if ((rc_1 = sqlite3_step(stmt_cr)) != SQLITE_DONE){
      fprintf(stderr, "Update failed: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;     
  }


  while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    char quotation[] = "\"";
    char sql_insert_first[500] = "INSERT INTO long_distance_aircraft(airplane) values (\"";
    source_lat = atof((char *) sqlite3_column_text(stmt, 9));
    source_lon = atof((char *) sqlite3_column_text(stmt, 10));
    dest_lat = atof((char *) sqlite3_column_text(stmt, 11));
    dest_lon = atof((char *) sqlite3_column_text(stmt, 12));
    geo_comparator= distance( source_lat, source_lon ,dest_lat,dest_lon,'K');

    if (geo_comparator > 10000){
      equipment= strdup( sqlite3_column_text(stmt, 8));
      strncat(sql_insert_first, equipment, sizeof(equipment));
      strncat(sql_insert_first, quotation, sizeof(quotation));
      rc_2 = sqlite3_prepare_v2(db, sql_insert_first, -1, &stmt_in, 0);
      if (rc_2 != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
      }
      if ((rc_2 = sqlite3_step(stmt_cr)) != SQLITE_DONE){
        fprintf(stderr, "Update failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;     
      }

    }
  
  }
  sql_results[500] = "SELECT DISTINCT airplane from long_distance_aircraft;";
  rc_3 = sqlite3_prepare_v2(db, sql_results, -1, &stmt_re, 0);
  if (rc_3 != SQLITE_OK) {  
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  if ((rc_3 = sqlite3_step(stmt_re)) != SQLITE_DONE){
      fprintf(stderr, "Update failed: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;     
  }

 	sqlite3_finalize(stmt); //always finalize a statement
  sqlite3_finalize(stmt_cr);
  sqlite3_finalize(stmt_in);
  sqlite3_finalize(stmt_re);
}


