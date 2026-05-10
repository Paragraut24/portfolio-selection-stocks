const express = require('express');
const cors = require('cors');
const path = require('path');
const { exec } = require('child_process');
const fs = require('fs');
const csv = require('csv-parser');
const multer = require('multer');
require('dotenv').config();

// Import database and auth
const { testConnection, initializeDatabase } = require('./database/db');
const authRoutes = require('./routes/auth');
const { authenticateToken, optionalAuth } = require('./middleware/auth');

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('frontend'));

// Configure multer for file uploads
const upload = multer({ dest: 'uploads/' });

// Path to C++ backend and Python scripts
const BACKEND_PATH = path.join(__dirname, '../backend');
const BACKEND_EXE = path.join(BACKEND_PATH, 'portfolio_backend.exe');
const SCRIPTS_PATH = path.join(__dirname, '../scripts');
const FETCH_SCRIPT = path.join(SCRIPTS_PATH, 'fetch_stocks.py');

// Initialize database connection
testConnection().then(async (connected) => {
    if (connected) {
        await initializeDatabase();
    } else {
        console.warn('⚠ Running without database. Authentication will not work.');
    }
});

// Authentication routes
app.use('/api/auth', authRoutes);

// API Routes

// Get all stocks (optionally authenticated)
app.get('/api/stocks', optionalAuth, (req, res) => {
    const stocksFile = path.join(BACKEND_PATH, 'stocks.csv');
    const stocks = [];
    
    fs.createReadStream(stocksFile)
        .pipe(csv())
        .on('data', (row) => {
            stocks.push({
                id: parseInt(row.id),
                name: row.name,
                cost: parseInt(row.cost),
                return_pct: parseFloat(row.expected_return_percent),
                risk: parseFloat(row.risk_score)
            });
        })
        .on('end', () => {
            res.json({ success: true, stocks });
        })
        .on('error', (err) => {
            res.status(500).json({ success: false, error: err.message });
        });
});

// Upload stocks CSV
app.post('/api/stocks/upload', upload.single('file'), (req, res) => {
    if (!req.file) {
        return res.status(400).json({ success: false, error: 'No file uploaded' });
    }
    
    const targetPath = path.join(BACKEND_PATH, 'stocks.csv');
    fs.copyFile(req.file.path, targetPath, (err) => {
        fs.unlink(req.file.path, () => {}); // Clean up uploaded file
        
        if (err) {
            return res.status(500).json({ success: false, error: err.message });
        }
        res.json({ success: true, message: 'Stocks updated successfully' });
    });
});

// Run optimization
app.post('/api/optimize', (req, res) => {
    const { budget, algorithm } = req.body;
    
    if (!budget || budget <= 0) {
        return res.status(400).json({ success: false, error: 'Invalid budget' });
    }
    
    const stocksFile = path.join(BACKEND_PATH, 'stocks.csv');
    const command = `cd "${BACKEND_PATH}" && "${BACKEND_EXE}" "${stocksFile}" ${budget}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            return res.status(500).json({ 
                success: false, 
                error: error.message,
                stderr: stderr 
            });
        }
        
        res.json({ 
            success: true, 
            message: 'Optimization completed',
            output: stdout
        });
    });
});

// Get results for specific algorithm
app.get('/api/results/:algorithm', (req, res) => {
    const algorithm = req.params.algorithm;
    const resultFile = path.join(BACKEND_PATH, `portfolio_results_${algorithm}.csv`);
    
    if (!fs.existsSync(resultFile)) {
        return res.status(404).json({ 
            success: false, 
            error: 'Results not found. Run optimization first.' 
        });
    }
    
    const results = [];
    fs.createReadStream(resultFile)
        .pipe(csv())
        .on('data', (row) => {
            results.push(row);
        })
        .on('end', () => {
            res.json({ success: true, algorithm, results });
        })
        .on('error', (err) => {
            res.status(500).json({ success: false, error: err.message });
        });
});

// Get algorithm comparison
app.get('/api/comparison', (req, res) => {
    const comparisonFile = path.join(BACKEND_PATH, 'algorithm_comparison.csv');
    
    if (!fs.existsSync(comparisonFile)) {
        return res.status(404).json({ 
            success: false, 
            error: 'Comparison not found. Run optimization first.' 
        });
    }
    
    const comparison = [];
    fs.createReadStream(comparisonFile)
        .pipe(csv())
        .on('data', (row) => {
            comparison.push(row);
        })
        .on('end', () => {
            res.json({ success: true, comparison });
        })
        .on('error', (err) => {
            res.status(500).json({ success: false, error: err.message });
        });
});

// Get diversification results
app.get('/api/diversification', (req, res) => {
    const diversFile = path.join(BACKEND_PATH, 'diversification_output.csv');
    
    if (!fs.existsSync(diversFile)) {
        return res.status(404).json({ 
            success: false, 
            error: 'Diversification results not found.' 
        });
    }
    
    const results = [];
    fs.createReadStream(diversFile)
        .pipe(csv())
        .on('data', (row) => {
            results.push(row);
        })
        .on('end', () => {
            res.json({ success: true, results });
        })
        .on('error', (err) => {
            res.status(500).json({ success: false, error: err.message });
        });
});

// Fetch real-time stock data
app.post('/api/stocks/fetch', (req, res) => {
    const { source = 'mock' } = req.body;
    
    // Validate source
    const validSources = ['mock', 'alpha_vantage', 'yahoo'];
    if (!validSources.includes(source)) {
        return res.status(400).json({ 
            success: false, 
            error: `Invalid source. Must be one of: ${validSources.join(', ')}` 
        });
    }
    
    const command = `python "${FETCH_SCRIPT}" --source ${source}`;
    
    exec(command, { cwd: SCRIPTS_PATH }, (error, stdout, stderr) => {
        if (error) {
            return res.status(500).json({ 
                success: false, 
                error: error.message,
                stderr: stderr 
            });
        }
        
        try {
            const result = JSON.parse(stdout);
            res.json(result);
        } catch (parseError) {
            res.json({ 
                success: true, 
                message: 'Fetch completed',
                output: stdout 
            });
        }
    });
});

// Get Power BI export data
app.get('/api/powerbi/export', (req, res) => {
    const files = {
        stocks: path.join(BACKEND_PATH, 'stocks.csv'),
        results_2d: path.join(BACKEND_PATH, 'portfolio_results_2d.csv'),
        results_1d: path.join(BACKEND_PATH, 'portfolio_results_1d.csv'),
        results_greedy: path.join(BACKEND_PATH, 'portfolio_results_greedy.csv'),
        results_fractional: path.join(BACKEND_PATH, 'portfolio_results_fractional.csv'),
        comparison: path.join(BACKEND_PATH, 'algorithm_comparison.csv'),
        diversification: path.join(BACKEND_PATH, 'diversification_output.csv')
    };
    
    const exportData = {
        success: true,
        files: {},
        timestamp: new Date().toISOString()
    };
    
    // Check which files exist
    for (const [key, filePath] of Object.entries(files)) {
        exportData.files[key] = {
            exists: fs.existsSync(filePath),
            path: filePath
        };
    }
    
    res.json(exportData);
});

// Start server
app.listen(PORT, () => {
    console.log(`Portfolio Selection Server running on http://localhost:${PORT}`);
    console.log(`Backend path: ${BACKEND_PATH}`);
});
