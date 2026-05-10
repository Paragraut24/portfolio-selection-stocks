const express = require('express');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');
const { pool } = require('../database/db');
const { JWT_SECRET, authenticateToken } = require('../middleware/auth');

const router = express.Router();
const SALT_ROUNDS = 10;

// Sign up
router.post('/signup', async (req, res) => {
    const { name, email, password } = req.body;
    
    // Validation
    if (!name || !email || !password) {
        return res.status(400).json({ 
            success: false, 
            error: 'All fields are required' 
        });
    }
    
    if (password.length < 6) {
        return res.status(400).json({ 
            success: false, 
            error: 'Password must be at least 6 characters' 
        });
    }
    
    try {
        // Check if user already exists
        const [existing] = await pool.query(
            'SELECT id FROM users WHERE email = ?',
            [email]
        );
        
        if (existing.length > 0) {
            return res.status(400).json({ 
                success: false, 
                error: 'Email already registered' 
            });
        }
        
        // Hash password
        const passwordHash = await bcrypt.hash(password, SALT_ROUNDS);
        
        // Insert user
        const [result] = await pool.query(
            'INSERT INTO users (name, email, password_hash) VALUES (?, ?, ?)',
            [name, email, passwordHash]
        );
        
        res.json({ 
            success: true, 
            message: 'Account created successfully',
            userId: result.insertId
        });
        
    } catch (error) {
        console.error('Signup error:', error);
        res.status(500).json({ 
            success: false, 
            error: 'Registration failed. Please try again.' 
        });
    }
});

// Login
router.post('/login', async (req, res) => {
    const { email, password, remember } = req.body;
    
    if (!email || !password) {
        return res.status(400).json({ 
            success: false, 
            error: 'Email and password are required' 
        });
    }
    
    try {
        // Get user
        const [users] = await pool.query(
            'SELECT * FROM users WHERE email = ?',
            [email]
        );
        
        if (users.length === 0) {
            return res.status(401).json({ 
                success: false, 
                error: 'Invalid email or password' 
            });
        }
        
        const user = users[0];
        
        // Check if account is active
        if (!user.is_active) {
            return res.status(403).json({ 
                success: false, 
                error: 'Account is disabled' 
            });
        }
        
        // Verify password
        const isValid = await bcrypt.compare(password, user.password_hash);
        
        if (!isValid) {
            return res.status(401).json({ 
                success: false, 
                error: 'Invalid email or password' 
            });
        }
        
        // Generate JWT token
        const expiresIn = remember ? '30d' : '24h';
        const token = jwt.sign(
            { userId: user.id, email: user.email },
            JWT_SECRET,
            { expiresIn }
        );
        
        // Calculate expiration date
        const expiresAt = new Date();
        expiresAt.setDate(expiresAt.getDate() + (remember ? 30 : 1));
        
        // Store session
        await pool.query(
            'INSERT INTO sessions (user_id, token, expires_at) VALUES (?, ?, ?)',
            [user.id, token, expiresAt]
        );
        
        // Update last login
        await pool.query(
            'UPDATE users SET last_login = NOW() WHERE id = ?',
            [user.id]
        );
        
        res.json({ 
            success: true, 
            token,
            user: {
                id: user.id,
                name: user.name,
                email: user.email
            }
        });
        
    } catch (error) {
        console.error('Login error:', error);
        res.status(500).json({ 
            success: false, 
            error: 'Login failed. Please try again.' 
        });
    }
});

// Logout
router.post('/logout', authenticateToken, async (req, res) => {
    try {
        // Delete session
        await pool.query(
            'DELETE FROM sessions WHERE token = ?',
            [req.token]
        );
        
        res.json({ 
            success: true, 
            message: 'Logged out successfully' 
        });
    } catch (error) {
        console.error('Logout error:', error);
        res.status(500).json({ 
            success: false, 
            error: 'Logout failed' 
        });
    }
});

// Get current user
router.get('/me', authenticateToken, (req, res) => {
    res.json({ 
        success: true, 
        user: req.user 
    });
});

// Verify token
router.get('/verify', authenticateToken, (req, res) => {
    res.json({ 
        success: true, 
        valid: true,
        user: req.user
    });
});

module.exports = router;
