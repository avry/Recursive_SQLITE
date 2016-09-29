/*
Q1 (10 pts)

In a plain text ASCII file called q1.txt answer the question is the 
database normalized? Explain your answer in 1 or 2 paragraphs.
*/
SELECT
FROM 
WHERE



/*Q2 (5 pts)

Write a C program, in a file called q2.c that prints a list of all 
international flights by active and commercial (i.e., not user-added) 
airlines that do not have a reciprocal return. (A reciprocal return flight 
for YEG->LHR would be LHR->YEG by the same airline).
*/

select count(*)
from

(SELECT r.airline, r.Airline_ID, r.source_airport, r.source_airport_ID,
r.Destination_airport, r.Destination_airport_ID, r.codeshare,r.stops, r.equipment 
FROM airlines al, routes r
WHERE r.Airline_ID = al.Airline_ID 
      AND al.active = 'Y' 
      AND al.ICAO IS NOT NULL AND al.IATA IS NOT NULL AND al.callsign 
      IS NOT NULL AND al.country IS NOT NULL
EXCEPT
SELECT r8.airline, r8.Airline_ID, r8.source_airport, r8.source_airport_ID,
r8.Destination_airport, r8.Destination_airport_ID, r8.codeshare,r8.stops, r8.equipment
FROM  routes r8, routes r9
WHERE r8.Airline_ID = r9.Airline_ID AND r8.source_airport_ID=r9.Destination_airport_ID
      and r8.Destination_airport_ID = r9.source_airport_ID
EXCEPT
SELECT r6.*
FROM  airports ap6, airports ap7, routes r6
WHERE r6.destination_airport_id = ap6.airport_id and r6.source_airport_ID = ap7.airport_id 
      and ap7.country = ap6.country
      )


/* result is 1862 records*/



/*
Q3 (5 pts)

Write a C program, in a file called q3.c that prints the list of 
top-10 countries with the most airlines flying to/from Canada.
*/
select countries.country, COUNT(*)
from
(      SELECT ap2.country
      FROM airports ap,  routes r, airports ap2
      WHERE r.destination_airport_ID = ap.airport_id and ap.country = "Canada" and 
            r.source_airport_ID = ap2.airport_id 
      UNION ALL
      SELECT ap9.country 
      FROM airports ap8,  routes r8, airports ap9
      WHERE r8.source_airport_ID = ap8.airport_id and ap8.country = "Canada" and 
            r8.destination_airport_ID = ap9.airport_id ) as countries
group by countries.country order by count(*) DESC limit 10;

/*
Canada|2298
United States|729
United Kingdom|83
Mexico|57
Cuba|56
France|38
Germany|38
Dominican Republic|31
China|30
Jamaica|22
*/

/*
Q4 (10 pts)

Write a C program, in a file called q4.c that prints the list of 
top-10 lengthiest commercial flights (measured by the geographical 
distance between the respective airports). 
See https://en.wikipedia.org/wiki/Geographical_distance.
*/

SELECT r1.* , ap1.latitude, ap1.longitude, ap2.latitude, ap2.longitude
FROM routes r1, airports ap1, airports ap2
WHERE r1.source_airport_ID = ap1.airport_id and r1.destination_airport_ID = ap2.airport_id





/*
Q5 (15 pts)

Write a C program, in a file called q5.c that prints the list of types 
of airplanes capable of operating flights over 10,000 Km in length.
*/

SELECT *
FROM routes



/*
Q6 (5 pts)

Write a C program, in a file called q6.c that prints the list of 
airports that can be reached from YEG with at most 3 connections.
*/

WITH RECURSIVE 
      cnt(id, name) AS ( 
            SELECT r.Destination_airport_ID, r.Destination_airport 
            FROM routes r 
            WHERE r.source_airport = "YEG" 
                  UNION 
            SELECT r1.Destination_airport_ID 
            FROM routes r1
            WHERE r1.source_airport_ID = cnt.id
            LIMIT 3
      )
      SELECT id, name from cnt;


/*
Q7 (15 pts)

Write a C program, in a file called q7.c that prints the highest 
airport(s) that one can reach from YEG, regardless of the number of connections.
*/
WITH RECURSIVE 
      cnt(id, name) AS ( 
            SELECT r.Destination_airport_ID, r.Destination_airport 
            FROM routes r 
            WHERE r.source_airport = "YEG" 
                  UNION 
            SELECT r1.Destination_airport_ID 
            FROM routes r1
            WHERE r1.source_airport_ID = cnt.id
      )
      SELECT id, name from cnt;



/*
Q8 (15 pts)

Write a C program, in a file called q8.c that prints the list of airports 
with commercial flights into (or out of) them, but yet cannot be reached 
flying from from YEG regardless of the number of connections.
*/

SELECT ap.name, ap.airport_id
FROM airports ap
EXCEPT
WITH RECURSIVE 
      cnt(id, name) AS ( 
            SELECT r.Destination_airport_ID, r.Destination_airport 
            FROM routes r 
            WHERE r.source_airport = "YEG" 
                  UNION 
            SELECT r1.Destination_airport_ID 
            FROM routes r1
            WHERE r1.source_airport_ID = cnt.id
      )
      SELECT id, name from cnt;

WITH RECURSIVE 
      cnt(id, name) AS ( 
            SELECT r.Destination_airport_ID, r.Destination_airport 
            FROM routes r 
            WHERE r.source_airport = "YEG" 
                  UNION 
            SELECT r1.Destination_airport_ID 
            FROM routes r1
            WHERE r1.source_airport_ID = cnt.id
      )
SELECT ap.name, ap.airport_id
FROM airports ap
EXCEPT
SELECT id, name from cnt;

/*
Q9 (15 pts)

Write a C program, in a file called q9.c that takes an airport IATA 
code as a parameter and produces the equivalent answer of question 8 
above for that airport.
*/

SELECT ap1.name, ap1.airport_id
FROM airports ap
EXCEPT
WITH RECURSIVE 
      cnt(id, name) AS ( 
            SELECT r.Destination_airport_ID, r.Destination_airport 
            FROM routes r , airports ap
            WHERE r.airport_id = ap.airport_id AND ap.IATA_FAA = "%s"
                  UNION 
            SELECT r1.Destination_airport_ID 
            FROM routes r1
            WHERE r1.source_airport_ID = cnt.id
      )
      SELECT id, name from cnt;




