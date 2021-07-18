const http = require('http')
const path = require('path')
const fs = require('fs')
const url = require('url');
let globalSocket
var Device_GUID = "dDf5FFShellysde";
var Device_Class = "Shelly";

// process.on('uncaughtException', function (err) {
//     // console.log(err);
// }); 

const io = require("socket.io")(3000)//3000


let dictionary = new Map();

http.createServer((req, res) => {
	
	let filePath = '.' + req.url;
	
	  if(req.url === "/state"){
		console.log("state");
		var jsData = JSON.stringify(Array.from(dictionary.values()))
        res.end(jsData);
	  }
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

const Arpping = require('arpping');
const arpping = new Arpping({
	debug: true
});

const getIPRange = require('get-ip-range');

//let dictionary = new Map(),
	getDevices = async () => {
	//	return await findLocalDevices('172.20.10.0/24')
	//	return await findLocalDevices('192.168.0.1/24')
	//	return await findLocalDevices('192.168.1.2/24')
   let {hosts} = await arpping.ping(getIPRange('192.168.0.2', '192.168.0.150'))
	console.log(hosts)
	return hosts;
	}
	
  checkRequest = async (ip, result) => {
		return new Promise((resolve, reject) => {
			try {
				result.value = []
				console.log("input before get ip " + ip)
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
						  var jsonParsed = JSON.parse(body)
						  
					      if (jsonParsed.class == Device_Class) {					
							  result.state = jsonParsed.state							   				
							  result.class = jsonParsed.class
							  result.name = jsonParsed.name
							  result.device_guid = jsonParsed.device_guid	
							  result.index = jsonParsed.index
							  console.log("result.state: " + result.state);
						      console.log("result.class: " + result.class);
						      console.log("result.name: " + result.name);
						      console.log("result.device_guid: " + result.device_guid);
							  console.log("result.index: " + result.index);
						  
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
			
	       dictionary.forEach(device => {
			if (device.ip === ip) {
				device.state = turn
			}
			})
			
			io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
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
	        dictionary.forEach(device => {
			if (device.ip === ip) {
				device.name = name
			}
			})
			
			io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
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

		
		dictionary.forEach(async record => {
			
			if (!devices.find(device => device.ip == record.ip)) {
			  dictionary.delete(record.ip)
			console.log("dictionary.delete(record.ip)  " + record.ip)
			}
		}
		)
		
		  devices.forEach(async device => { //dev 17, 18, 19,    dic 17, 18, 19, 20, 21  
				let checkResult = false
				var DeviceState = []
				try {
					checkResult = await checkRequest(device.ip, DeviceState)
					
					if (checkResult) {
						console.log("device.ip is checkresult " + device.ip);
						device.pinged = true
						device.state = DeviceState.state
						device.name = DeviceState.name
						device.class = DeviceState.class
						device.index = DeviceState.index
						
						
						dictionary.set(device.ip, device)
						   
					} else {
						if (dictionary.has(device.ip)) {
						  dictionary.delete(device.ip)
						}
					}
					
				} catch (e) { 
					console.log(e)
					
					 //device.pinged = false
				}

				if (devices.size == 0) {
				   dictionary.clear()	
				  console.log(" dictionary.clear()	")
				}
			//   io.sockets.emit("devices", Array.from(dictionary.values())) // auto update client push click new data

			//}
		})
		/////////////////////////////////////////
		let tmp = [];
		let tmp_count = 0;
		dictionary.forEach(async record => {
			tmp[tmp_count] = record.index;
			tmp_count++;
		})
		console.log("tmp :")
		console.log(tmp)
	//	tmp.sort();
		const sorted = tmp.sort((a, b) => {  
		  return a - b
		})
		tmp_count = 0;
		let dictionary_buf = new Map();
		while(dictionary.size > 0){
			dictionary.forEach(async record => {
				if(record.index == sorted[tmp_count]){
					dictionary_buf.set(record.ip, record);
					dictionary.delete(record.ip);
					tmp_count++;
					console.log("dictionary: ")
					console.log(dictionary)
					console.log("dictionary_buf :")
					console.log(dictionary_buf)
				}
			})
		}
		dictionary_buf.forEach(async record => {
			dictionary.set(record.ip, record);
		})
		dictionary_buf.clear()		
		////////////////////////////////////////
		 io.sockets.emit("devices", Array.from(dictionary.values())) // auto update client push click new data
		console.log(dictionary)
	}, 10000)// refr page
	
try {
	io.on('connection', socket => {
	globalSocket = socket
	io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
	
	socket.on("relay", async data => {
		let turnResult = await relayRequest(data.ip, data.turn)
		console.log('turn result', turnResult)
//		response(Array.from(dictionary.values()))
//		io.sockets.emit("devices", Array.from(dictionary.values())) // push click new data
	})
	socket.on("ChangeName", async data => {
		let tResult = await relayChangeName(data.ip, data.name)
	//	response(Array.from(dictionary.values()))
		//console.log('setname result', data.name)
	})
	socket.on("deviceReq", (response) => {
		response(Array.from(dictionary.values()))
	})
})
}
catch (e){
	console.log("catch soccet io: ", e)
	return false;
}
