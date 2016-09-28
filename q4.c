//Avery Tan
//to compile executable code, type "gcc -g q4.c sqlite3.c -lpthread -ldl -lm"
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

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





  char airline[10] = {"", "", "", "", "", "", "", "", "", ""};
  char airline_id[10] = {"", "", "", "", "", "", "", "", "", ""} ;
  char source_airport[10]= {"", "", "", "", "", "", "", "", "", ""};
  char source_airport_id[10]= {"", "", "", "", "", "", "", "", "", ""};
  char destination_airport[10]= {"", "", "", "", "", "", "", "", "", ""};
  char destination_airport_id[10]= {"", "", "", "", "", "", "", "", "", ""};
  char codeshare[10]= {"", "", "", "", "", "", "", "", "", ""};
  int stops[10] = {0,0,0,0,0,0,0,0,0,0};
  char equipment[10]= {"", "", "", "", "", "", "", "", "", ""};
  double ap1_latitude[10] = {0,0,0,0,0,0,0,0,0,0};
  double ap1_longitude[10]= {0,0,0,0,0,0,0,0,0,0};
  double ap2_latitude[10]= {0,0,0,0,0,0,0,0,0,0};
  double ap2_longitude[10]= {0,0,0,0,0,0,0,0,0,0};
  double flight_distance[10]= {0,0,0,0,0,0,0,0,0,0};




int main(int argc, char **argv){
	sqlite3 *db; //the database
	sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";

  char sql_statement[999] =  "SELECT r1.* , ap1.latitude, ap1.longitude, ap2.latitude, "\
                                      "ap2.longitude "\
                              "FROM routes r1, airports ap1, airports ap2 "\
                              "WHERE r1.source_airport_ID = ap1.airport_id and " \
                                    "r1.destination_airport_ID = ap2.airport_id ";


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
    int count = 0;

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int col;
        for(col=0; col<sqlite3_column_count(stmt)-1; col++) {
          if (count < 10){
            switch (col) {
              case 0:
                //printf("%s ", sqlite3_column_text(stmt, col));
                strncpy(airline[count], (char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col)) );
                break;
              case 1:
                strncpy(airline_id[count], (char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col)));
                break;
              case 2:
                //printf("%s ", sqlite3_column_text(stmt, col));
                strncpy(source_airport[count] , (char *) sqlite3_column_text(stmt, col) , sizeof((char *) sqlite3_column_text(stmt, col)));
                break;
              case 3:
                //printf("%s\n", sqlite3_column_text(stmt, col));
                strncpy(source_airportid[count] , (char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col)));
                break;
              case 4:
                strncpy(destination_airport[count] ,(char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col)));
                break;
              case 5:
                strncpy(destination_airport_id[count] ,(char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 6:
                strncpy(codeshare[count] , (char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 7:
                strncpy(stops[count] , atoi((char *) sqlite3_column_text(stmt, col)), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 8:
                strncpy(equipment[count], (char *) sqlite3_column_text(stmt, col), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 9:
                strncpy(ap1_latitude[count] , atof((char *) sqlite3_column_text(stmt, col)), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 10:
                strncpy(ap1_longitude[count] , atof((char *) sqlite3_column_text(stmt, col)), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 11:
                strncpy(ap2_latitude[count] ,atof((char *) sqlite3_column_text(stmt, col)), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
              case 12:
                strncpy(ap2_longitude[count] , atof((char *) sqlite3_column_text(stmt, col)), sizeof((char *) sqlite3_column_text(stmt, col))); 
                break;
            }
          }
        }
        count = count +1;

    }

    sqlite3_finalize(stmt); //always finalize a statement
}


