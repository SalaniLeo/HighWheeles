const mysql = require('mysql2/promise');

const db = mysql.createPool({
  host: '192.168.50.10',
  port: 3306,
  user: 'scuola',
  password: '1234',
  database: 'scuola',
});

module.exports = db;