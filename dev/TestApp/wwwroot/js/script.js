const counter = document.getElementById('counter');

const { sendMessage, addListener } = window.gluino;

function updateCounter() {
  const count = parseInt(counter.innerText);
  counter.innerText = count + 1;

  sendMessage(`Counter: ${count + 1}`);

  console.log(window.gluino);
}

async function testBind() {
  const result = await window.gluino.test('this is arg 1', 'this is arg 2');
  console.log(result);
}
