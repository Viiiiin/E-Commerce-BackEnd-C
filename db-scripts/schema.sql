
-- database already exists

\c :dbname 


-- Pick the one most appropriate for your application
-- CREATE DOMAIN TimePoint AS date ;
create domain TimePoint as timestamp ;
-- create domain TimePoint as timestamp with time zone;


CREATE DOMAIN  SmallVar AS VARCHAR(30) ;
CREATE DOMAIN  MediumVar as VARCHAR(200) ;
CREATE DOMAIN  LargeVar as VARCHAR(2000) ;
-- create domain VarChar with different length


CREATE DOMAIN IntegerGtz as Integer check(value >0);
CREATE DOMAIN Valuta as VARCHAR(3);
CREATE DOMAIN CAP as VARCHAR(5);

CREATE TYPE Prezzo (
	Valuta valuta,
	Cifra IntegerGtz);
	
CREATE TYPE Indirizzo(
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
	indirizzo Indirizzo NOT NULL);
	

	
CREATE TABLE IF NOT EXISTS Azienda(
	id serial PRIMARY KEY,
	RagioneSociale MediumVar NOT NULL,
	sede indirizzo NOT NULL);
	

CREATE TABLE IF NOT EXISTS Produttore();
	
CREATE TABLE IF NOT EXISTS Trasportatore();
	
CREATE TABLE IF NOT EXISTS Prodotto(
	id serial PRIMARY KEY,
	nome SmallVar NOT NULL,
	descrizione LargeVar,
	prezzo Prezzo NOT NULL);
	
CREATE TABLE IF NOT EXISTS Acquisto(
	costumer int
	prodotto int);
	
	
	 
 


