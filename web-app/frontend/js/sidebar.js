// Sidebar Navigation Generator
function generateSidebar(activePage) {
    const pages = [
        { id: 'index', icon: '🏠', label: 'Dashboard', sublabel: 'Overview', file: 'index.html' },
        { id: 'stocks', icon: '📊', label: 'Stock Data', sublabel: 'Add Records', file: 'stocks.html' },
        { id: 'optimizer', icon: '⚡', label: 'Optimizer', sublabel: 'Run & View Results', file: 'optimizer.html' },
        { id: 'analytics', icon: '📈', label: 'Analytics', sublabel: 'Charts & Power BI', file: 'analytics.html' },
        { id: 'analysis', icon: '🔬', label: 'Analysis', sublabel: 'Algorithm Compare', file: 'analysis.html' }
    ];
    
    const sidebar = document.createElement('div');
    sidebar.className = 'sidebar';
    
    // Header
    sidebar.innerHTML = `
        <div class="sidebar-header">
            <div class="sidebar-logo">
                <div class="logo-icon">P</div>
                <div class="logo-text">
                    <h2>Portfolio AI</h2>
                    <p>Smart Optimization</p>
                </div>
            </div>
        </div>
        
        <nav class="sidebar-nav">
            ${pages.map(page => `
                <a href="${page.file}" class="nav-item ${page.id === activePage ? 'active' : ''}">
                    <span class="nav-icon">${page.icon}</span>
                    <div class="nav-label">
                        ${page.label}
                        <span class="nav-sublabel">${page.sublabel}</span>
                    </div>
                </a>
            `).join('')}
        </nav>
        
        <div class="sidebar-footer">
            <button onclick="logout()" class="logout-button">
                <span class="logout-icon">🚪</span>
                <span>Logout</span>
            </button>
        </div>
    `;
    
    document.body.insertBefore(sidebar, document.body.firstChild);
}

// Auto-detect current page and generate sidebar
document.addEventListener('DOMContentLoaded', () => {
    const currentPage = window.location.pathname.split('/').pop().replace('.html', '') || 'index';
    generateSidebar(currentPage);
});
