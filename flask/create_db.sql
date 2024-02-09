DROP DATABASE IF EXISTS seminar;
CREATE DATABASE seminar;
USE seminar;

DROP USER app;
CREATE USER app@'%' IDENTIFIED BY '1234';
GRANT SELECT, INSERT, UPDATE, DELETE ON seminar.* TO app@'%';

-- kreiramo tablicu korisnik i ubacujemo podatke
CREATE TABLE korisnik (
    id INT PRIMARY KEY auto_increment,
    ime CHAR(30),
    prezime CHAR(30),
    username VARCHAR(30),
    password VARCHAR(32)
);

INSERT INTO korisnik(ime, prezime, username, password)
VALUES
    ('Dominik', 'Lucic', 'dlucic', '0209'),
    ('Zvonimir', 'Skvorc', 'zskvorc', '3006');
    
    
CREATE TABLE temperatura(
	id INT PRIMARY KEY auto_increment,
    vrijednost INT
);
