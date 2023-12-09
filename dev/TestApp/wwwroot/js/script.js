const counter = document.getElementById('counter');

function updateCounter() {
  const count = parseInt(counter.innerText);
  counter.innerText = count + 1;

  Gluino.sendMessage(`Counter: ${count + 1}`);

  console.log(Gluino);
}

Gluino.addListener(function (e) {
  console.log(e);
});
