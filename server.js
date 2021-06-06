var addon = require('./build/Release/rileyChess.node')
const express = require('express');
const ws = require('ws');
var bodyParser = require('body-parser');
const { v4: uuidv4 } = require('uuid');

const app = express();
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
app.use(function (req, res, next) {

    // Website you wish to allow to connect
    res.setHeader('Access-Control-Allow-Origin', '*');

    // Request methods you wish to allow
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');

    // Request headers you wish to allow
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');

    // Set to true if you need the website to include cookies in the requests sent
    // to the API (e.g. in case you use sessions)
    res.setHeader('Access-Control-Allow-Credentials', true);

    // Pass to next layer of middleware
    next();
});


var port = process.env.PORT;
if (port == null || port == "") {
  port = 8000;
}
const server = app.listen(port, () => {
  console.log('We are live on ' + port);
});

app.post('/getMove', function (req, res) {
    var boardRepresentation = req.body['board'];
    res.header("Access-Control-Allow-Origin", "*"); 
    res.send({
        nextMove: addon.jsGetNextMove(boardRepresentation),
        score: '200',
    })
  })

  app.get('/',function(req, res) {
    res.send('Making some changes LOL TESTING')
});


const socketSend = (socket, data) => {
  socket.send(JSON.stringify(data));
}


const wsServer = new ws.Server({ server });

var nextRoom = 1000;
var rooms = {};
var roomLookup = {};

wsServer.on('connection', socket => {
  socket.on('message', message => handleSocketMessage(message, socket));
});


const handleSocketMessage = (message, socket) => {
  const {type, data} = JSON.parse(message);
  
  if(type === 'create') {
    nextRoom += 1;
    const roomNum = nextRoom;
    roomLookup[socket] = roomNum;
    rooms[roomNum] = {
      'white': data.side === 'white' ?  socket : null,
      'black': data.side === 'black' ?  socket : null,
      'sideToPlay': true
    }
    socketSend(socket, {
      type: 'created',
      data: {
        'roomNum': roomNum
      }
    })

  }

  if(type === 'join') {
    const roomNum = data.roomNum;
    if(roomNum in rooms) {
      roomLookup[socket] = roomNum;
      rooms[roomNum]["black"] = socket; // change this later
      const toSend = {
        type: 'ready',
      }
      if(rooms[roomNum]["white"]) socketSend(rooms[roomNum]["white"], toSend);
      if(rooms[roomNum]["black"]) socketSend(rooms[roomNum]["black"], toSend);
    } else {
      socketSend(socket, {
        type: 'noRoom',
      })
    }
  }

  if(type === 'move') {
    console.log('making move!', data.move, data.side);
    const roomNum = roomLookup[socket];
    const toSend = {
      type: 'move',
      data: {
        move: data.move
      }
    }
    if(rooms[roomNum]['sideToPlay']) socketSend(rooms[roomNum]["black"], toSend) // white just made a move
    else socketSend(rooms[roomNum]["white"], toSend) // black just made a move
    rooms[roomNum]['sideToPlay'] = !rooms[roomNum]['sideToPlay'];
  }

    



}



