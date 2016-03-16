// GLOBAL VARS
var socket;
var canvas;
var ctx;

var canvasData;
var data;
var type;
var stats;
var image;
var URL = window.URL ? window.URL : window.webkitURL;

//----------------------------------------------------------------------------------------------------------------
// ON READY, SETUP SOCKET!
//----------------------------------------------------------------------------------------------------------------

$(document).ready( function() {
	document.getElementById("brow").textContent = " " + BrowserDetect.browser + " "
		+ BrowserDetect.version +" " + BrowserDetect.OS +" ";
        
	stats = new Stats();
	stats.setMode( 0 ); // 0: fps, 1: ms

	// Align top-left
	stats.domElement.style.position = 'absolute';
	stats.domElement.style.right = '0px';
	stats.domElement.style.top = '0px';

  	document.body.appendChild( stats.domElement );
  	setupWebGL();
	setupSocket();
 	stats.begin();
    animate();
});

//----------------------------------------------------------------------------------------------------------------
// WS:ON OPEN
//----------------------------------------------------------------------------------------------------------------
function onOpen(){
	document.getElementById("wslm_statustd").style.backgroundColor = "#40ff40";
	document.getElementById("wslm_statustd").textContent = " websocket connection opened ";
}

//----------------------------------------------------------------------------------------------------------------
// WS: ON CLOSE
//----------------------------------------------------------------------------------------------------------------
function onClose(){
	document.getElementById("wslm_statustd").style.backgroundColor = "#ff4040";
	document.getElementById("wslm_statustd").textContent = " websocket connection CLOSED ";
}

//----------------------------------------------------------------------------------------------------------------
// WS: ON MESSAGE
//----------------------------------------------------------------------------------------------------------------

var tex;

var prevBlob;
function onMessage( messageEvent ){
    stats.end();
    if (messageEvent.data instanceof Blob) {
    	messageEvent.data.type = "image/jpeg";

      	URL.revokeObjectURL(prevBlob);
      	prevBlob = URL.createObjectURL(messageEvent.data);

      	THREE.ImageUtils.crossOrigin = '';

      	var image = new Image();
		image.onload = function () {
			// texture.image = this;
			canvas.width = this.width;
			canvas.height = this.height;

	      	var context = canvas.getContext('2d');
	      	context.drawImage(this, 0, 0);

	      	texture.needsUpdate = true;
		};
		image.crossOrigin = "";
		image.src = prevBlob;
		// image.crossOrigin = "anonymous";

		if ( texture ) texture.needsUpdate = true;
	}
    stats.begin();
}

//----------------------------------------------------------------------------------------------------------------
// setup web socket
//----------------------------------------------------------------------------------------------------------------

function setupSocket(){
	// setup!
	socket = new WebSocket( "ws://127.0.0.1:9093");	
	socket.binaryType = "blob";
	
	// open
	try {
		socket.onopen 		= onOpen;
		socket.onmessage 	= onMessage;
		socket.onclose 	 	= onClose;
	} catch(exception) {
		alert('Error' + exception);  
	}
}


//----------------------------------------------------------------------------------------------------------------
// setupGLCanvas
//----------------------------------------------------------------------------------------------------------------

var camera, scene, renderer, mesh, texture, mat;
var canvas;

function setupWebGL(){
	camera = new THREE.PerspectiveCamera( 50, window.innerWidth / window.innerHeight, 1, 2000 );
	camera.position.z = 1;

	scene = new THREE.Scene();
	canvas = document.createElement('canvas');
	texture = new THREE.Texture(canvas);
	mat = new THREE.MeshBasicMaterial( { map: texture });//, overdraw: 0.5 } );

	mesh = new THREE.Mesh( new THREE.PlaneBufferGeometry( 1, 1 ), 
							mat );
	scene.add(mesh);

	renderer = new THREE.WebGLRenderer( { antialias: true } );
	renderer.setPixelRatio( window.devicePixelRatio );
	renderer.setSize( window.innerWidth, window.innerHeight );
	document.body.appendChild( renderer.domElement );

	// canvas.style.position = 'absolute';
	// canvas.style.top = '0';
	// canvas.style.left = '0';
}

function animate() {
	requestAnimationFrame( animate );

	render();
	stats.update();

}

function render() {

	// camera.position.x += ( mouseX - camera.position.x ) * 0.05;
	// camera.position.y += ( - mouseY - camera.position.y ) * 0.05;
	// camera.lookAt( scene.position );

	renderer.render( scene, camera );

}
