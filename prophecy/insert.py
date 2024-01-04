from cs50 import SQL
import csv

db = SQL('sqlite:///roster.db')

with open('students.csv') as file:
    reader = csv.DictReader(file)
    db.execute("INSERT INTO houses (id, house, head) VALUES(1, 'Slytherin', 'Severus Snape')")
    db.execute("INSERT INTO houses (id, house, head) VALUES(2, 'Ravenclaw', 'Filius Flitwick')")  # first, fill out houses table with the four houses and respective heads
    db.execute("INSERT INTO houses (id, house, head) VALUES(3, 'Hufflepuff', 'Pomona Sprout')")
    db.execute("INSERT INTO houses (id, house, head) VALUES(4, 'Gryffindor', 'Minerva McGonagall')")
    for row in reader:
        # db.execute('INSERT INTO students (id, student_name) VALUES(?, ?)', row['id'], row['student_name'])  # insert the given student into students
        db.execute('INSERT INTO house_assignments (id, student_id, house_id) VALUES(?, ?, SELECT id FROM houses WHERE house = (SELECT house from houses WHERE house = ?)).', row['id'], row['id'], row['house'])

    # don't think student id is actually needed if it matches assignment id


