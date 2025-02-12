async function refresh_clock() {
	the_now=new Date();
	the_now=the_now.toString();
	all_now=the_now.split('G');
	document.getElementById("time_display").innerHTML = all_now[0];//.toString();

}


window.addEventListener('load', function () {
	var fetchInterval = 1000; // 1 second.
	setInterval(refresh_clock, fetchInterval);
	});

