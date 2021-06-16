const findLocalDevices = require('local-devices')
const http = require('http')
const fs = require('fs')
let globalSocket
// process.on('uncaughtException', function (err) {
//     // console.log(err);
// }); 
const io = require("socket.io")(3000)

  fs.readFile('./index.html', (err, html) => {
  	if (err) {
        throw err; 
    }  
	http.createServer((req, res) => {
	 	res.writeHead(200, { 'Content-Type': 'text/html' });
		res.write(html)
		res.end()
	}).listen(3001)
  })

io.on('connection', socket => {
	globalSocket = socket
	socket.on("relay", async data => {
		let turnResult = await relayRequest(data.ip, data.turn)
		console.log('turn result', turnResult)
	})
	console.log("new socket conn", socket)
})
let dictionary = new Map(),
	getDevices = async () => {
		return await findLocalDevices()
	},
	checkRequest = async (ip, responseText) => {
		return new Promise((resolve, reject) => {
			try {
				const req = http.get({hostname: ip}, res => {
					console.log(`statusCode: ${res.statusCode}`)
					res.on("data", function(chunk) {
					    console.log("BODY: " + chunk);
					    if (chunk.indexOf(responseText) >= 0) {
							resolve(true)
					    } else {
					    	reject(false)
					    }
					});
				}).on('error', function(e) {
				  // console.log("Got error: " + e.message);
				});
			} catch (e) {
				// console.log("err", e)
			}
	    	reject(false)
		})
	},
	relayRequest = async (ip, turn) => {
		return new Promise((resolve, reject) => {
			try {
				const req = http.get({hostname: `${ip}/relay?turn=${turn}`}, res => {
					console.log(`statusCode: ${res.statusCode}`)
					res.on("data", function(chunk) {
					    console.log("BODY: " + chunk);
					    if (chunk.indexOf(responseText) >= 0) {
							resolve(true)
					    }
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
		console.log("updated dictionary", dictionary)
		console.log("check for zombies ...", dictionary.size)
		dictionary.forEach(record => {
			console.log(record)
			if (!devices.find(device => device.ip == record.ip)) {
				dictionary.delete(record.ip)
				console.log(`zombie found: ${record.ip}`)
			} else {
				console.log(`device ${record.ip} is available`)
			}
		})
		console.log("zombies cleared")
		devices.forEach(async device => {
			let checkResult = false
			device.pinged = false
			try {
				checkResult = await checkRequest(device.ip, "Shelly")
				if (checkResult) {
					device.pinged = true
				} else {
					device.pinged = false
				}
			} catch (e) {
				device.pinged = false
			}
			console.log(checkResult)
		})
		if (globalSocket) {
			globalSocket.emit("devices", devices)
		}
	}, 1000)
