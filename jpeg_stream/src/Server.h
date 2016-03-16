#pragma once


#include "ofMain.h"
#include "ofxAsio.h"
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"


typedef websocketpp::server<websocketpp::config::asio> ws_server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::connection_hdl;

// pull out the type of messages sent by our config
typedef ws_server::message_ptr message_ptr;

class Server : protected ofThread {
public:
    
    Server() : bServerSetup(false){
        websocketServer.set_open_handler(bind(&Server::on_open,this,::_1));
        websocketServer.set_message_handler(bind(&Server::on_message,this,::_1,::_2));
        websocketServer.set_close_handler(bind(&Server::on_close,this,::_1));
    }
    
    ~Server(){
        close();
    }
    
    void setup( uint16_t port){
        try {
            // Set logging settings
            websocketServer.set_access_channels(websocketpp::log::alevel::all);
            websocketServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
            
            // Initialize Asio
            websocketServer.init_asio();
            
            // Register our message handler
            
            // Listen on port
            websocketServer.listen(port);
            
            // Start the server accept loop
            websocketServer.start_accept();
            
            bServerSetup= true;
            cout << "Accepting connections on "<<port<<endl;
            cout << "Running"<<endl;
            startThread();
        } catch (websocketpp::exception const & e) {
            std::cout << e.what() << std::endl;
            bServerSetup= false;
        } catch (...) {
            std::cout << "other exception" << std::endl;
            bServerSetup= false;
        }
    }
    
    void close(){
        if (!bServerSetup) return;
        bServerSetup = false;
        websocketServer.stop();
        websocketServer.stop_listening();
        stopThread();
    }
    
    // as noted here, this is not necessarily the best for scale!
    // http://www.zaphoyd.com/websocketpp/manual/common-patterns/server-initiated-messages
    
    void send( string msg ){
        if (!bServerSetup) return;
        
        for (auto it : websocketConnections) {
            try {
                websocketServer.send(it,msg,websocketpp::frame::opcode::text);
                
            } catch (...) {
                //todo
            }
        }
    }
    
    void sendBinary( const ofBuffer & buf ){
        sendBinary( buf.getData(), buf.size() );
    }
    
    template<class T>
    void sendBinary( T * data, size_t len ){
        if (!bServerSetup) return;
        for (auto it : websocketConnections) {
            websocketServer.send(it,data, len, websocketpp::frame::opcode::binary);
        }
    }
    
protected:
    
    bool bServerSetup;
    
    // Create a server endpoint
    ws_server websocketServer;
    
    // current connetions
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;
    con_list websocketConnections;
    
    // main thread
    void threadedFunction(){
        cout << "Running"<<endl;
        while (isThreadRunning()) {
            if (!bServerSetup) return;
            // Start the ASIO io_service run loop
            websocketServer.run();
        }
    }
    
    void on_open(connection_hdl hdl) {
        cout << "new connection" << endl;
        websocketConnections.insert(hdl);
    }
    
    void on_close(connection_hdl hdl) {
        cout << "connection closed" << endl;
        websocketConnections.erase(hdl);
    }
    
    // Define a callback to handle incoming messages
    void on_message(connection_hdl hdl, ws_server::message_ptr msg) {
        std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;
        
        // todo: notify event here
    }
};
