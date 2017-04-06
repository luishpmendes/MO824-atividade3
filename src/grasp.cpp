#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>
#include <iterator>

using namespace std;

typedef unsigned int uint;
typedef long int lint;
typedef unsigned long int ulint;
typedef vector < vector <double> > matrix;
typedef pair < vector <uint>, double > tSolution;

tSolution greedyRandomizedConstruction (matrix A, vector <double> incrementalUtility, double alpha, default_random_engine generator) {
    tSolution result = make_pair(vector <uint> (A.size(), 0), 0.0);

    while (true) {
        double minUtility = numeric_limits <double> :: max(), maxUtility = numeric_limits <double> :: min();
        vector < pair <ulint, double> > candidateList;
        for (ulint i = 0; i < A.size(); i++) {
            if (result.first[i] == 0 && (i <= 0 || result.first[i - 1] == 0) && (i >= A.size() - 1 || result.first[i + 1] == 0)) { // if canditade can be put in solution
                if (incrementalUtility[i] > 0) { // if this candidate can improve solution
                    candidateList.push_back(make_pair(i, incrementalUtility[i]));
                    if (minUtility > incrementalUtility[i]) {
                        minUtility = incrementalUtility[i];
                    }
                    if (maxUtility < incrementalUtility[i]) {
                        maxUtility = incrementalUtility[i];
                    }
                }
            }
        }
        double restriction = maxUtility - alpha * (maxUtility - minUtility);
        vector < pair <ulint, double> > restrictedCandidateList;
        for (ulint i = 0; i < candidateList.size(); i++) {
            if (candidateList[i].second >= restriction) {
                restrictedCandidateList.push_back(candidateList[i]);
            }
        }
        if (restrictedCandidateList.size() > 0) {
            uniform_int_distribution <ulint> distribution (0, restrictedCandidateList.size() - 1);
            ulint s = distribution(generator);
            result.first[restrictedCandidateList[s].first] = 1;
            result.second += restrictedCandidateList[s].second;
        } else {
            break;
        }
    }

    return result;
}

bool isFeasible(tSolution solution) {
    for (ulint i = 0; i < solution.first.size(); i++) {
        if (solution.first[i] == 1) {
            if (i > 0 && solution.first[i - 1] == 1) {
                return false;
            }
            if (i < solution.first.size() - 1 && solution.first[i + 1] == 1) {
                return false;
            }
        }
    }
    return true;
}

bool oneFlip (matrix A, vector <double> incrementalUtility, default_random_engine generator, tSolution * solution) {
    bool result = false;
    vector <ulint> neighborhood;
    for (ulint i = 0; i < A.size(); i++) {
        neighborhood.push_back(i);
    }
    bool flag = true;
    while (flag) {
        flag = false;
        shuffle (neighborhood.begin(), neighborhood.end(), generator);
        for (vector <ulint> :: iterator it = neighborhood.begin(); it != neighborhood.end(); it++) {
            ulint i = *it;
            tSolution newSolution = make_pair(vector <ulint> ((*solution).first), (*solution).second);
            if (newSolution.first[i] == 0) {
                newSolution.first[i] = 1;
                newSolution.second += incrementalUtility[i];
            } else {
                newSolution.first[i] = 0;
                newSolution.second -= incrementalUtility[i];
            }
            if (isFeasible(newSolution) && newSolution.second >= (*solution).second) {
                result = true;
                flag = true;
                (*solution).first = vector <ulint> (newSolution.first);
                (*solution).second = newSolution.second;
            }
        }
    }
    return result;
}

bool twoFlip (matrix A, vector <double> incrementalUtility, default_random_engine generator, tSolution * solution) {
    bool result = false;
    vector < vector <ulint> > neighborhood;
    for (ulint i = 0; i < A.size(); i++) {
        for (ulint j = i + 1; j < A.size(); j++) {
            vector <ulint> neighbor;
            neighbor.push_back(i);
            neighbor.push_back(j);
            neighborhood.push_back(neighbor);
        }
    }
    bool flag = true;
    while (flag) {
        flag = false;
        shuffle (neighborhood.begin(), neighborhood.end(), generator);
        for (vector <ulint> :: iterator it = neighborhood.begin(); it != neighborhood.end(); it++) {
            vector <ulint> neighbor = *it;
            tSolution newSolution = make_pair(vector <ulint> ((*solution).first), (*solution).second);
            if (newSolution.first[i] == 0) {
                newSolution.first[i] = 1;
                newSolution.second += incrementalUtility[i];
            } else {
                newSolution.first[i] = 0;
                newSolution.second -= incrementalUtility[i];
            }
            if (isFeasible(newSolution) && newSolution.second >= (*solution).second) {
                result = true;
                flag = true;
                (*solution).first = vector <ulint> (newSolution.first);
                (*solution).second = newSolution.second;
            }
        }
    }
    return result;
}

void localSearch (matrix A, vector <double> incrementalUtility, default_random_engine generator, tSolution * solution) {

}

bool termination (chrono :: high_resolution_clock :: time_point tBegin, ulint timeLimit) {
    chrono :: high_resolution_clock :: time_point tCurrent = chrono :: high_resolution_clock :: now();
    chrono :: seconds elapsedTime = chrono :: duration_cast <chrono :: seconds> (tCurrent - tBegin);
    if ((ulint) elapsedTime.count() >= timeLimit) {
        return true;
    }
    return false;
}



void repair(matrix A, tSolution * solution) {

}

tSolution grasp (matrix A, chrono :: high_resolution_clock :: time_point tBegin, ulint timeLimit, double alpha, ulint seed) {
    tSolution result;

    default_random_engine generator (seed);

    vector <double> incrementalUtility (A.size(), 0.0);
    for (ulint i = 0; i < A.size(); i++) {
        for (ulint j = 0; j < A[i].size(); j++) {
            incrementalUtility[i] += A[i][j];
            incrementalUtility[j] += A[i][j];
        }
    }
    for (ulint i = 0; i < A.size(); i++) {
        incrementalUtility[i] -= A[i][i];
    }

    bool flag = true;
    while (termination (tBegin, timeLimit) != true) {
        tSolution solution = greedyRandomizedConstruction(A, incrementalUtility, alpha, generator);
        localSearch(A, incrementalUtility, generator, &solution);

        if (!isFeasible(solution)) {
            repair(A, &solution);
        }

        if (flag || result.second > solution.second) {
            flag = false;
            result = solution;
        }
    }
    return result;
}

int main (int argc, char * argv[]) {
    chrono :: high_resolution_clock :: time_point tBegin = chrono :: high_resolution_clock :: now();
    string I ("0");
    ulint seed = 0;
    ulint timeLimit = 10;
    double alpha = 0.3;

    if (argc >= 2) {
        seed = atoi(argv[1]);
    }

    if (argc >= 3) {
        timeLimit = atoi(argv[2]);
    }

    if (argc >= 4) {
        alpha = atof(argv[3]);
    }

    if (alpha < 0.0) {
        alpha = 0.0;
    } else if (alpha > 1.0) {
        alpha = 1.0;
    }

    ulint n;

    cin >> n;

    matrix A (n, vector <double> (n, 0.0));

    for (ulint i = 0; i < n; i++) {
        for (ulint j = i; j < n; j++) {
            cin >> A[i][j];
        }
    }

    tSolution solution = grasp(A, tBegin, timeLimit, alpha, seed);

    cout << solution.second << endl;

    for (ulint i = 0; i < n; i++) {
        cout << solution.first[i] << endl;
    }

    return 0;
}
