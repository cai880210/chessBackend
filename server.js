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


const wsServer = new ws.Server({ server });

var nextRoom = 1000;
var rooms = {};
var roomLookup = {};

wsServer.on('connection', socket => {
  // assign this socket to a unique id

  socket.on('message', message => handleSocketMessage(message, socket));


});


const handleSocketMessage = (message, socket) => {
  const {type, data} = JSON.parse(message);

  if(type === 'create') {
    nextRoom += 1;
    const roomNum = nextRoom;
    rooms[roomNum] = {
      'white': data.side === "white" ?  socket : null,
      'black': data.side === "black" ?  socket : null,
    }

    socket.send(JSON.stringify({
      type: 'created',
      data: {
        'roomNum': roomNum
      }
    }))

  }



}



