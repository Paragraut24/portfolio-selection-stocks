@echo off
echo ========================================
echo Database Setup Script
echo ========================================
echo.

echo This script will create the portfolio_system database
echo and all required tables.
echo.
echo You will need your MySQL root password.
echo.
pause

echo.
echo Running database schema...
echo.

mysql -u root -p < database\schema.sql

if errorlevel 1 (
    echo.
    echo [ERROR] Database setup failed!
    echo.
    echo Common issues:
    echo 1. MySQL is not running
    echo 2. Wrong password
    echo 3. MySQL not in PATH
    echo.
    echo To fix:
    echo - Start MySQL service
    echo - Check your password
    echo - Add MySQL to system PATH
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo [SUCCESS] Database created successfully!
echo ========================================
echo.
echo Database: portfolio_system
echo Tables created:
echo   - users
echo   - stocks (with 10 sample stocks)
echo   - portfolios
echo   - portfolio_stocks
echo   - optimization_history
echo   - sessions
echo.
echo Next step: Run start.bat to start the server
echo.
pause
