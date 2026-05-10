@echo off
echo ========================================
echo Portfolio Selection System - Startup
echo ========================================
echo.

REM Check if .env exists
if not exist .env (
    echo [ERROR] .env file not found!
    echo.
    echo Creating .env from template...
    copy .env.example .env
    echo.
    echo [ACTION REQUIRED] Please edit .env file with your MySQL password
    echo Run: notepad .env
    echo Then run this script again.
    pause
    exit /b 1
)

echo [1/4] Checking Node.js...
node --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Node.js is not installed!
    echo Please install from: https://nodejs.org/
    pause
    exit /b 1
)
echo [OK] Node.js found

echo.
echo [2/4] Checking MySQL...
mysql --version >nul 2>&1
if errorlevel 1 (
    echo [WARNING] MySQL command not found in PATH
    echo Make sure MySQL is installed and running
    echo.
)

echo.
echo [3/4] Installing dependencies...
if not exist node_modules (
    echo Installing npm packages...
    call npm install
    if errorlevel 1 (
        echo [ERROR] npm install failed!
        pause
        exit /b 1
    )
) else (
    echo [OK] Dependencies already installed
)

echo.
echo [4/4] Starting server...
echo.
echo ========================================
echo Server will start on http://localhost:3000
echo.
echo To create account: http://localhost:3000/signup.html
echo To login: http://localhost:3000/login.html
echo.
echo Press Ctrl+C to stop the server
echo ========================================
echo.

call npm start
