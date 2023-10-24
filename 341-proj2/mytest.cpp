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
    bool insertError();
    bool removeNormal();
    bool removeEdge();
    bool removeError();
    bool checkInsertBal();
    bool checkRemoveBal();
    bool checkDeorbited();
    bool checkDeorbitedEdge();
    bool checkDeorbitError();
    bool testsHeight();
    bool testCountSat();
    bool testAssignment();
    bool testInsertComplexity();
    bool testRemoveComplexity();
    bool testEmptyObject();
    bool testBST();
    bool testBST2();

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

    cout << "Testing insertError" << endl;
    if (myTester.insertError()) {
				cout << "insertError" << PASS << endl;
	}
    else {
				cout << "insertError" << FAIL << endl;
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

    cout << "Testing checkDeorbited" << endl;
    if (myTester.checkDeorbited()) {
				cout << "checkDeorbited" << PASS << endl;
	}
    else {
				cout << "checkDeorbited" << FAIL << endl;
	}

    cout << "Testing checkDeorbitedEdge" << endl;
    if (myTester.checkDeorbitedEdge()) {
				cout << "checkDeorbitedEdge" << PASS << endl;
	}
    else {
				cout << "checkDeorbitedEdge" << FAIL << endl;
	}

    cout << "Testing checkDeorbitError" << endl;
    if (myTester.checkDeorbitError()) {
				cout << "checkDeorbitError" << PASS << endl;
	}
    else {
				cout << "checkDeorbitError" << FAIL << endl;
	}

    cout << "Testing testsHeight" << endl;
    if (myTester.testsHeight()) {
				cout << "testsHeight" << PASS << endl;
	}
    else {
				cout << "testsHeight" << FAIL << endl;
	}

	cout << "Testing testCountSat" << endl;
    if (myTester.testCountSat()) {
				cout << "testCountSat" << PASS << endl;
	}
    else {
				cout << "testCountSat" << FAIL << endl;
	}

    cout << "Testing testAssignment" << endl;
    if (myTester.testAssignment()) {
				cout << "testAssignment" << PASS << endl;
	}
    else {
				cout << "testAssignment" << FAIL << endl;
	}

    cout << "Testing testInsertComplexity" << endl;
    if (myTester.testInsertComplexity()) {
				cout << "testInsertComplexity" << PASS << endl;
	}
    else {
				cout << "testInsertComplexity" << FAIL << endl;
	}

    cout << "Testing testRemoveComplexity" << endl;
    if (myTester.testRemoveComplexity()) {
				cout << "testRemoveComplexity" << PASS << endl;
	}
    else {
				cout << "testRemoveComplexity" << FAIL << endl;
	}

    cout << "Testing testEmptyObject" << endl;
    if (myTester.testEmptyObject()) {
				cout << "testEmptyObject" << PASS << endl;
	}
    else {
				cout << "testEmptyObject" << FAIL << endl;
	}

    cout << "Testing testBST" << endl;
    if (myTester.testBST()) {
				cout << "testBST" << PASS << endl;
	}
    else {
				cout << "testBST" << FAIL << endl;
	}

    cout << "Testing testBST2" << endl;
    if (myTester.testBST2()) {
				cout << "testBST2" << PASS << endl;
	}
    else {
				cout << "testBST2" << FAIL << endl;
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

bool Tester::insertError() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);  // there are 4 inclination
    Random altGen(0, 3);     // there are 4 altitudes
    SatNet network;
    Sat tempSat(-20);

    int teamSize = 100;
    int tempArray[100]{0};
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
        Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
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

bool Tester::checkDeorbited() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int bal = 0;
    int j = 0;
	int teamSize = 10000;
	int tempArray[10000]{};
    int idList[30]{};
	int ID = 0;
    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}

    for (int i = 0; i < 30; i++) {
        network.setState(tempArray[i], DEORBITED);
        idList[j] = tempArray[i];
        j++;
    }

    network.removeDeorbited();
    for (int i = 0; i < 30; i++) {
        if (network.findSatellite(idList[i]) == true) {
			return false;
		}
	}
	return true;
}

bool Tester::checkDeorbitedEdge() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int bal = 0;
    int j = 0;
	int teamSize = 100;
	int tempArray[100]{};
    int idList[30]{};
	int ID = 0;

    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
        tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}

    for (int i = 30; i < 50; i++) {
        network.setState(tempArray[i], DEORBITED);
        idList[j] = tempArray[i];
        j++;
    }
    network.removeDeorbited();
    for (int i = 0; i < 30; i++) {
		if (network.findSatellite(idList[i]) == true) {
            return false;
        }
    }
	return true;
}

bool Tester::checkDeorbitError() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int bal = 0;
	int j = 0;
	int teamSize = 100;
	int tempArray[100]{};
	int idList[30]{};
	int ID = 0;

    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}

    for (int i = 30; i < 50; i++) {
		network.setState(tempArray[i], DEORBITED);
		idList[j] = tempArray[i];
		j++;
	}

	network.removeDeorbited();
    for (int i = 0; i < 30; i++) {
        if (network.findSatellite(idList[i]) == true) {
			return false;
		}
	}
	return true;
}

