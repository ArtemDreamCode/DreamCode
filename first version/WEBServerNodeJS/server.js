const findLocalDevices = require('local-devices')
const http = require('http')
const path = require('path')
const fs = require('fs')
const Device_GUID = 'dDf5FFShellysde';

let globalSocket
 var jsonData
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
	//	return await findLocalDevices('172.20.10.0/24')
		return await findLocalDevices('192.168.0.1/24')
	},
	checkRequest = async (ip, responseText, result) => {
		return new Promise((resolve, reject) => {
			try {
				result.value = []
				console.log("input before get ip " + ip)
				const req = http.get('http://' + ip + '/state', res => {					
					res.on("data", function(chunk) {
						if (chunk.indexOf(Device_GUID) >= 0) {
						  var jsonData = chunk; 
						  console.log("http://" + ip + "/state   :=>jsdata :  " + jsonData);
						  
						  var jsonParsed = JSON.parse(jsonData);
						  console.log("jsonParsed.state.state: " + jsonParsed.state);
						  console.log("jsonParsed.state.class: " + jsonParsed.class);
						  console.log("jsonParsed.state.name: " + jsonParsed.name);
					    if (jsonParsed.class == responseText) {	
						  result.pinged = true	
						  result.state = jsonParsed.state							   				
                          result.class = jsonParsed.class
                          result.name = jsonParsed.name
						  result.device_guid = jsonParsed.device_guid	
							resolve(true)
							return true;
					    } else {
							console.log("not equal")
					    	resolve(false)
					    	return false;
					    }
					}});
				}).on('error', function(e) {
					resolve(false)
				   console.log("Got error: " + e.message);
				}).on('socket', function (socket) {
    socket.setTimeout(1000);  
    socket.on('timeout', function() {
		console.log("Check timeout, socket abort");
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
				  console.log("Got error: " + e.message);
				});
			} catch (e) {
				console.log("err", e)
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
			//  console.log("black " + device.ip)
			if (!dictionary.has(device.ip)) {
			//	if (IPMask.test(device.ip)){
			//		console.log("white ip " + device.ip)
				  dictionary.set(device.ip, device)}
			//}
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
					device.pinged = DeviceState.pinged
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
		console.log(dictionary)
		io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
		})
	}, 2000)// время ответа сервера
	
io.on('connection', socket => {
	globalSocket = socket
	socket.on("relay", async data => {
		let turnResult = await relayRequest(data.ip, data.turn)
		console.log('turn result', turnResult)
	})
	socket.on("deviceReq", (response) => {
		response(Array.from(dictionary.values()))
	})
})
