async function refresh_clock() {
	the_now=new Date();
	the_now=the_now.toString();
	all_now=the_now.split('G');
	document.getElementById("time_display").innerHTML = all_now[0];//.toString();

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

async function DoScan()
{
	console.log("Starting scan");
	let scandata=await sendCommand("scanreport");

	console.log("Ended scan");
	preid=document.getElementById("scan_result");
	preid.innerHTML=scandata;
	console.log("Displayed scan");
}

window.addEventListener('load', function () {
	var fetchInterval = 1000; // 1 second.
	setInterval(refresh_clock, fetchInterval);
	});

