const WebSocket = require('ws');

const ws = new WebSocket('ws://localhost:3000');

ws.on('open', () => {
  setInterval(() => {
    for (let i = 1; i <= 7; i++) {
      const velocita = (Math.random() * 100).toFixed(2);
      ws.send(JSON.stringify({ sensore: i, velocita }));
    }
  }, 4000);
});
