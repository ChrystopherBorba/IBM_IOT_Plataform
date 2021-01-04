
var app = require('express')(); // framework de servidor do node.js
var server = require('http').Server(app); // Criação do servidor
var io = require('socket.io')(server); // biblioteca de WebSocket
var Client = require("ibmiotf"); // biblioteca responsável pela conexão com o IBM Cloud

var appClientConfig = { 
    "org": "xxxxxx", //ID da organização 
    "id": "xxxxx", //ID do dispositivo
    "domain": "internetofthings.ibmcloud.com", // Dominio do IBM Cloud
    "auth-key": "x-xxxxxx-xxxxxxxxxx", // CHAVE API (Credenciais de aplicação)
    "auth-token": "XXXXXXXXXXXXXXXXXX", // Token de autenticação (Credenciais de aplicação)
    "enforce-ws": true // necessário quando queremos utilizar os métodos do ibmiotf por meio do websocket
} // parâmetros gerais da conexão, esses valores são retirados do IBM CLoud após cadastrar dispositivo e aplicação

app.get('/', function (req, res) {
    res.sendFile(__dirname + '/index.html');
}); // Rota da pagina principal

io.on('connection', function (socket) { // conexão web socket
    var appClient = new Client.IotfApplication(appClientConfig); // atribui parametros do IBM cloud
    appClient.connect(); // conecta com IBM Cloud
    appClient.on("connect", function () {
        appClient.subscribeToDeviceEvents("Tipo de dispositivo", "ID do dispositivo", "+", "json"); //seta parametros de subscribe
    });
    appClient.on("deviceEvent", function (deviceType, deviceId, eventType, format, payload) {
        var msg = JSON.parse(payload); // envia mensagem a cada evento gerado pelo dispositivo (real-time)
        socket.emit('announcements', msg); // é enviado os dados a página web por meio do websocket
        console.log(msg); // imprime a mesma mensagem no console
    });
});

server.listen(8080); //  servidor na porta na url: http://localhost:8080/

/* referências:

 ibmiotf: https://www.npmjs.com/package/ibmiotf
 webSocket: https://stackabuse.com/node-js-websocket-examples-with-socket-io/
 
*/



