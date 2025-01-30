const form = document.getElementById("userForm");
const loginBtn = document.getElementById("loginBtn");
const registerBtn = document.getElementById("registerBtn");
const messageSpan = document.getElementById("responseMessage");

const response = new URLSearchParams(window.location.search).get("response");

if (response) {
  messageSpan.style.display = "inline";
  if (response === "RegisterFailed") {
    messageSpan.textContent = "Register Failed";
    messageSpan.classList.add("text-danger");
  } else if (response === "RegisterSuccess") {
    messageSpan.textContent = "Register Success";
    messageSpan.classList.add("text-success");
  } else if (response === "LoginFailed") {
    messageSpan.textContent = "Login Failed";
    messageSpan.classList.add("text-danger");
  }
}

form.addEventListener("submit", function (event) {
  if (document.activeElement === loginBtn) {
    form.action = "/login";
  } else if (document.activeElement === registerBtn) {
    form.action = "/register";
  }
});
