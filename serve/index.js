require('dotenv').config();
var io = require('socket.io')(3001);
var mqtt = require('mqtt');

var client  = mqtt.connect('mqtt://'+process.env.BROKER);

client.on('connect', function () {
    //console.log('in');
});

io.on('connection', function(socket) {
    //console.log('a user connected');

    client.subscribe(process.env.TOPIC);
    //console.log(process.env.TOPIC);

    socket.on('disconnect', function(){
            client.unsubscribe(process.env.TOPIC);
       //console.log('user disconnected');
    });
});

client.on("message", function (topic, payload) {
    //console.log([topic, payload].join(": "));

    io.sockets.emit(process.env.TOPIC, {
        message:payload.toString()
    });
    //console.log('p: '+payload);
});
