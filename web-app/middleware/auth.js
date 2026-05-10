const jwt = require('jsonwebtoken');
const { pool } = require('../database/db');

const JWT_SECRET = process.env.JWT_SECRET || 'your-secret-key-change-in-production';

// Middleware to verify JWT token
async function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1]; // Bearer TOKEN
    
    if (!token) {
        return res.status(401).json({ 
            success: false, 
            error: 'Access token required' 
        });
    }
    
    try {
        // Verify token
        const decoded = jwt.verify(token, JWT_SECRET);
        
        // Check if session exists and is valid
        const [sessions] = await pool.query(
            'SELECT * FROM sessions WHERE token = ? AND expires_at > NOW()',
            [token]
        );
        
        if (sessions.length === 0) {
            return res.status(401).json({ 
                success: false, 
                error: 'Invalid or expired token' 
            });
        }
        
        // Get user data
        const [users] = await pool.query(
            'SELECT id, name, email, is_active FROM users WHERE id = ?',
            [decoded.userId]
        );
        
        if (users.length === 0 || !users[0].is_active) {
            return res.status(401).json({ 
                success: false, 
                error: 'User not found or inactive' 
            });
        }
        
        // Attach user to request
        req.user = users[0];
        req.token = token;
        next();
    } catch (error) {
        if (error.name === 'JsonWebTokenError') {
            return res.status(403).json({ 
                success: false, 
                error: 'Invalid token' 
            });
        }
        if (error.name === 'TokenExpiredError') {
            return res.status(401).json({ 
                success: false, 
                error: 'Token expired' 
            });
        }
        return res.status(500).json({ 
            success: false, 
            error: 'Authentication error' 
        });
    }
}

// Optional authentication (doesn't fail if no token)
async function optionalAuth(req, res, next) {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1];
    
    if (!token) {
        req.user = null;
        return next();
    }
    
    try {
        const decoded = jwt.verify(token, JWT_SECRET);
        const [users] = await pool.query(
            'SELECT id, name, email FROM users WHERE id = ? AND is_active = TRUE',
            [decoded.userId]
        );
        
        req.user = users.length > 0 ? users[0] : null;
    } catch (error) {
        req.user = null;
    }
    
    next();
}

module.exports = {
    authenticateToken,
    optionalAuth,
    JWT_SECRET
};
