import geodist

sql = select r.Airline_ID, ap.Latitude, ap.Longitude, 
            ap2.Latitude, ap2.Longitude
      from routes r, airports ap, airports ap2
      where r.Source_airport_ID = ap.Airport_ID AND 
            r.Destination_airport_ID = ap2.Airport_ID


#sql is now a list containing all the tables that pass the test.

airline_distance = dict()

for i in sql:
   distance = geodist.distance(sql[1], sql[2],sql[3], sql[4], 'M') 
   airline_distance[sql[0]] = distance

sort(airline_distance)


for i in range()
