// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet() {
    m_root = nullptr;
}

SatNet::~SatNet() {
    if (m_root != nullptr) {
        clear();
	}
}

void SatNet::insert(const Sat& satellite) {
    if (satellite.getID() < MINID or satellite.getID() > MAXID) {
		return;
	}
    m_root = insert(satellite, m_root);
}

void SatNet::clear() {
    if (m_root != nullptr) {
        clear(m_root);
        m_root = nullptr;
    }
}

void SatNet::remove(int id) {
    m_root = remove(m_root, id);
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
    Sat* currNode = m_root;
    while (currNode != nullptr) {
        if (currNode->getLeft() == nullptr) {
            cout << currNode->getID() << ": " << currNode->getStateStr() << ": " << currNode->getInclinStr() << ": " << currNode->getAltStr() << endl;
            currNode = currNode->getRight();
        }
        else {
            Sat* prevNode = currNode->getLeft();
            while (prevNode->getRight() != nullptr and prevNode->getRight() != currNode) {
                prevNode = prevNode->getRight();
            }

            if (prevNode->getRight() == nullptr) {
                prevNode->setRight(currNode);
                currNode = currNode->getLeft();
            }
            else {
                prevNode->setRight(nullptr);

                cout << currNode->getID() << ": " << currNode->getStateStr() << ": " << currNode->getInclinStr() << ": " << currNode->getAltStr() << endl;

                currNode = currNode->getRight();
            }
        }
    }
}

bool SatNet::setState(int id, STATE state) {
    Sat* satNode = find(m_root, id);

    if (satNode != nullptr) {
        satNode->setState(state);
        return true;
    }
    else {
    	return false;
    }
}

void SatNet::removeDeorbited() {
    int height = m_root->getHeight();
    for (int i = 0; i < height; i++) {
        removeDeorbited(m_root);
    }
}

bool SatNet::findSatellite(int id) const {
    return findSatellite(id, m_root);
}

//copy constructor
//copies the given SatNet into a new SatNet
const SatNet& SatNet::operator=(const SatNet& rhs) {
    if (this != &rhs) {
		clear();
		Sat* currNode = rhs.m_root;
        while (currNode != nullptr) {
            if (currNode->getLeft() == nullptr) {
				Sat satellite(currNode->getID(), currNode->getAlt(), currNode->getInclin());
				insert(satellite);
				currNode = currNode->getRight();
			}
            else {
				Sat* prevNode = currNode->getLeft();
                //find the rightmost node in the left subtree
                while (prevNode->getRight() != nullptr and prevNode->getRight() != currNode) {
					prevNode = prevNode->getRight();
				}
                //if the rightmost node is null, set it to the current node
                if (prevNode->getRight() == nullptr) {
					prevNode->setRight(currNode);
					currNode = currNode->getLeft();
				}
                //if the rightmost node is the current node, set it to null
                //and insert the current node into the new tree
                else {
					prevNode->setRight(nullptr);
                    //insert the current node into the new tree
					Sat satellite(currNode->getID(), currNode->getAlt(), currNode->getInclin());
					insert(satellite);

					currNode = currNode->getRight();
				}
			}
		}
	}   
	return *this;
}

//countrs the number of satellites with the given inclination
//recursive function
//adds one for each node with the given inclination
int SatNet::countSatellites(INCLIN degree) const {
    return countSatellites(degree, m_root);
}


// ***************************************************
// Helper functions
// ***************************************************

//removes all deorbited satellites from the tree
//recursive function

void SatNet::removeDeorbited(Sat* satNode) {
    if (satNode == nullptr) {
		return;
	}
    else {
		removeDeorbited(satNode->m_left);
		removeDeorbited(satNode->m_right);
        if (satNode->getState() == DEORBITED) {
			remove(satNode->getID());
		}
	}
}

int SatNet::countSatellites(INCLIN degree, Sat* satNode) const {
    if (satNode == nullptr) {
        return 0;
    }
    else if (satNode->getInclin() == degree) {
        return 1 + countSatellites(degree, satNode->m_left) + countSatellites(degree, satNode->m_right);
    }
    else {
        return countSatellites(degree, satNode->m_left) + countSatellites(degree, satNode->m_right);
    }
}

//finds the satellite with the given id
bool SatNet::findSatellite(int id, Sat* aNode) const {
    if (aNode == nullptr) {
        return false;
    }
    else if (aNode->getID() == id) {
        return true;
    }
    else if (aNode->getID() > id) {
        return findSatellite(id, aNode->m_left);
    }
    else {
        return findSatellite(id, aNode->m_right);
    }
}

