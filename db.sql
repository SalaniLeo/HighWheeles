
-- Creazione del database
CREATE DATABASE IF NOT EXISTS pista;
USE pista;

-- Tabella dei giri
CREATE TABLE IF NOT EXISTS giri (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Tabella dei sensori
CREATE TABLE IF NOT EXISTS sensori (
    id INT AUTO_INCREMENT PRIMARY KEY,
    id_giro INT,
    numero_sensore INT,
    velocita FLOAT,
    FOREIGN KEY (id_giro) REFERENCES giri(id)
);
