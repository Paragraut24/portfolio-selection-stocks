// API Configuration
const API_BASE = 'http://localhost:3000/api';

// Utility Functions
function showMessage(elementId, message, type = 'info') {
    const element = document.getElementById(elementId);
    if (element) {
        element.innerHTML = `<div class="message ${type}">${message}</div>`;
    }
}

function showLoading(elementId, message = 'Loading...') {
    const element = document.getElementById(elementId);
    if (element) {
        element.innerHTML = `
            <div class="loading">
                <div class="spinner"></div>
                <p>${message}</p>
            </div>
        `;
    }
}

// Format currency
function formatCurrency(amount) {
    return `Rs.${parseFloat(amount).toFixed(2)}`;
}

// Format percentage
function formatPercent(value) {
    return `${parseFloat(value).toFixed(2)}%`;
}

// API Calls
async function fetchStocks() {
    const response = await fetch(`${API_BASE}/stocks`);
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data.stocks;
}

async function uploadStocks(file) {
    const formData = new FormData();
    formData.append('file', file);
    
    const response = await fetch(`${API_BASE}/stocks/upload`, {
        method: 'POST',
        body: formData
    });
    
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data;
}

async function fetchStocksFromAPI(source = 'mock') {
    const response = await fetch(`${API_BASE}/stocks/fetch`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ source })
    });
    
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data;
}

async function runOptimization(budget, algorithm) {
    const response = await fetch(`${API_BASE}/optimize`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ budget, algorithm })
    });
    
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data;
}

async function fetchResults(algorithm) {
    const response = await fetch(`${API_BASE}/results/${algorithm}`);
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data.results;
}

async function fetchComparison() {
    const response = await fetch(`${API_BASE}/comparison`);
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data.comparison;
}

async function fetchDiversification() {
    const response = await fetch(`${API_BASE}/diversification`);
    const data = await response.json();
    if (!data.success) throw new Error(data.error);
    return data.results;
}

// Chart.js Helper Functions
function createPieChart(canvasId, labels, data, title) {
    const ctx = document.getElementById(canvasId);
    if (!ctx) return;
    
    new Chart(ctx, {
        type: 'pie',
        data: {
            labels: labels,
            datasets: [{
                data: data,
                backgroundColor: [
                    '#667eea', '#764ba2', '#f093fb', '#4facfe',
                    '#43e97b', '#fa709a', '#fee140', '#30cfd0',
                    '#a8edea', '#fed6e3'
                ]
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                title: {
                    display: true,
                    text: title,
                    font: { size: 16 }
                },
                legend: {
                    position: 'bottom'
                }
            }
        }
    });
}

function createBarChart(canvasId, labels, datasets, title, yAxisLabel) {
    const ctx = document.getElementById(canvasId);
    if (!ctx) return;
    
    new Chart(ctx, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: datasets
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                title: {
                    display: true,
                    text: title,
                    font: { size: 16 }
                },
                legend: {
                    position: 'top'
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: yAxisLabel
                    }
                }
            }
        }
    });
}

function createLineChart(canvasId, labels, datasets, title, yAxisLabel) {
    const ctx = document.getElementById(canvasId);
    if (!ctx) return;
    
    new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: datasets
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                title: {
                    display: true,
                    text: title,
                    font: { size: 16 }
                },
                legend: {
                    position: 'top'
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: yAxisLabel
                    }
                }
            }
        }
    });
}

// Export functions for use in other pages
window.API = {
    fetchStocks,
    uploadStocks,
    fetchStocksFromAPI,
    runOptimization,
    fetchResults,
    fetchComparison,
    fetchDiversification
};

window.Utils = {
    showMessage,
    showLoading,
    formatCurrency,
    formatPercent
};

window.Charts = {
    createPieChart,
    createBarChart,
    createLineChart
};
