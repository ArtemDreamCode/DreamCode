const findLocalDevices = require('local-devices')
const http = require('http')
const path = require('path')
const fs = require('fs')
let globalSocket
// process.on('uncaughtException', function (err) {
//     // console.log(err);
// }); 
const io = require("socket.io")(3000)//3000

http.createServer((req, res) => {
	let filePath = '.' + req.url;
	    if (filePath == './')
	        filePath = './index.html';
	fs.readFile(filePath, function (err,data) {
	    let extname = path.extname(filePath);
	    let contentType = 'text/html';
	    switch (extname) {
	        case '.js':
	            contentType = 'text/javascript';
	            break;
	    }
	    console.log(contentType)
	 	res.writeHead(200);
		res.end(data)
	})
}).listen(3001)// 3001


let dictionary = new Map(),
	getDevices = async () => {
		return await findLocalDevices('172.20.10.0/24')
	//	return await findLocalDevices('192.168.0.1/24')
	},
	checkRequest = async (ip, responseText, result) => {
		return new Promise((resolve, reject) => {
			try {
				result.value = []
				const req = http.get('http://' + ip + '/state', res => {					
					res.on("data", function(chunk) {
						  var jsonData = chunk; 
						  console.log("jsonData: " + jsonData);
						  var jsonParsed = JSON.parse(jsonData);
						  console.log("jsonParsed.state.state: " + jsonParsed.state);
						  console.log("jsonParsed.state.class: " + jsonParsed.class);
						  console.log("jsonParsed.state.name: " + jsonParsed.name);
					    if (jsonParsed.class == responseText) {					
						  result.state = jsonParsed.state							   				
                          result.class = jsonParsed.class
                          result.name = jsonParsed.name 
							resolve(true)
							return true;
					    } else {
							console.log(ip + "  not equal")
					    	resolve(false)
					    	return false;
					    }
					});
				}).on('error', function(e) {
					resolve(false)
				   console.log(ip + "  Got error: " + e.message);
				}).on('socket', function (socket) {
    socket.setTimeout(10000);  
    socket.on('timeout', function() {
		console.log(ip + "  Check timeout, socket abort");
        req.abort();
    });
});
				
			} catch (e) {
				 console.log("err", e)
			}
	    	//reject(false)
		})
	},
	relayRequest = async (ip, turn) => {
		console.log(ip, turn);
		return new Promise((resolve, reject) => {
			try {
				
				//const url =  `http://${ip}/relay?turn=${turn}`;
				const url = 'http://'+ ip +'/relay?turn='+ turn;
				console.log('Sending url ', url);
				const req = http.get(url, (res) => {
				//const req = http.get({hostname: `http://${ip}`, path:`/relay/?turn=${turn}`}, res => {
					res.on("data", function(chunk) {
					    //console.log("BODY: " + chunk);
		    });
				}).on('error', function(e) {
				  console.log(ip + "  Got error: " + e.message);
				});
			} catch (e) {
				console.log(ip + "  err", e)
			}
			resolve(false)
		})
	},
	relayChangeName = async (ip, name) => {
		console.log(ip, name);
		return new Promise((resolve, reject) => {
			try {
				
				const url =  `http://${ip}/set?name=${name}`;
				console.log('Sending url ', url);
				const req = http.get(url, (res) => {
				//const req = http.get({hostname: `http://${ip}`, path:`/relay/?turn=${turn}`}, res => {
					res.on("data", function(chunk) {
					    //console.log("BODY: " + chunk);
		    });
				}).on('error', function(e) {
				  console.log(ip + "  Got error: " + e.message);
				});
			} catch (e) {
				console.log(ip + "  err", e)
			}
			resolve(false)
		})
	}, 
	relayChangeState = async (ip) => {
		console.log(ip);
		return new Promise((resolve, reject) => {
			try {
				
				const url =  `http://${ip}/state`;
				console.log('Sending url ', url);
				const req = http.get(url, (res) => {
				//const req = http.get({hostname: `http://${ip}`, path:`/relay/?turn=${turn}`}, res => {
					res.on("data", function(chunk) {
					    //console.log("BODY: " + chunk);
		    });
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
		try{
			devices = await getDevices()
		}
		catch (e){
			console.log("err in fetch devices", e)
			return false;
		}
		  devices.forEach(device => {
			if (!dictionary.has(device.ip)) {
				dictionary.set(device.ip, device)
			}
		})
		dictionary.forEach(record => {
			if (!devices.find(device => device.ip == record.ip)) {
				dictionary.delete(record.ip)
				console.log(`zombie found: ${record.ip}`)
			} else {
				console.log(`device ${record.ip} is available`)
			}
		})
		console.log("zombies cleared")
		dictionary.forEach(async device => {
			let checkResult = false
			var DeviceState = []
			try {
				checkResult = await checkRequest(device.ip, "Shelly", DeviceState)
				//console.log("checkResult " + checkResult)
				if (checkResult) {
					device.pinged = true
					device.state = DeviceState.state
					device.name = DeviceState.name
					device.class = DeviceState.class				
				} else {
					device.pinged = false
				}
				
			//	console.log("device.pinged" + device.pinged)
 			} catch (e) { 
				console.log(e)
				 //device.pinged = false
			}
		//	console.log(device.ip, checkResult, device.state )
		io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
		})
		console.log(dictionary)
	}, 2000)// refr page
	
io.on('connection', socket => {
	globalSocket = socket
	socket.on("relay", async data => {
		let turnResult = await relayRequest(data.ip, data.turn)
		console.log('turn result', turnResult)
//		io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
	})
	socket.on("ChangeName", async data => {
		let tResult = await relayChangeName(data.ip, data.name)
		//console.log('setname result', data.name)
	})
	socket.on("ChangeState", async data => {
		let tResult = await relayChangeState(data.ip)
		//console.log('setname result', data.name)
	})
	socket.on("deviceReq", (response) => {
		response(Array.from(dictionary.values()))
	})
})
