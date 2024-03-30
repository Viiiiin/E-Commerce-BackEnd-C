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


CREATE TYPE DomainType AS ENUM ('discrete', 'real', 'double');



-- Patient data that depend on time
-- Variables within time series (all have the same type)
CREATE TABLE IF NOT EXISTS TimeVar (
        vid serial PRIMARY KEY,     -- serial id for variable
        pid int NOT NULL,           -- pid of process logged
        sysname String4Info,        --- system containing logged variable
        varname String4VarName NOT NULL,     -- variable name
        vardomain DomainType NOT NULL,
        varinfo String4Info NOT NULL
);

-- Timescaledb extension for time series
-- Do not forget to create timescaledb extension
-- CREATE EXTENSION timescaledb;

CREATE TABLE IF NOT EXISTS LogTable (
        nanosec bigint NOT NULL,		-- nanseconds after second
        vid int NOT NULL,			-- variable id
        varvalue VarType,                       -- variable value
	loginfo String4Info,                    -- extra info
        PRIMARY KEY (nanosec, vid),
        CONSTRAINT vid_ref FOREIGN KEY(vid) REFERENCES TimeVar(vid)
);

CREATE TABLE IF NOT EXISTS Costumer(
	id serial PRIMARY KEY,
	nome SmallVar NOT NULL,
	cognome SmallVar NOT NULL,
	email SmallVar NOT NULL,
	indirizzo Indirizzo NOT NULL
);
	
	
CREATE TABLE IF NOT EXISTS Produttore(
	id serial PRIMARY KEY,
	ragioneSociale MediumVar NOT NULL,
	sede indirizzo NOT NULL 
);
	
CREATE TABLE IF NOT EXISTS Trasportatore(
	id serial PRIMARY KEY,
	ragioneSociale MediumVar NOT NULL,
	sede indirizzo NOT NULL 
);
	
CREATE TABLE IF NOT EXISTS Prodotto(
	id serial PRIMARY KEY,
	nome SmallVar NOT NULL,
	descrizione LargeVar,
	prezzo Prezzo NOT NULL
);
	
CREATE TABLE IF NOT EXISTS Acquisto(
	id serial PRIMARY KEY,
	istante TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	costumer int NOT NULL,
	prodotto int NOT NULL,
	trasportatore int NOT NULL,
	consegnato bool NOT NULL,
	istConsegna TIMESTAMP, 
	FOREIGN KEY(costumer) REFERENCES Costumer(id),
	FOREIGN KEY(prodotto) REFERENCES Prodotto(id),
	FOREIGN KEY(trasportatore) REFERENCES Trasportatore(id),
	CONSTRAINT check_istante CHECK ( istante < istConsegna ),
	CONSTRAINT check_isConsegnato CHECK (( consegnato = true AND istConsegna is NOT NULL ) OR ( consegnato = false ))
);


INSERT INTO Trasportatore (ragioneSociale, sede) VALUES 
    ('Azienda di Trasporti ABC', ('Via Roma', '123', '00100')),
    ('Trasporti XYZ S.p.A.', ('Via Milano', '456', '00200')),
    ('Logistica 123', ('Corso Napoli', '789', '00300'));


INSERT INTO Costumer (nome, cognome, email, indirizzo) 
VALUES 
    ('Mario', 'Rossi', 'mario.rossi@example.com', ('Via Roma', '123', '00100')),
    ('Luigi', 'Bianchi', 'luigi.bianchi@example.com', ('Via Milano', '456', '00200')),
    ('Anna', 'Verdi', 'anna.verdi@example.com', ('Corso Napoli', '789', '00300'));


INSERT INTO Prodotto (nome, descrizione, prezzo) 
VALUES 
    ('Maglietta', 'Maglietta di cotone blu con logo stampato', ('EUR', 19)),
    ('Pantaloni', 'Pantaloni jeans regular fit', ('EUR', 39)),
    ('Scarpe', 'Scarpe sportive leggere e traspiranti', ('EUR', 59));
