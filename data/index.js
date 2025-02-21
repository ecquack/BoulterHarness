async function refresh_clock() {
	the_now=new Date();
	the_now=the_now.toString();
	all_now=the_now.split('G');
	document.getElementById("time_display").innerHTML = all_now[0];//.toString();

	let scandata=await sendCommand("refresh");
	ShowScan(scandata);

}

		
async function sendCommand(full_url)
{
	try {
		let res = await fetch(full_url);
		return await res.text();
	} catch (error) {
		console.log(error);
	}

}


function ShowScan(scandata) {
	console.log("Ended scan");
	preid=document.getElementById("scan_result");
	preid.innerHTML=scandata;
	console.log("Displayed scan");
	
	passfail=document.getElementById("passfail");
	if(scandata[0]=="G") {
		passfail.innerHTML="FAIL";
		passfail.setAttribute("style","color:red;");
	}
	else if(scandata[0]=="\r") 
		{
		passfail.innerHTML="PASS";
		passfail.setAttribute("style","color:green;");
	}
	else passfail.innterHTML=""; // no result yet- fresh reboot
	// weird case. what's the first character of a pass? carriage return...
}

async function DoScan()
{
	console.log("Starting scan");
	let scandata=await sendCommand("scanreport");
	ShowScan(scandata);
	
}

window.addEventListener('load', function () {
	var fetchInterval = 1000; // 1 second.
	setInterval(refresh_clock, fetchInterval);
	});

