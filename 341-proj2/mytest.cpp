#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
const string PASS = "(PASSED)";
const string FAIL = "------------FAILED------------";

enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE };
class Random {
public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL) {
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int>& array) {
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i <= m_max; i++) {
            array.push_back(i);
        }
        shuffle(array.begin(), array.end(), m_generator);
    }

    void getShuffle(int array[]) {
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i <= m_max; i++) {
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it = temp.begin(); it != temp.end(); it++) {
            array[i] = *it;
            i++;
        }
    }

    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT) {
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester {
public:
    bool insertNormal();
    bool insertEdge();
    bool interstError();
    bool removeNormal();
    bool removeEdge();
    bool removeError();
    bool checkInsertBal();
    bool checkRemoveBal();
private:
};

int main() {
    Tester myTester;

    cout << "Testing insertNormal" << endl;
    if (myTester.insertNormal()) {
        		cout << "insertNormal" << PASS << endl;
    }
    else {
				cout << "insertNormal" << FAIL << endl;
	}

    cout << "Testing insertEdge" << endl;
    if (myTester.insertEdge()) {
				cout << "insertEdge" << PASS << endl;
	}
    else {
				cout << "insertEdge" << FAIL << endl;
	}

    cout << "Testing interstError" << endl;
    if (myTester.interstError()) {
				cout << "interstError" << PASS << endl;
	}
    else {
				cout << "interstError" << FAIL << endl;
	}

    cout << "Testing removeNormal" << endl;
    if (myTester.removeNormal()) {
				cout << "removeNormal" << PASS << endl;
	}
    else {
				cout << "removeNormal" << FAIL << endl;
	}

    cout << "Testing removeEdge" << endl;
    if (myTester.removeEdge()) {
				cout << "removeEdge" << PASS << endl;
	}
    else {
				cout << "removeEdge" << FAIL << endl;
	}

	cout << "Testing removeError" << endl;
    if (myTester.removeError()) {
				cout << "removeError" << PASS << endl;
	}
    else {
				cout << "removeError" << FAIL << endl;
	}
    cout << "Testing checkInsertBal" << endl;
    if (myTester.checkInsertBal()) {
				cout << "checkInsertBal" << PASS << endl;
	}
    else {
				cout << "checkInsertBal" << FAIL << endl;
	}

	cout << "Testing checkRemoveBal" << endl;
    if (myTester.checkRemoveBal()) {
				cout << "checkRemoveBal" << PASS << endl;
	}
    else {
				cout << "checkRemoveBal" << FAIL << endl;
	}

	return 0;

}

bool Tester::insertNormal() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);  // there are 4 inclination
    Random altGen(0, 3);     // there are 4 altitudes
    SatNet network;

    int teamSize = 100;
    int tempArray[100]{};
    int ID = 0;
    for (int i = 0; i < teamSize; i++) {
        ID = idGen.getRandNum();
        tempArray[i] = ID;
        Sat satellite(ID,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }
    for (int i = 0; i < teamSize; i++) {
        if (network.findSatellite(tempArray[i]) == false) {
			return false;
		}
	}

    return true;
}

bool Tester::insertEdge() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);  // there are 4 inclination
    Random altGen(0, 3);     // there are 4 altitudes
    SatNet network;

    int teamSize = 100;
    int tempArray[100]{};
    int ID = 0;
    for (int i = 0; i < teamSize; i++) {
        ID = idGen.getRandNum();
        tempArray[i] = ID;
        Sat satellite(ID,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }
    if (network.findSatellite(tempArray[99]) == false) {
        return false;
    }

	return true;
}

bool Tester::interstError() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);  // there are 4 inclination
    Random altGen(0, 3);     // there are 4 altitudes
    SatNet network;
    Sat tempSat(-20);

    int teamSize = 100;
    int tempArray[100]{};
    int ID = 0;
    for (int i = 0; i < teamSize; i++) {
        ID = idGen.getRandNum();
        tempArray[i] = ID;
        Sat satellite(ID,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }
    network.insert(tempSat);
    if (network.findSatellite(-20) == true) {
        return false;
    }

	return true;
}

bool Tester::removeNormal() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;

	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
    for (int i = 0; i < teamSize; i++) {
		network.remove(tempArray[i]);
	}
    for (int i = 0; i < teamSize; i++) {
        if (network.findSatellite(tempArray[i]) == true) {
			return false;
		}
	}

	return true;
}

bool Tester::removeEdge() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;

	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
    for (int i = 0; i < teamSize; i++) {
		network.remove(tempArray[99]);
	}
    if (network.findSatellite(tempArray[99]) == true) {
		return false;
	}

	return true;
}

bool Tester::removeError() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
    int bal = 0;

	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
    bal = network.checkBal(network.m_root);
	network.remove(10);
    if (network.checkBal(network.m_root) != bal) {
		return false;
	}

	return true;
}

bool Tester::checkInsertBal() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int bal = 0;

	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
        bal = network.checkBal(network.m_root);
        if (bal > 1 || bal < -1) {
            return false;
        }
	}

	return true;
}

bool Tester::checkRemoveBal() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int bal = 0;

	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
    for (int i = 0; i < teamSize; i++) {
		network.remove(tempArray[i]);
		bal = network.checkBal(network.m_root);
        if (bal > 1 || bal < -1) {
			return false;
		}
	}

	return true;
}
