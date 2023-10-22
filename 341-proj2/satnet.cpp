// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet() {
    m_root = nullptr;
}

SatNet::~SatNet() {
    clear();
}

void SatNet::insert(const Sat& satellite) {
    m_root = insert(satellite, m_root);
}

void SatNet::clear() {
    clear(m_root);
	m_root = nullptr;
}

void SatNet::remove(int id) {
    remove(m_root, id);
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
    Sat* currentNode = m_root;
    while (currentNode != nullptr) {
        if (currentNode->getLeft() == nullptr) {
            cout << currentNode->getID() << ": " << currentNode->getStateStr() << ": " << currentNode->getInclinStr() << ": " << currentNode->getAltStr() << endl;
            currentNode = currentNode->getRight();
        }
        else {
            Sat* prevNode = currentNode->getLeft();
            while (prevNode->getRight() != nullptr && prevNode->getRight() != currentNode) {
                prevNode = prevNode->getRight();
            }

            if (prevNode->getRight() == nullptr) {
                prevNode->setRight(currentNode);
                currentNode = currentNode->getLeft();
            }
            else {
                prevNode->setRight(nullptr);

                cout << currentNode->getID() << ": " << currentNode->getStateStr() << ": " << currentNode->getInclinStr() << ": " << currentNode->getAltStr() << endl;

                currentNode = currentNode->getRight();
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
    Sat* currentNode = m_root;
    while (currentNode != nullptr) {
        if (currentNode->getLeft() == nullptr) {
            if (currentNode->getState() == DEORBITED) {
				remove(currentNode->getID());
			}
			currentNode = currentNode->getRight();
		}
        else {
			Sat* prevNode = currentNode->getLeft();
            while (prevNode->getRight() != nullptr && prevNode->getRight() != currentNode) {
				prevNode = prevNode->getRight();
			}

            if (prevNode->getRight() == nullptr) {
				prevNode->setRight(currentNode);
				currentNode = currentNode->getLeft();
			}
            else {
				prevNode->setRight(nullptr);

                if (currentNode->getState() == DEORBITED) {
					remove(currentNode->getID());
				}

				currentNode = currentNode->getRight();
			}
		}
	}
}

bool SatNet::findSatellite(int id) const {
    return findSatellite(id, m_root);
}

const SatNet& SatNet::operator=(const SatNet& rhs) {
    SatNet temp(rhs);
    return temp;
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

bool SatNet::findSatellite(int id, Sat* aNode) const {
	if (aNode == nullptr)
		return false;
	else if (aNode->getID() == id)
		return true;
	else if (aNode->getID() > id)
		return findSatellite(id, aNode->m_left);
	else
		return findSatellite(id, aNode->m_right);
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
    if (aNode == nullptr || aNode->getID() == element)
        return aNode;
    else if (aNode->getID() > element)
        return find(aNode->m_left, element);
    else  
        return find(aNode->m_right, element);
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
    if ((checkBal(satNode) < -1) and (checkBal(satNode->m_right) <= 0))
        return leftRot(satNode);
    else if ((checkBal(satNode) > 1) and (checkBal(satNode->m_left) >= 0))
        return rightRot(satNode);
    else if ((checkBal(satNode) < -1) and (checkBal(satNode->m_right) >= 0)) {
        satNode->m_right = rightRot(satNode->m_right);
        return leftRot(satNode);
    }
    else if ((checkBal(satNode) > 1) and (checkBal(satNode->m_left) <= 0)) {
        satNode->m_left = leftRot(satNode->m_left);
        return rightRot(satNode);
    }
    else
        return satNode;
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
    if (satNode == nullptr)// item not found; do nothing
        return nullptr;
    // continue to traverse until we find the element
    else if (element < satNode->getID())//traverse to left
        satNode->m_left = remove(satNode->m_left, element);
    else if (element > satNode->getID())//traverse to right
        satNode->m_right = remove(satNode->m_right, element);
    // the case of the node with two children
    else if (satNode->m_left && satNode->m_right) {
        // find right’s lowest value
        temp = findMin(satNode->m_right);
        // copy its value to the node that we want to delete
        satNode->m_id = temp->getID();
        // now delete the found node
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
    if (satNode == nullptr || satNode->m_right == nullptr)
        return satNode;
    else {
        //returns the right child of the node
        return findMax(satNode->m_right);
    }
}

//finds the minimum value in the tree
Sat* SatNet::findMin(Sat* satNode) {
    //if satnode is null or satnode's left child is null
	if (satNode == nullptr || satNode->m_left == nullptr)
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

