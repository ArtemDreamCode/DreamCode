const { app, BrowserWindow } = require('electron');

const http = require('http')
const path = require('path')
const fs = require('fs')
const url = require('url');
let globalSocket
let Device_GUID = "dDf5FFShellysde";
let Device_New_Name = "New Robotic Device";
let Plagin_GUID = "DFDDSDFDF454DFdfd";
let Device_Class = "Shelly";
let FirstGetData = true;
var mainWindow;

///*
//const SerialPort = require('serialport');
//const parsers = SerialPort.parsers;
///*
//const parser = new parsers.Readline({
// delimiter: '\n'
//});

//const port = new SerialPort('Com7', {
// baudRate: 115200
//});

//port.pipe(parser);

/*parser.on('data', console.log);
parser.on('data', function (data) {
	let str = data.toString(); //Convert to string
    console.log("from web intrfce" + str);
	str = str.replace(/\r?\n|\r/g, ""); //remove '\r' from this String
    str = JSON.stringify(data); // Convert to JSON
    str = JSON.parse(data); //Then parse it

    console.log(str.name);
    console.log(str.ip);
    console.log(str.ident);
    console.log(str.state);

})//*/



const io = require("socket.io")(3000, {
 cors: {
                origins: ["*"],
                headers: ["*"]
        }
})//3000


app.on('ready', function() {
	mainWindow = new BrowserWindow({
				width: 800,
				height: 800,
				frame: true,
				webPreferences: {
					nodeIntegration: true,
					enableRemoteModule: true,
					contextIsolation: false
				}
			});
			//mainWindowd.maximize();
			mainWindow.webContents.openDevTools();
			mainWindow.show();
		//	mainWindow.loadURL("http://localhost:8080"); // look fict load page !!

 })

let dictionary = new Map();
let dictionary_new = new Map();
let dictionary_old = new Map();
let dictionary_new_com = new Map();


http.createServer((req, res) => {
	
	let filePath = '.' + req.url;
	
	  if(req.url === "/state"){
	//	console.log("state");
		var jsData = JSON.stringify(Array.from(dictionary.values()))
        res.end(jsData);
	  }
	    if (filePath == './')
	        filePath = './index.html';
			//filePath = './keyboard.html';

	    fs.readFile(filePath, function (err,data) {
	    let extname = path.extname(filePath);
	    let contentType = 'text/html';
	    switch (extname) {
	        case '.js':
	            contentType = 'text/javascript';
	            break;
	    }
	 //   console.log(contentType)
	 	res.writeHead(200);
		res.end(data)
	
	})
}).listen(3001)// 3001


const Arpping = require('arpping');
const arpping = new Arpping({
	debug: true
});

/*
  receivedComPortData = () => {
	 let serialPort = new SerialPort("Com7", {
		baudRate: 115200
		});
		
    serialPort.on("open", function() {
		
       console.log('serial open: ');
     });
	
	 // let parser = new ReadLine();
	//	serialPort.pipe(parser);
		
	//	parser.on('data', function() {
			
	//	   console.log('serial data: ', data);
	//	 });
		 
	//	 parser.on('error' , function() {
//		   console.log('error parser read Com3: ');
	//	 });
	
     serialPort.on('data', function(data) {
		console.log('serial received: ' + data);
		if (data.indexOf("getInfo") >= 0) {
				//let myJson = {};
				//myJson.dictionary_new = Array.from(dictionary_new.values());
				//myJson.dictionary_old = Array.from(dictionary_old.values());
//				myJson.state = "123456789/123456789/123456789/123456789/123456789/123456789/123456789/123456789/123456789/123456789/";
				
			// let json = JSON.stringify(myJson);
			let json = "{"
			for (let i=0;i<=9;i++){
//			let k;
//			if (i == 9) {
//				k = ""
//			}else {k = ","}
			
			dictionary_new.forEach(async record => {
			   // let myJson = {};
			//	let 
			  //  myJson.name = record.name;
			//	myJson.ip = record.ip;
			//	let json = JSON.stringify(myJson);
			 
			    json += "\"name\": \""+record.name+"\", \"ip\": \""+record.ip+"\"";
				serialPort.write(json, function(err) {
				 if (err) {
			       return console.log('Error on write: ', err.message);
				 }
				   console.log('message written ', json);
				  });
				 })
		 
			}
			json += "}"
			serialPort.write(json, function(err) {
				 if (err) {
			       return console.log('Error on write: ', err.message);
				 }
				   console.log('message written ', json);
				  });
			
		}
		
		if (data.indexOf("ip:") >= 0) {
            console.log('message written ', data);
		}
     });
	 	 	 
	 serialPort.on('error' , function() {
       console.log('error read Com3: ');
     })
	 
	 
   };
  */
const getIPRange = require('get-ip-range');

