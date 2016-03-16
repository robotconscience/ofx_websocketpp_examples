#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    wsServer.setup(9093);
    video.setup(640,480);
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    if ( video.isFrameNew()){
        //send
        static ofBuffer buff;
        unsigned long size;
        unsigned char * data = turbo.compress(video, 100, &size);
        buff.clear();
        buff.set(reinterpret_cast<char*>(data), size);
        wsServer.sendBinary(buff);
        delete data;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    video.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
