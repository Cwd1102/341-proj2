// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet() {
    m_root = nullptr;
}

SatNet::~SatNet() {

}

void SatNet::insert(const Sat& satellite) {
    if (m_root == nullptr) {
        m_root = new Sat(satellite);
    }
    else {
        //sets the height of the node
        m_root->m_height = returnHeight(satellite, *m_root, m_root->getHeight());
    }
}

void SatNet::clear() {

}

void SatNet::remove(int id) {

}

void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const {
    if (satellite != nullptr) {
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellites() const {

}

bool SatNet::setState(int id, STATE state) {

}

void SatNet::removeDeorbited() {

}

bool SatNet::findSatellite(int id) const {

}

const SatNet& SatNet::operator=(const SatNet& rhs) {

}

int SatNet::countSatellites(INCLIN degree) const {

}

//helper function
//recursive function to return the height of the tree
//inserts nodes into the tree as well
int SatNet::returnHeight(const Sat& satellite,Sat& current, int height) {
    //every time the return happens, the current height is set then incremented and returned
    if (satellite.getID() < current.getID()) {
	   if(current.m_left == nullptr){
		   current.m_left = new Sat(satellite);
		   return height + 1;
	   }
	   else{
		   returnHeight(satellite, *current.m_left, height + 1);
	   }
   }
   else if(satellite.getID() > current.getID()){
	   if(current.m_right == nullptr){
		   current.m_right = new Sat(satellite);
		   return height + 1;
	   }
	   else{
		   returnHeight(satellite, *current.m_right, height + 1);
	   }
   }
   else{
	   return height;


}