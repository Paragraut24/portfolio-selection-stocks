// Authentication check for protected pages
(function() {
    const API_BASE = 'http://localhost:3000/api';
    
    // Get token from storage
    function getToken() {
        return localStorage.getItem('auth_token') || sessionStorage.getItem('auth_token');
    }
    
    // Check if user is authenticated
    async function checkAuth() {
        const token = getToken();
        
        if (!token) {
            redirectToLogin();
            return false;
        }
        
        try {
            const response = await fetch(`${API_BASE}/auth/verify`, {
                headers: {
                    'Authorization': `Bearer ${token}`
                }
            });
            
            const data = await response.json();
            
            if (!data.success || !data.valid) {
                redirectToLogin();
                return false;
            }
            
            // Update user info in UI if element exists
            updateUserInfo(data.user);
            return true;
            
        } catch (error) {
            console.error('Auth check failed:', error);
            redirectToLogin();
            return false;
        }
    }
    
    // Redirect to login page
    function redirectToLogin() {
        const currentPage = window.location.pathname;
        if (!currentPage.includes('login.html') && !currentPage.includes('signup.html')) {
            window.location.href = 'login.html';
        }
    }
    
    // Update user info in UI
    function updateUserInfo(user) {
        // Add user name to header if element exists
        const userNameElement = document.getElementById('userName');
        if (userNameElement && user) {
            userNameElement.textContent = user.name;
        }
    }
    
    // Logout function
    window.logout = async function() {
        const token = getToken();
        
        if (token) {
            try {
                await fetch(`${API_BASE}/auth/logout`, {
                    method: 'POST',
                    headers: {
                        'Authorization': `Bearer ${token}`
                    }
                });
            } catch (error) {
                console.error('Logout error:', error);
            }
        }
        
        // Clear storage
        localStorage.removeItem('auth_token');
        sessionStorage.removeItem('auth_token');
        localStorage.removeItem('user_name');
        localStorage.removeItem('user_email');
        
        // Redirect to login
        window.location.href = 'login.html';
    };
    
    // Add authorization header to fetch requests
    window.authFetch = async function(url, options = {}) {
        const token = getToken();
        
        if (token) {
            options.headers = {
                ...options.headers,
                'Authorization': `Bearer ${token}`
            };
        }
        
        return fetch(url, options);
    };
    
    // Check auth on page load (skip for login/signup pages)
    const currentPage = window.location.pathname;
    if (!currentPage.includes('login.html') && !currentPage.includes('signup.html')) {
        checkAuth();
    }
})();
