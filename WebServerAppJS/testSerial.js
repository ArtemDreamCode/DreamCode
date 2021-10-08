const SerialPort = require('serialport');
const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
  delimiter: '\n'
});

const port = new SerialPort('Com7', {
  baudRate: 115200
});

port.pipe(parser);

//parser.on('data', console.log);
parser.on('data', function (data) {
	let str = data.toString(); //Convert to string
    console.log(str);
	str = str.replace(/\r?\n|\r/g, ""); //remove '\r' from this String
    str = JSON.stringify(data); // Convert to JSON
    str = JSON.parse(data); //Then parse it

    console.log(str.name);
    console.log(str.ip);
    console.log(str.ident);
    console.log(str.state);

})

setInterval(function(){ port.write("Hello"); }, 3000);