bool Tester::testsHeight() {
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
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
        if ((network.checkBal(network.m_root) < -1) or (network.checkBal(network.m_root) > 1)) {
            return false;
        }
	}
	return true;
}

bool Tester::testCountSat() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
	int count = 0;
	int inclin = 0;

    for (int i = 0; i < teamSize; i++) {
        ID = idGen.getRandNum();
        tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
        if (satellite.m_inclin == 1) {
			inclin++;
		}
	}
	count = network.countSatellites(static_cast<INCLIN>(1));
    if (inclin != count) {
		return false;
	}
	return true;
}

bool Tester::testAssignment() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	SatNet network1;
	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
	int count = 0;
	int inclin = 0;

    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
	network1 = network;
    for (int i = 0; i < teamSize; i++) {
        if (network.findSatellite(tempArray[i]) != network1.findSatellite(tempArray[i])) {
			return false;
		}
	}
	return true;
}


bool Tester::testInsertComplexity() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int teamSize1 = 10000;
    int teamSize2 = 20000;
	int tempArray[10000]{};
    int tempArray2[20000]{};
	int ID = 0;
    double start = 0;
    double stop = 0;
    double T1 = 0;
    double T2 = 0;
    double realRatio = 0;
    int constant = teamSize2 / teamSize1;
    double expectedRatio = ((teamSize1 * constant) * log(teamSize1 * constant)) / (teamSize1 *log(teamSize1));
    
    start = clock();
    for (int i = 0; i < teamSize1; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);


	}
    stop = clock();
    T1 = (stop - start) / CLOCKS_PER_SEC;
    start = 0;
    stop = 0;
    start = clock();

    for (int i = 0; i < teamSize2; i++) {
        ID = idGen.getRandNum();
        tempArray2[i] = ID;
        Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);


    }
    stop = clock();

    T2 = (stop - start) / CLOCKS_PER_SEC;

    realRatio = T2 / T1;
    if (((realRatio) < expectedRatio - .4) or (realRatio > expectedRatio + .4)) {
        return false;
    }

	return true;
}

bool Tester::testRemoveComplexity() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int teamSize1 = 10000;
	int teamSize2 = 20000;
	int tempArray[10000]{};
	int tempArray2[20000]{};
	int ID = 0;
	double start = 0;
	double stop = 0;
	double T1 = 0;
	double T2 = 0;
	double realRatio = 0;
	int constant = teamSize2 / teamSize1;
	double expectedRatio = ((teamSize1 * constant) * log(teamSize1 * constant)) / (teamSize1 * log(teamSize1));

    for (int i = 0; i < teamSize1; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
    start = clock();
    for (int i = 0; i < teamSize1; i++) {
		network.remove(tempArray[i]);
	}
    stop = clock();
	T1 = (stop - start) / CLOCKS_PER_SEC;

	start = 0;
	stop = 0;
    
    for (int i = 0; i < teamSize2; i++) {
		ID = idGen.getRandNum();
		tempArray2[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
	start = clock();
    for (int i = 0; i < teamSize2; i++) {
		network.remove(tempArray2[i]);
	}
	stop = clock();
	T2 = (stop - start) / CLOCKS_PER_SEC;

	realRatio = T2 / T1 ;

    if (((realRatio) < expectedRatio - .4) or (realRatio > expectedRatio + .4)) {
		return false;
	}

	return true;
}

bool Tester::testEmptyObject() {
    SatNet network;
    Sat satellite(0);
    network.insert(satellite);
    if (network.m_root != nullptr) {
		return false;
	}
	return true;
}

bool Tester::testBST() {
	Random idGen(MINID, MAXID);
	Random inclinGen(0, 3);  // there are 4 inclination
	Random altGen(0, 3);     // there are 4 altitudes
	SatNet network;
	int teamSize = 100;
	int tempArray[100]{};
	int ID = 0;
	int inclin = 0;

    for (int i = 0; i < teamSize; i++) {
		ID = idGen.getRandNum();
		tempArray[i] = ID;
		Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
		network.insert(satellite);
	}
	network.testBST(network.m_root);
	return true;
}

bool Tester::testBST2() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);  // there are 4 inclination
    Random altGen(0, 3);     // there are 4 altitudes
    SatNet network;
    int teamSize = 100;
    int tempArray[100]{};
    int ID = 0;
    int count = 0;

    for (int i = 0; i < teamSize; i++) {
        ID = idGen.getRandNum();
        tempArray[i] = ID;
        Sat satellite(ID, static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }
    for (int i = 0; i < teamSize; i++) {
		network.remove(tempArray[i]);
	}
	network.testBST(network.m_root);
	return true;
}