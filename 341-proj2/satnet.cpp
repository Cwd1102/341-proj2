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

//returns the height of the tree
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
                while ((prevNode->getRight() != nullptr) and (prevNode->getRight() != currNode)) {
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

//counts the number of satellites with the given inclination
int SatNet::countSatellites(INCLIN degree, Sat* satNode) const {
    //if the node is null
    if (satNode == nullptr) {
        return 0;
    }
    //if the node's inclination is greater than the given inclination
    else if (satNode->getInclin() > degree) {
		return countSatellites(degree, satNode->m_left) + countSatellites(degree, satNode->m_right);
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
    //if the node is null
    if (aNode == nullptr) {
        return false;
    }
    //if the node's id is equal to the given id
    else if (aNode->getID() == id) {
        return true;
    }
    //if the node's id is greater than the given id
    else if (aNode->getID() > id) {
        return findSatellite(id, aNode->m_left);
    }
    //if the node's id is less than the given id
    else {
        return findSatellite(id, aNode->m_right);
    }
}

void SatNet::clear(Sat* satNode) {
    //This works on any subtree
    if (satNode == nullptr) {
        return;
    }
    else {
        //clear the left and right subtrees
        clear(satNode->m_left);
        clear(satNode->m_right);
        delete satNode;
    }
}

Sat* SatNet::find(Sat* aNode, const int& ID) {
    //if the node is null or the node's id is the ID
    if ((aNode == nullptr) or (aNode->getID() == ID)) {
        return aNode;
    }
    //if the node's id is greater than the ID
    else if (aNode->getID() > ID) {
        return find(aNode->m_left, ID);
    }
    //if the node's id is less than the ID
    else {
        return find(aNode->m_right, ID);
    }
}

//inserts the given satellite into the tree
Sat* SatNet::insert(const Sat& ID, Sat*& satNode) {
    //if the node is null
    if (satNode == nullptr) {
        //create a new node
        satNode = new Sat(ID);
        return satNode;
    }
    //if the node's id is greater than the ID
    else if (satNode->getID() > ID.getID()) {
        //insert the ID into the left subtree
        Sat* leftNode = insert(ID, satNode->m_left);
        satNode->m_left = leftNode;
        updateHeight(satNode);
        return reBal(satNode);
    }
    //if the node's id is less than the ID
    else if (satNode->getID() < ID.getID()) {
        //insert the ID into the right subtree
        Sat* rightNode = insert(ID, satNode->m_right);
        satNode->m_right = rightNode;
        updateHeight(satNode);
        return reBal(satNode);
    }
    else {
        return satNode;
    }
}

//rebalances the tree
Sat* SatNet::reBal(Sat* satNode) {
    //if the balance is less than -1 and the balance of the right child is greater than or equal to 0
    if ((checkBal(satNode) < -1) and (checkBal(satNode->m_right) <= 0)) {
        return leftRot(satNode);
    }
    //if the balance is greater than 1 and the balance of the left child is greater than or equal to 0
    else if ((checkBal(satNode) < -1) and (checkBal(satNode->m_right) >= 0)) {
        satNode->m_right = rightRot(satNode->m_right);
        return leftRot(satNode);
    }
    //if the balance is less than -1 and the balance of the right child is less than or equal to 0
    else if ((checkBal(satNode) > 1) and (checkBal(satNode->m_left) >= 0)) {
        return rightRot(satNode);
    }
    //if the balance is greater than 1 and the balance of the left child is less than or equal to 0
    else if ((checkBal(satNode) > 1) and (checkBal(satNode->m_left) <= 0)) {
        satNode->m_left = leftRot(satNode->m_left);
        return rightRot(satNode);
    }
    else {
        return satNode;
    }
}

//rotates the tree to the left
Sat* SatNet::leftRot(Sat* satNode) {
    Sat* curr = satNode;
    Sat* next = curr->m_right;
    curr->m_right = next->m_left;
    next->m_left = curr;
    //update the height of the nodes
    updateHeight(curr);
    updateHeight(next);
    return next;
}

//rotates the tree to the right
Sat* SatNet::rightRot(Sat* satNode) {
    Sat* curr = satNode;
    Sat* next = curr->m_left;
    curr->m_left = next->m_right;
    next->m_right = curr;
    //update the height of the nodes
    updateHeight(curr);
    updateHeight(next);
    return next;
}

//checks the balance of the tree
int SatNet::checkBal(Sat* satNode) {
    if (satNode == nullptr)
        return -1;
    else {
        //the balance is the height of the left subtree minus the height of the right subtree
        //find the height of the left and right subtrees
        int leftHeight = -1;
        int rightHeight = -1;
        //if the left and right subtrees are not null
        if (satNode->m_left != nullptr)
            leftHeight = satNode->m_left->m_height;
        if (satNode->m_right != nullptr)
            rightHeight = satNode->m_right->m_height;
        return (leftHeight - rightHeight);
    }
}

//removes the given satellite from the tree
Sat* SatNet::remove(Sat* satNode, const int& ID) {
    //if the node is null
    if (satNode == nullptr) {
        return nullptr;
    }
    //if the node's id is greater than the ID
    if (ID < satNode->getID()) {
        satNode->m_left = remove(satNode->m_left, ID);
    }
    //if the node's id is less than the ID
    else if (ID > satNode->getID()) {
        satNode->m_right = remove(satNode->m_right, ID);
    }
    //if the node's id is equal to the ID
    else {
        //if the node has no children
        if (satNode->m_left == nullptr) {
            Sat* temp = satNode->m_right;
            delete satNode;
            return temp;
        }
        //if the node has one child
        else if (satNode->m_right == nullptr) {
            Sat* temp = satNode->m_left;
            delete satNode;
            return temp;
        }
        //if the node has two children
        Sat* temp = findMin(satNode->m_right);
        satNode->m_id = temp->getID();
        satNode->m_right = remove(satNode->m_right, satNode->getID());
    }
    //update the height of the node
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

//updates the height of the node
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

//testBST
void SatNet::testBST(Sat* satNode) const {
    if (satNode != nullptr) {
		testBST(satNode->m_left);
		testBST(satNode->m_right);
	}
}
