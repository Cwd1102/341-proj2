// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet() {

}

SatNet::~SatNet() {

}
void SatNet::insert(const Sat& satellite) {

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