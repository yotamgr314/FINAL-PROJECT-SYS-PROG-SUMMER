// Logout functionality
document.getElementById('logoutBtn').addEventListener('click', function() {
    // Redirect to log out route
    window.location.href = '/logout';
});

// Debounce function to prevent sending too many requests at once
function debounce(func, delay) {
    let timer;
    return function(...args) {
        clearTimeout(timer);
        timer = setTimeout(() => {
            func.apply(this, args);
        }, delay);
    };
}

const username = new URLSearchParams(window.location.search).get("username");

// Fetch the user profile
function fetchUserProfile() {
    fetch(`/profileinfo?username=${username}`, {
        method: 'GET',
        headers: {
            'Content-Type': 'text/plain'
        }
    })
    .then(response => response.text())
    .then(data => {
        document.getElementById('profileText').value = data;
    })
    .catch(error => {
        console.error('Error fetching profile:', error);
    });
}

// Update the user profile
function updateUserProfile() {
    const profileText = document.getElementById('profileText').value;

    fetch(`/profileinfo?username=${username}`, {
        method: 'POST',
        headers: {
            'Content-Type': 'text/plain'
        },
        body: "\\" + profileText
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            console.log('Profile updated successfully');
        } else {
            console.log('Error updating profile');
        }
    })
    .catch(error => {
        console.error('Error updating profile:', error);
    });
}

// Fetch user profile when the page loads
window.onload = function() {
    fetchUserProfile();
};

// Update the profile text area with debounce (1-second delay)
const profileTextArea = document.getElementById('profileText');
profileTextArea.addEventListener('input', debounce(updateUserProfile, 1000));
