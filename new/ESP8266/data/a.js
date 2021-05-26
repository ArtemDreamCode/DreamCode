function get_param(param) {
	var req = getXmlHttp()  
	var statusElem = document.getElementById(param);		
	req.onreadystatechange = function() { 
		if (req.readyState == 4) { 
			if(req.status == 200) { 
				if(param == 'boot_state'){
					document.getElementsByName('boot_state')[Number(req.responseText)].checked = 'true';
				} else if ((param == 'use_mqtt')||(param == 'use_http')) {
					statusElem.checked = (req.responseText == "true");
				} else {
					statusElem.value = req.responseText;				
				}
				statusElem.disabled = false
			}
		}	
	}
	statusElem.disabled = true
	req.open('GET', 'get?param=' + param, true);  
	req.send(null);
}

function get_info(param) {
	var req = getXmlHttp()  
	var statusElem = document.getElementById(param);		
	req.onreadystatechange = function() { 
		if (req.readyState == 4) { 
			if(req.status == 200) { 
				statusElem.innerHTML = req.responseText;				
				statusElem.disabled = false
			}
		}	
	}
	statusElem.disabled = true
	req.open('GET', 'info?param=' + param, true);  
	req.send(null);
}

function set_param(param, value) {
	var req = getXmlHttp()  
	var statusElem = document.getElementById(param);
	req.onreadystatechange = function() { 
		if (req.readyState == 4) { 
			if(req.status == 200) { 
				if(param == 'boot_state'){
					document.getElementsByName('boot_state')[Number(req.responseText)].checked = 'true';
				} else if ((param == 'use_mqtt')||(param == 'use_http')) {
					statusElem.checked = (req.responseText == "true");
				} else {
					statusElem.value = req.responseText;				
				}
				statusElem.disabled = false
			}
		}	
	}	
	statusElem.disabled = true
	req.open('GET', 'set?param=' + param + '&value=' + value, true);  
	req.send(null);
	
}

function openTab(evt, tabName) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";
}

function getXmlHttp(){
  var xmlhttp;
  try {
    xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
  } catch (e) {
    try {
      xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    } catch (E) {
      xmlhttp = false;
    }
  }
  if (!xmlhttp && typeof XMLHttpRequest!='undefined') {
    xmlhttp = new XMLHttpRequest();
  }
  return xmlhttp;
}

function get_state() {
	var req = getXmlHttp()  
	var statusElem = document.getElementById('req_status');		
	req.onreadystatechange = function() { 
	if (req.readyState == 4) { 
		if(req.status == 200) { 
			statusElem.innerHTML = req.responseText;
			}
		}	
	}
	req.open('GET', 'state', true);  
	req.send(null);
}

function reboot(){
	if(!confirm('Are you sure you want to restart the device?'))
		return;
	var req = getXmlHttp()  
	req.open('GET', 'restart', true);  
	req.send(null);
}

function factoryReset(){
	if(!confirm('Are you sure you want to reset the device to factory settings?'))
		return;
	location.reload();
}

function LoadState(){
	openTab(event, 'states');
    get_state();
	get_info("dev_id");
	get_info("local_ip");
	get_info("softap_ip");
	get_info("mac");
	setInterval(get_state, 5000);
}

function LoadSettings(){
	openTab(event, 'settings');
	get_param('boot_state');
	get_param('use_mqtt');
	get_param('mqtt_brocker');
	get_param('mqtt_port');
	get_param('mqtt_user');
	get_param('mqtt_pass');
	get_param('use_http');
	get_param('http_on_request');
	get_param('http_off_request');
}

function LoadWiFiSettings(){
	openTab(event, 'wifi');
	get_param('wifi_ssid');
	get_param('wifi_pass');
	get_info("wifi_networks");
}

function SaveWiFiSettings(){
	set_param('wifi_ssid',document.getElementById('wifi_ssid').value);
	set_param('wifi_pass',document.getElementById('wifi_pass').value);	
}

function SaveDevID(){
	set_param('dev_id',document.getElementById('dev_id1').value);	
}

function SaveBootState(){
	set_param('boot_state',document.querySelector('input[name="boot_state"]:checked').value);
}

function SaveMQTT(){
	set_param('use_mqtt',document.getElementById('use_mqtt').checked);
	set_param('mqtt_brocker',document.getElementById('mqtt_brocker').value);
	set_param('mqtt_port',document.getElementById('mqtt_port').value);
	set_param('mqtt_user',document.getElementById('mqtt_user').value);
	set_param('mqtt_pass',document.getElementById('mqtt_pass').value);	
}

function SaveHTTP(){
	set_param('use_http',document.getElementById('use_http').checked);
	set_param('http_on_request',document.getElementById('http_on_request').value);	
	set_param('http_off_request',document.getElementById('http_off_request').value);	
}