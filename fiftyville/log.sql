-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Figure out what crimes happened on this day on this street
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND year = 2021 AND street = 'Humphrey Street';

-- id of CS50 duck theft = 295
-- description: time - 10:15 am, location - bakery, three witnesses interviewed

-- meant to find the three witnesses of interest from the crime
SELECT id, name, transcript FROM interviews
WHERE month = 7 AND day = 28 AND year = 2021 and transcript LIKE '%thief%';

-- the three witnesses: Ruth, Eugene, Raymond
-- Ruth: Within ten minutes of the theft, the thief got into a car and drove away (10:15 to 10:25). Look at bakery security footage to narrow this down.
-- Eugene: The thief was someone they recognized. Earlier in the day, they saw the thief at an ATM on Leggett Street withdrawing money.
-- Raymond: As they left the bakery, the thief called someone who talked to them for less than a minute.
--          Thief plans to take the earliest flight out of Fiftyville tomorrow, which the accomplice purchased a ticket for.

-- Check baker security logs during the given timespan and gets license plates of interest
SELECT license_plate FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute >= 16 AND minute <= 26;

-- gets the account numbers from atm withdrawals that occurred that day at the Leggett Street atm

SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' and transaction_type = 'withdraw';

-- gets caller and receivers from phone calls that took place on the day and were less than one minute
-- caller is a candidate for thief, receiver is a candidate for accomplice

SELECT caller, receiver FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE origin_airport_id =
(SELECT id FROM airports WHERE city = 'Fiftyville')
AND year = 2021 AND month = 7 AND day = 29
)



--should be able to get the thief from their bank account number, phone number, passport number, and license plate

SELECT name, passport_number, phone_number FROM people
JOIN bank_accounts ON people.id = person_id
WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' and transaction_type = 'withdraw')
AND phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
AND passport_number IN
(SELECT passport_number FROM passengers
JOIN flights ON flight_id = flights.id
JOIN airports ON origin_airport_id = airports.id
WHERE city = 'Fiftyville' AND year = 2021 AND month = 7 AND day = 29)
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute >= 16 AND minute <= 26);

--narrowed it down to Bruce or Diana and got their passport numbers, now check the time of their flight

SELECT hour, minute, destination_airport_id FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE passport_number = 5773159633;

-- got back that Bruce flew out at 8:20 am

SELECT hour, minute, destination_airport_id FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE passport_number = 3592750733;

-- flew three times, but the earliest is 10:19 am so Bruce is the thief! Answer 1
-- His destination airport id was 4, so check which one that is
SELECT city FROM airports WHERE id = 4;

-- Answer 2: New York City
-- Lastly, we want to get the other person on the line with Bruce, using Bruce's phone number (367) 555-5533

SELECT name FROM people WHERE phone_number IN
(SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller = '(367) 555-5533');

-- Answer 3: Robin!

--should be able to get the accomplice from their phone number and the guilty caller's phone number
SELECT name FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND year = 2021 AND hour = 10 AND minute >= 16 AND minute <= 26);