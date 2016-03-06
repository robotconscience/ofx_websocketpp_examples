#pragma once

#include "ofMain.h"


#include "ofxAsio.h"

// websocketpp stuff
#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/client.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class Client : protected ofThread {
public:
    
    Client(): bConnected(false){
        
    }
    
    ~Client(){
        close();
    }
    
    void connect( string uri ){
        // this is from the default example!
        
        try {
            // Set logging to be pretty verbose (everything except message payloads)
            c.set_access_channels(websocketpp::log::alevel::all);
            c.clear_access_channels(websocketpp::log::alevel::frame_payload);
            
            // Initialize ASIO
            c.init_asio();
            
            // Register our message handler
            c.set_message_handler(bind(&on_message,&c,::_1,::_2));
            
            websocketpp::lib::error_code ec;
            con = c.get_connection(uri, ec);
            if (ec) {
                std::cout << "could not create connection because: " << ec.message() << std::endl;
                return;
            }
            
            // Note that connect here only requests a connection. No network messages are
            // exchanged until the event loop starts running in the next line.
            c.connect(con);
            
            bConnected = true;
            
            // all good, start thread!
            startThread();
        } catch (websocketpp::exception const & e) {
            std::cout << e.what() << std::endl;
            bConnected = false;
        }
    }
    
    void close(){
        if (!bConnected) return;
        stopThread();
        c.stop();
        bConnected = true;
    }
    
    void send( string msg ){
        if ( !bConnected ) return;
        c.send(con->get_handle(), msg, websocketpp::frame::opcode::text);
    }
    
protected:
    
    client c;
    
    bool bConnected;
    client::connection_ptr con;
    
    // This message handler will be invoked once for each incoming message. It
    // prints the message and then sends a copy of the message back to the server.
    static void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;
    }
    
    void threadedFunction(){
        while (isThreadRunning()){
            if ( !bConnected ) return;
            
            // Start the ASIO io_service run loop
            // this will cause a single connection to be made to the server. c.run()
            // will exit when this connection is closed.
            c.run();
        }
    }
};