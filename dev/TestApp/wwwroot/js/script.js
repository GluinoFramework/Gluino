const counter = document.getElementById('counter');

function updateCounter() {
  const count = parseInt(counter.innerText);
  counter.innerText = count + 1;
}