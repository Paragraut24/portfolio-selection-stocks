const mysql = require('mysql2/promise');
const fs = require('fs');
const path = require('path');

// Database configuration
const dbConfig = {
    host: process.env.DB_HOST || 'localhost',
    user: process.env.DB_USER || 'root',
    password: process.env.DB_PASSWORD || '',
    database: process.env.DB_NAME || 'portfolio_system',
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0,
    multipleStatements: true
};

// Create connection pool
const pool = mysql.createPool(dbConfig);

// Test connection
async function testConnection() {
    try {
        const connection = await pool.getConnection();
        console.log('✓ MySQL database connected successfully');
        connection.release();
        return true;
    } catch (error) {
        console.error('✗ MySQL connection failed:', error.message);
        return false;
    }
}

// Initialize database (verify tables exist)
async function initializeDatabase() {
    try {
        const connection = await pool.getConnection();
        
        // Check if tables exist
        const [tables] = await connection.query(
            "SHOW TABLES LIKE 'users'"
        );
        
        if (tables.length === 0) {
            console.error('✗ Database tables not found. Please run schema.sql first.');
            console.error('Run from web-app directory: mysql -u <user> -p < database/schema.sql');
            connection.release();
            return false;
        }
        
        console.log('✓ Database tables verified');
        connection.release();
        return true;
    } catch (error) {
        console.error('Database initialization error:', error.message);
        return false;
    }
}

module.exports = {
    pool,
    testConnection,
    initializeDatabase
};