//let dictionary = new Map(),
	getDevices = async () => {
	//	return await findLocalDevices('172.20.10.0/24')
	//	return await findLocalDevices('192.168.0.1/24')
	//	return await findLocalDevices('192.168.1.2/24')
   let {hosts} = await arpping.ping(getIPRange('192.168.0.1', '192.168.0.30'))
//   let {hosts} = await arpping.ping(getIPRange('192.168.0.1', '192.168.0.20'))
//	console.log(hosts)
	return hosts;
	}
	
  checkRequest = async (ip, result) => {
	  	  if (FirstGetData){
			mainWindow.loadURL("http://localhost:8080"); 
			FirstGetData = false;								 
		}
		return new Promise((resolve, reject) => {
			try {
				result.value = []
			//	console.log("input before get ip " + ip)
				const req = http.get('http://' + ip + '/state', res => {
                    var body = "";					
					res.on("data", function(chunk) {			
					    body += chunk;})
					
					res.on('end', function(){

////////////////////////////

        if (res.statusCode == 200) { 
            try {
                if (body != '') {
					if (body.indexOf(Device_GUID) >= 0) {
                      var jsonParsed = JSON.parse(body)
					      if (jsonParsed.class == Device_Class) {					
							  result.state = jsonParsed.state							   				
							  result.class = jsonParsed.class
							  result.name = jsonParsed.name
							  result.device_guid = jsonParsed.device_guid	
							  result.index = jsonParsed.index
							  result.isnewdevice = jsonParsed.isnewdevice;
							  resolve(true)
							  return true;
					    } else {
							console.log("not equal")
					    	resolve(false)
					    	return false;
					    }
						}else{
						resolve(false)
						return false;}	
                } else {
                    console.log('Body is empty');
					resolve(false)
					return false;
                }
            } catch (err) {
                console.log(err);
				resolve(false)
		    	return false;
            }
        } else {
            console.log('Status code: ' + res.statusCode);
		    resolve(false)
			return false;
        }
     });
  }).on('error', function(e) {
					resolve(false)
				  // console.log(ip + "  Got error: " + e.message);
				}).on('socket', function (socket) {
    socket.setTimeout(5000);  
    socket.on('timeout', function() {
		//console.log(ip + "  Check timeout, socket abort");
        req.abort();
    });
});
				
			} catch (e) {
				// console.log("err", e)
			}
	    	//reject(false)
		})
	},
	relayRequest = async (ip, turn) => {
	//	console.log(ip, turn);
		return new Promise((resolve, reject) => {
			try {
				
				//const url =  `http://${ip}/relay?turn=${turn}`;
				const url = 'http://'+ ip +'/relay?turn='+ turn;
				//console.log('Sending url ', url);
				const req = http.get(url, (res) => {
				//const req = http.get({hostname: `http://${ip}`, path:`/relay/?turn=${turn}`}, res => {
					res.on("data", function(chunk) {
					    //console.log("BODY: " + chunk);
		    });
			
			
			//io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
				}).on('error', function(e) {
				//  console.log(ip + "  Got error: " + e.message);
				});
			} catch (e) {
				//console.log(ip + "  err", e)
			}
			resolve(false)
		})
	},
	relayChangeName = async (ip, name) => {
	//	console.log(ip, name);
		return new Promise((resolve, reject) => {
			try {
				
				const url =  `http://${ip}/set?name=${name}`;
			//	console.log('Sending url ', url);
				const req = http.get(url, (res) => {
				//const req = http.get({hostname: `http://${ip}`, path:`/relay/?turn=${turn}`}, res => {
					res.on("data", function(chunk) {
					    //console.log("BODY: " + chunk);
		    });
	        dictionary.forEach(device => {
			if (device.ip === ip) {
				device.name = name
			}
			})
			
			io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
			io.sockets.emit("devices_new", Array.from(dictionary_new.values())) // auto update client push click new data
			io.sockets.emit("devices_old", Array.from(dictionary_old.values())) // auto update client push click new data
		 			
			}).on('error', function(e) {
				//  console.log(ip + "  Got error: " + e.message);
				});
			} catch (e) {
				//console.log(ip + "  err", e)
			}
			resolve(false)
		})
	},  
	
	ResetDevice = async (ip) => {
		console.log(ip);
		return new Promise((resolve, reject) => {
			try {
				
				const url =  `http://${ip}/reset`;
				console.log('Sending url ', url);
				const req = http.get(url, (res) => {
				//const req = http.get({hostname: `http://${ip}`, path:`/reset, res => {
					res.on("data", function(chunk) {
					    //console.log("BODY: " + chunk);
		    });
			
			//io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
				}).on('error', function(e) {
				  console.log(ip + "  Got error: " + e.message);
				});
			} catch (e) {
				console.log(ip + "  err", e)
			}
			resolve(false)
		})
	},  
  
	checkDevisecInterval = setInterval(async () => {
		console.log("start fetching new devices ...")
		let devices = []
		try{ //получаем все устройства в сети
			let obDevices = await getDevices()
			obDevices.forEach(device => {
				devices.push({ip: device})
			})
			console.log("devices.length: " + devices.length, devices);
		}
		catch (e){
			console.log("err in fetch devices", e)
			return false;
		}

		
		dictionary.forEach(async record => { //если устройство есть в словаре, но изсети пропало, удаляем
			
			if (!devices.find(device => device.ip == record.ip)) {
			  dictionary.delete(record.ip)
			console.log("dictionary.delete(record.ip)  " + record.ip)
			}
		}
		)
		let k = 0;
		 devices.forEach(async device => {  
				let checkResult = false
				var DeviceState = []
				try { //пробуем пингануть устройство
					checkResult = await checkRequest(device.ip, DeviceState) //пингуем каждое устройство
					
					if (checkResult) { //если ответило, заполняем структуру
						console.log("device.ip is checkresult " + device.ip);
						device.pinged = true
						device.state = DeviceState.state
						device.name = DeviceState.name
						device.class = DeviceState.class
						device.index = DeviceState.index
						device.isnewdevice = DeviceState.isnewdevice // "old" / "new"
						let s = '';
						s = device.isnewdevice;
						if ((s.indexOf('new') >= 0)) { //если устройство новое
							k+=1;			
							if(dictionary_old.has(device.ip)) {dictionary_old.delete(device.ip)} // если было старое но стало новое
							dictionary_new.set(device.ip, device);  //удаляем изстарыхи добавляем в новое
						}
						else //иначе если старое
						{   
							if(dictionary_new.has(device.ip)) {dictionary_new.delete(device.ip)} // если было новое и стало старым
							dictionary_old.set(device.ip, device); //удаляем из новых и добавляем в старое
						}
						dictionary.set(device.ip, device); //в любом случае добавляем в общую
						CountNewDev = k; 
					} 
					else { //если устройство не отвечает
						if (dictionary_new.has(device.ip)) { //и содержится в новых, удаляем
						  dictionary_new.delete(device.ip)
						}
						else if (dictionary_old.has(device.ip)) { //или содержится в старых - тоже удаляем
						  dictionary_old.delete(device.ip)
						}
					}
				} 
				catch (e) { 
					//console.log(e)
					 //device.pinged = false
				}

				if (devices.size == 0) { //если вообще нет устройств в сети
				   dictionary.clear()	
				 // console.log(" dictionary.clear()	")
				}

		})

		////////////////////////////////////////
		io.sockets.emit("devices_new", Array.from(dictionary_new.values())) // auto update client push click new data
		io.sockets.emit("devices_old", Array.from(dictionary_old.values())) // auto update client push click new data
		 
		 
		let myJson = {}; //формируем общий json из новых и старых устройств
			myJson.dictionary_new = Array.from(dictionary_new.values());
			myJson.dictionary_old = Array.from(dictionary_old.values());
			
		let json = JSON.stringify(myJson);
		console.log("send to web int: " + json)
	///*
	//	port.write(json); //отправляем по серийному порту
	//*/
		//console.log("getPortsList ", getPortsList)
		 
		// InitSerialPort();
		// receivedComPortData();
		// SerialPortWrite(json);
		console.log(dictionary)
		
	}, 3000)// refr page 
	
	/*
	TO DO
	  Сделать принудительные эммиты при отваливании устройства и при добавлении.	
	
	*/
	
	
