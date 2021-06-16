const findLocalDevices = require('local-devices')
const http = require('http')
const fs = require('fs')
let globalSocket
// process.on('uncaughtException', function (err) {
//     // console.log(err);
// }); 
const io = require("socket.io")(3000)//3000

  fs.readFile('./index.html', (err, html) => {
  	if (err) {
        throw err; 
    }  
	http.createServer((req, res) => {
	 	res.writeHead(200, { 'Content-Type': 'text/html' });
		res.write(html)
		res.end()
	}).listen(3001)// 3001
  })

io.on('connection', socket => {
	globalSocket = socket
	socket.on("relay", async data => {
		let turnResult = await relayRequest(data.ip, data.turn)
		console.log('turn result', turnResult)
	})
	//console.log("new socket conn", socket)
})
let dictionary = new Map(),
	getDevices = async () => {
		return await findLocalDevices('172.20.10.0/24')
	},
	checkRequest = async (ip, responseText) => {
		return new Promise((resolve, reject) => {
			try {
				const req = http.get({hostname: ip}, res => {
					//console.log(`statusCode: ${res.statusCode}`)
					res.on("data", function(chunk) {
					    console.log("BODY: " + chunk, chunk.indexOf(responseText), responseText );
					    if (chunk.indexOf(responseText) >= 0) {
							resolve(true)
							return true;
					    } else {
							console.log("not equal")
					    	reject(false)
					    	return false;
					    }
					});
				}).on('error', function(e) {
				  // console.log("Got error: " + e.message);
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
					    console.log("BODY: " + chunk);
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
		let devices = await getDevices()
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
			device.pinged = false
			try {
				checkResult = await checkRequest(device.ip, "Shelly")
				console.log("checkResult " + checkResult)
				if (checkResult) {
					device.pinged = true
				} else {
					device.pinged = false
				}
				console.log("device.pinged" + device.pinged)
				//globalSocket.emit
				//globalSocket.emit
				globalSocket.emit("devices", Array.from(dictionary.values()))
			} catch (e) { 
				console.log(e)
				device.pinged = false
			}
			//console.log(checkResult)
		})
		if (globalSocket) {
			globalSocket.emit("devices", Array.from(dictionary.values()))
		}
	}, 3000)// refr page
