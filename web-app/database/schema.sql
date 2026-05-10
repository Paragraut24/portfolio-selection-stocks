-- Portfolio Selection System Database Schema
-- MySQL Database

CREATE DATABASE IF NOT EXISTS portfolio_system;
USE portfolio_system;

-- Users table
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    last_login TIMESTAMP NULL,
    is_active BOOLEAN DEFAULT TRUE,
    INDEX idx_email (email)
);

-- Stocks table
CREATE TABLE IF NOT EXISTS stocks (
    id INT AUTO_INCREMENT PRIMARY KEY,
    stock_id VARCHAR(50) UNIQUE NOT NULL,
    name VARCHAR(255) NOT NULL,
    cost DECIMAL(10, 2) NOT NULL,
    expected_return_percent DECIMAL(5, 2) NOT NULL,
    risk_score DECIMAL(5, 2) NOT NULL,
    sector VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_stock_id (stock_id),
    INDEX idx_name (name)
);

-- User portfolios table
CREATE TABLE IF NOT EXISTS portfolios (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    budget DECIMAL(12, 2) NOT NULL,
    algorithm VARCHAR(50) NOT NULL,
    total_cost DECIMAL(12, 2) NOT NULL,
    total_expected_return DECIMAL(12, 2) NOT NULL,
    avg_return_percent DECIMAL(5, 2) NOT NULL,
    avg_risk DECIMAL(5, 2) NOT NULL,
    runtime_ms DECIMAL(10, 3) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_user_id (user_id),
    INDEX idx_algorithm (algorithm)
);

-- Portfolio stocks (selected stocks in each portfolio)
CREATE TABLE IF NOT EXISTS portfolio_stocks (
    id INT AUTO_INCREMENT PRIMARY KEY,
    portfolio_id INT NOT NULL,
    stock_id INT NOT NULL,
    fraction DECIMAL(5, 4) DEFAULT 1.0000,
    cost DECIMAL(10, 2) NOT NULL,
    expected_return DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (portfolio_id) REFERENCES portfolios(id) ON DELETE CASCADE,
    FOREIGN KEY (stock_id) REFERENCES stocks(id) ON DELETE CASCADE,
    INDEX idx_portfolio_id (portfolio_id),
    INDEX idx_stock_id (stock_id)
);

-- Optimization history
CREATE TABLE IF NOT EXISTS optimization_history (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    budget DECIMAL(12, 2) NOT NULL,
    algorithm VARCHAR(50) NOT NULL,
    num_stocks INT NOT NULL,
    total_cost DECIMAL(12, 2) NOT NULL,
    total_return DECIMAL(12, 2) NOT NULL,
    runtime_ms DECIMAL(10, 3) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_user_id (user_id),
    INDEX idx_created_at (created_at)
);

-- User sessions (for token management)
CREATE TABLE IF NOT EXISTS sessions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    token VARCHAR(255) UNIQUE NOT NULL,
    expires_at TIMESTAMP NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_token (token),
    INDEX idx_user_id (user_id)
);

-- Insert sample stocks data
INSERT INTO stocks (stock_id, name, cost, expected_return_percent, risk_score, sector) VALUES
('INFY', 'INFY', 4154.00, 13.0, 9.0, 'Technology'),
('TCS', 'TCS', 3931.00, 12.4, 11.6, 'Technology'),
('AAPL', 'AAPL', 3219.00, 12.7, 12.1, 'Technology'),
('TSLA', 'TSLA', 1995.00, 16.7, 10.2, 'Automotive'),
('RELIANCE', 'RELIANCE', 2192.00, 8.8, 10.3, 'Energy'),
('GOOGL', 'GOOGL', 2523.00, 6.7, 12.0, 'Technology'),
('MSFT', 'MSFT', 3848.00, 15.5, 5.1, 'Technology'),
('AMZN', 'AMZN', 1068.00, 17.6, 4.4, 'E-commerce'),
('WIPRO', 'WIPRO', 2635.00, 15.8, 11.0, 'Technology'),
('HCL', 'HCL', 4259.00, 9.6, 4.0, 'Technology')
ON DUPLICATE KEY UPDATE 
    name = VALUES(name),
    cost = VALUES(cost),
    expected_return_percent = VALUES(expected_return_percent),
    risk_score = VALUES(risk_score),
    sector = VALUES(sector);