void SatNet::clear(Sat* satNode) {
    //This works on any subtree
    if (satNode == nullptr) {//there is no pointer to delete
        return;
    }
    else {
        clear(satNode->m_left);//first delete the left child
        clear(satNode->m_right);//then delete the right child
        delete satNode;//then delete the node itself
    }
}

Sat* SatNet::find(Sat* aNode, const int& element) {
    if (aNode == nullptr or aNode->getID() == element) {
        return aNode;
    }
    else if (aNode->getID() > element) {
        return find(aNode->m_left, element);
    }
    else {
        return find(aNode->m_right, element);
    }
}

Sat* SatNet::insert(const Sat& element, Sat*& satNode) {
    if (satNode == nullptr) {
        Sat* satNode = new Sat(element);
        return satNode;
    }
    else if (satNode->getID() > element.getID()) {
        satNode->m_left = insert(element, satNode->m_left);
        updateHeight(satNode);
        return reBal(satNode);
    }
    else if (satNode->getID() < element.getID()) {
        satNode->m_right = insert(element, satNode->m_right);
        updateHeight(satNode);
        return reBal(satNode);
    }
    else
        return satNode;
}

Sat* SatNet::reBal(Sat* satNode) {
    if ((checkBal(satNode) < -1) and (checkBal(satNode->m_right) <= 0)) {
        return leftRot(satNode);
    }
    else if ((checkBal(satNode) > 1) and (checkBal(satNode->m_left) >= 0)) {
        return rightRot(satNode);
    }
    else if ((checkBal(satNode) < -1) and (checkBal(satNode->m_right) >= 0)) {
        satNode->m_right = rightRot(satNode->m_right);
        return leftRot(satNode);
    }
    else if ((checkBal(satNode) > 1) and (checkBal(satNode->m_left) <= 0)) {
        satNode->m_left = leftRot(satNode->m_left);
        return rightRot(satNode);
    }
    else {
        return satNode;
    }
}

Sat* SatNet::leftRot(Sat* satNode) {
    Sat* curr = satNode;
    Sat* next = curr->m_right;
    curr->m_right = next->m_left;
    next->m_left = curr;
    updateHeight(curr);
    updateHeight(next);
    return next;
}

Sat* SatNet::rightRot(Sat* satNode) {
    Sat* curr = satNode;
    Sat* next = curr->m_left;
    curr->m_left = next->m_right;
    next->m_right = curr;
    updateHeight(curr);
    updateHeight(next);
    return next;
}

int SatNet::checkBal(Sat* satNode) {
    if (satNode == nullptr)
        return -1;
    else {
        int leftHeight = -1;
        int rightHeight = -1;
        if (satNode->m_left != nullptr)
            leftHeight = satNode->m_left->m_height;
        if (satNode->m_right != nullptr)
            rightHeight = satNode->m_right->m_height;
        return (leftHeight - rightHeight);
    }
}

Sat* SatNet::remove(Sat* satNode, const int& element) {
    Sat* temp;
    if (satNode == nullptr) {
        return nullptr;
    }
    else if (element < satNode->getID()) {
        satNode->m_left = remove(satNode->m_left, element);
    }
    else if (element > satNode->getID()) {
        satNode->m_right = remove(satNode->m_right, element);
    }
    else if (satNode->m_left and satNode->m_right) {
        temp = findMin(satNode->m_right);
        satNode->m_id = temp->getID();
        satNode->m_right = remove(satNode->m_right, satNode->m_id);
    }
    else {
        temp = satNode;
        if (satNode->m_left == nullptr)
            satNode = satNode->m_right;
        else if (satNode->m_right == nullptr)
            satNode = satNode->m_left;
        delete temp;
    }
    updateHeight(satNode);
    satNode = reBal(satNode);
    return satNode;
}

//finds the maximum value in the tree
Sat* SatNet::findMax(Sat* satNode) {
    //if satnode is null or satnode's right child is null
    if (satNode == nullptr or satNode->m_right == nullptr)
        return satNode;
    else {
        //returns the right child of the node
        return findMax(satNode->m_right);
    }
}

//finds the minimum value in the tree
Sat* SatNet::findMin(Sat* satNode) {
    //if satnode is null or satnode's left child is null
	if (satNode == nullptr or satNode->m_left == nullptr)
		return satNode;
    else {
        //returns the left child of the node
		return findMin(satNode->m_left);
	}
}

void SatNet::updateHeight(Sat* satNode) {
    int leftHeight = -1;
    int rightHeight = -1;

    if (satNode == nullptr) {
        return;
    }
    if (satNode->m_left != nullptr) {
        leftHeight = satNode->m_left->m_height;
    }
    if (satNode->m_right != nullptr) {
        rightHeight = satNode->m_right->m_height;
    }
    satNode->m_height = max(leftHeight, rightHeight) + 1;
}
