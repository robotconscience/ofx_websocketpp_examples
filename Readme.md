#OF + websocketpp examples noodles

##What is this?
* I like websockets. I support an [OF addon](https://github.com/labatrockwell/ofxLibwebsockets) that wraps [libwebsockets](http://github.com/warmcat/libwebsockets). In the past, I needed a more powerful, multi-threadable websocket server, and turned to [websocketpp](https://github.com/zaphoyd/websocketpp). From there came [ofxWebsocketpp](https://github.com/labatrockwell/ofxWebsocketpp) written by my colleague Lucas Vickers.
* If you try to use that addon (ofxWebsocketpp), you will probably fail. It requires a lot of non-OF-y things like Boost, installing stuff at your root, etc etc. It was hard to use websocketpp then.
* Now it's easier, and you can use websocketpp with Asio instead. There is the amazing [ofxAsio](https://github.com/elliotwoods/ofxAsio) that makes it easy to use in OF.
* This repo is me slowly experimenting with newer websocketpp in OF. Maybe it will become an addon someday!

##How do I make it work?
* Download openFrameworks
* Clone [ofxAsio](https://github.com/elliotwoods/ofxAsio) into of/addons
* Clone this repo into addons or apps
* That's it!

##These don't really do anything / aren't written well / etc
* Yes, I know. Just poking around a bit, OK?

##How do I make a new project? 
* OS X
	* Open the project generator
	* Generate a new project with ofxAsio as one addon
	* Add the websocketpp folder to your 'header search paths' (e.g. if I'm making a project in this folder, I add '../libs' and '../libs/websocketpp')
	* 