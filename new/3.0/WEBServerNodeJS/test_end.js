const arp = require("arping");
 
arp.ping("192.168.0.1", (err, info) => {
    if (err) throw err; // Timeout, ...
    // THA = target hardware address
    // TIP = target IP address
    console.log("%s (%s) responded in %s secs", info.tha, info.tip, info.elapsed);
});