try {
	io.on('connection', socket => {
	globalSocket = socket
	io.sockets.emit("devices_new", Array.from(dictionary_new.values())) // push click new data
	io.sockets.emit("devices_old", Array.from(dictionary_old.values())) // push click new data	
	
	socket.on("relay", async data => {
		let turnResult = await relayRequest(data.ip, data.turn)
		
		dictionary_old.forEach(device => {
			if (device.ip === data.ip) {
				device.state = data.turn
			}
			})
		dictionary_new.forEach(device => {
			if (device.ip === data.ip) {
				device.state = data.turn
			}
			})
		io.sockets.emit("devices_old", Array.from(dictionary_old.values())) 
		io.sockets.emit("devices_new", Array.from(dictionary_new.values())) 
	})
	socket.on("ChangeName", async data => {
		let tResult = await relayChangeName(data.ip, data.name)
		
		dictionary_old.forEach(device => {
			if (device.ip === data.ip) {
				device.name = data.name
			}
			})
		dictionary_new.forEach(device => {
			if (device.ip === data.ip) {
				device.name = data.name
				device.isnewdevice = "old";
		        dictionary_old.set(device.ip, device);
		        dictionary_new.delete(device.ip);					
			}
			})			
		io.sockets.emit("devices_old", Array.from(dictionary_old.values())) 
		io.sockets.emit("devices_new", Array.from(dictionary_new.values()))
			 					
	})
	
	socket.on("reset", async data => {
		let tResult = await ResetDevice(data.ip)
	//	response(Array.from(dictionary.values()))
		console.log('ResetDevice', data.ip)
		dictionary_old.forEach(device => {
			if (device.ip === data.ip) {
				device.name = Device_New_Name;
				device.isnewdevice = "new";
				dictionary_new.set(device.ip, device);
				dictionary_old.delete(device.ip);
			}
		})
		
		io.sockets.emit("devices_old", Array.from(dictionary_old.values())) 
		io.sockets.emit("devices_new", Array.from(dictionary_new.values()))
	})
	
	io.sockets.emit("devices_old", Array.from(dictionary_old.values())) 
	io.sockets.emit("devices_new", Array.from(dictionary_new.values()))

})
}
catch (e){
	//console.log("catch soccet io: ", e)
	return false;
}
