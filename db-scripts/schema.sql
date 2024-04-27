
-- database already exists

\c :dbname 


-- Pick the one most appropriate for your application
-- CREATE DOMAIN TimePoint AS date ;
create domain TimePoint as timestamp ;
-- create domain TimePoint as timestamp with time zone;

CREATE DOMAIN String4Info AS VARCHAR(2000);
CREATE DOMAIN String4VarName AS VARCHAR(2000);
CREATE DOMAIN SmallVar AS VARCHAR(30) ;
CREATE DOMAIN MediumVar as VARCHAR(200) ;
CREATE DOMAIN LargeVar as VARCHAR(2000) ;
-- create domain VarChar with different length

CREATE DOMAIN IntegerGEZ as Integer check(value >=0);
CREATE DOMAIN IntegerGtz as Integer check(value >0);
CREATE DOMAIN Valuta as VARCHAR(3);
CREATE DOMAIN CAP as VARCHAR(5);

CREATE TYPE Prezzo AS (
	Valuta valuta,
	Cifra IntegerGtz);
	
CREATE TYPE Indirizzo AS (
	Via MediumVar,
	Civico SmallVar,
	CAP CAP);

-- Pick the one most appropriate for your application
--CREATE DOMAIN VarType AS real ;
create domain VarType as double precision ;
-- create domain VarType as int ;


CREATE TYPE DomainType AS ENUM ('costumer', 'fornitore', 'trasportatore');



-- Patient data that depend on time
-- Variables within time series (all have the same type)
CREATE TABLE IF NOT EXISTS nomefun (
        nome MediumVar PRIMARY KEY
);

-- Timescaledb extension for time series
-- Do not forget to create timescaledb extension
-- CREATE EXTENSION timescaledb;

CREATE TABLE IF NOT EXISTS LogTable (
        nanosec bigint NOT NULL,		-- nanseconds after second
        id int NOT NULL,			-- variable id
        vardomain DomainType NOT NULL,          
		fun MediumVar NOT NULL,
		loginfo String4Info,                     -- extra info
		istante TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        PRIMARY KEY (nanosec, id, vardomain),
        FOREIGN KEY(fun) REFERENCES nomefun(nome));
        

CREATE TABLE IF NOT EXISTS Costumer(
	id serial PRIMARY KEY,
	nome SmallVar NOT NULL,
	cognome SmallVar NOT NULL,
	email SmallVar NOT NULL,
	indirizzo Indirizzo NOT NULL
);
		
CREATE TABLE IF NOT EXISTS Produttore(
	id serial PRIMARY KEY,
	nome MediumVar NOT NULL,
	sede indirizzo NOT NULL 
);
	
CREATE TABLE IF NOT EXISTS Trasportatore(
	id serial PRIMARY KEY,
	nome MediumVar NOT NULL,
	sede indirizzo NOT NULL 
);
	
CREATE TABLE IF NOT EXISTS Prodotto(
	id serial PRIMARY KEY,
	produttore IntegerGEZ NOT NULL,
	nome SmallVar NOT NULL,
	descrizione LargeVar,
	prezzo Prezzo NOT NULL,
	FOREIGN KEY(produttore) REFERENCES Produttore(id)
);
	
CREATE TABLE IF NOT EXISTS Acquisto(
	id serial PRIMARY KEY,
	istante TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	costumer IntegerGEZ NOT NULL,
	prodotto IntegerGEZ NOT NULL,
	trasportatore IntegerGEZ NOT NULL,
	consegnato bool NOT NULL,
	istConsegna TIMESTAMP, 
	FOREIGN KEY(costumer) REFERENCES Costumer(id),
	FOREIGN KEY(prodotto) REFERENCES Prodotto(id),
	FOREIGN KEY(trasportatore) REFERENCES Trasportatore(id),
	CONSTRAINT check_istante CHECK ( istante < istConsegna ),
	CONSTRAINT check_isConsegnato CHECK (( consegnato = true AND istConsegna is NOT NULL ) OR ( consegnato = false ))
);


INSERT INTO nomefun (nome) VALUES
    ('Consegna'),
    ('Acquisto'),
    ('Vendita');


INSERT INTO Trasportatore (nome, sede) VALUES 
    ('Azienda di Trasporti ABC', ('Via Roma', '123', '00100')),
    ('Trasporti XYZ S.p.A.', ('Via Milano', '456', '00200')),
    ('Logistica 123', ('Corso Napoli', '789', '00300'));


INSERT INTO Costumer (nome, cognome, email, indirizzo) 
VALUES 
    ('Mario', 'Rossi', 'mario.rossi@example.com', ('Via Roma', '123', '00100')),
    ('Luigi', 'Bianchi', 'luigi.bianchi@example.com', ('Via Milano', '456', '00200')),
    ('Anna', 'Verdi', 'anna.verdi@example.com', ('Corso Napoli', '789', '00300'));


INSERT INTO Produttore (nome, sede) 
VALUES 
    ('Azienda Tessile S.r.l.', ('Via Garibaldi', '10', '20100')),
    ('Calzaturificio Italiano S.p.A.', ('Corso Italia', '20', '50123')),
    ('Maglieria Moda S.r.l.', ('Via Roma', '30', '10100')),
    ('Fabbri Abbigliamento S.p.A.', ('Via Venezia', '40', '30100'));


INSERT INTO Prodotto (produttore, nome, descrizione, prezzo) 
VALUES 
    (1, 'Laptop', 'Laptop con schermo da 15 pollici', ROW('EUR', 99999)),
    (2, 'Smartphone', NULL, ROW('USD', 79999)),
    (1, 'Tastiera wireless', 'Tastiera senza fili con layout italiano', ROW('EUR', 4999)),
    (3, 'Mouse gaming', NULL, ROW('USD', 2999)),
	(2, 'Tablet', 'Tablet con schermo da 10 pollici', ROW('USD', 49999)),
    (3, 'Cuffie Bluetooth', 'Cuffie senza fili con cancellazione del rumore', ROW('USD', 14999)),
    (1, 'Monitor', 'Monitor da 27 pollici con risoluzione 4K', ROW('EUR', 79999));
