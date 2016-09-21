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

SELECT al.Airline_ID
FROM Airport ap, airline al, routes r
WHERE r.Airline_ID = al.Airline_ID AND r.source_airport_ID = ap.Airport_ID 
      AND r.Destination_airport_ID = ap.Airport_ID AND al.active = 'Y' AND 
      al.ICAO IS NOT NULL AND al.IATA IS NOT NULL AND al.callsign IS NOT NULL AND
      al.country IS NOT NULL

EXCEPT

SELECT al.Airline_ID
FROM Airport ap, airline al, routes r
WHERE r.Airline_ID = al.Airline_ID AND r.source_airport_ID = ap.Airport_ID 
      AND r.Destination_airport_ID = ap.Airport_ID AND al.active = 'Y' AND 





/*
Q3 (5 pts)

Write a C program, in a file called q3.c that prints the list of 
top-10 countries with the most airlines flying to/from Canada.
*/

SELECT ap.country, COUNT(*)
FROM Airport ap, airline al, routes r
WHERE r.Airline_ID = al.Airline_ID AND r.source_airport_ID = ap.Airport_ID 
      AND r.Destination_airport_ID = ap.Airport_ID AND ap.Airport_ID = 'Canada'
GROUP BY ap.country;




/*
Q4 (10 pts)

Write a C program, in a file called q4.c that prints the list of 
top-10 lengthiest commercial flights (measured by the geographical 
distance between the respective airports). 
See https://en.wikipedia.org/wiki/Geographical_distance.
*/

SELECT 
FROM
WHERE





/*
Q5 (15 pts)

Write a C program, in a file called q5.c that prints the list of types 
of airplanes capable of operating flights over 10,000 Km in length.
*/

SELECT 
FROM
WHERE



/*
Q6 (5 pts)

Write a C program, in a file called q6.c that prints the list of 
airports that can be reached from YEG with at most 3 connections.
*/

SELECT
FROM
WHERE



/*
Q7 (15 pts)

Write a C program, in a file called q7.c that prints the highest 
airport(s) that one can reach from YEG, regardless of the number of connections.
*/

SELECT
FROM
WHERE



/*
Q8 (15 pts)

Write a C program, in a file called q8.c that prints the list of airports 
with commercial flights into (or out of) them, but yet cannot be reached 
flying from from YEG regardless of the number of connections.
*/

SELECT 
FROM
WHERE





/*
Q9 (15 pts)

Write a C program, in a file called q9.c that takes an airport IATA 
code as a parameter and produces the equivalent answer of question 8 
above for that airport.
*/

SELECT 
FROM
WHERE



