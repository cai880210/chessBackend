// var addon = require('./build/Release/rileyChess.node')
const express = require('express');
var bodyParser = require('body-parser')

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
app.listen(port, () => {  console.log('We are live on ' + port);})  ;

// app.post('/getMove', function (req, res) {
//     var boardRepresentation = req.body['board'];
//     res.header("Access-Control-Allow-Origin", "*"); 
//     res.send({
//         nextMove: addon.jsGetNextMove(boardRepresentation),
//         score: '200',
//     })
//   })

  app.get('/',function(req,res) {
    res.send('Making some changes LOL TESTING')
});

