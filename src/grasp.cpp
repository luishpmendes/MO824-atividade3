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

tSolution greedyRandomizedConstruction (matrix A, vector <double> * incrementalUtility, double alpha, default_random_engine generator) {
    tSolution result = make_pair(vector <uint> (A.size(), 0), 0.0);

    double minUtility = numeric_limits <double> :: max(), maxUtility = numeric_limits <double> :: min();

    while (true) {
        for (uint i = 0; i < A.size(); i++) {
            if (result.first[i] == 0 && (i <= 0 || result.first[i - 1] == 0) && (i >= A.size() - 1 || result.first[i + 1] == 0)) { // if 'i' can be put in solution
                if ((*incrementalUtility)[i] > 0) { // if 'i' can improve solution
                    if (minUtility > (*incrementalUtility)[i]) {
                        minUtility = (*incrementalUtility)[i];
                    }
                    if (maxUtility < (*incrementalUtility)[i]) {
                        maxUtility = (*incrementalUtility)[i];
                    }
                }
            }
        }
        // compute restriction
        double restriction = maxUtility - alpha * (maxUtility - minUtility);
        // populate RCL
        vector < pair <uint, double> > restrictedCandidateList;
        for (uint i = 0; i < A.size(); i++) {
            if (result.first[i] == 0 && (i <= 0 || result.first[i - 1] == 0) && (i >= A.size() - 1 || result.first[i + 1] == 0)) { // if 'i' can be put in solution
                if ((*incrementalUtility)[i] > 0) { // if 'i' can improve solution
                    if ((*incrementalUtility)[i] >= restriction) {
                        restrictedCandidateList.push_back(make_pair(i, (*incrementalUtility)[i]));
                    }
                }
            }
        }
        if (restrictedCandidateList.size() > 0) {
            uniform_int_distribution <uint> distribution (0, restrictedCandidateList.size() - 1);
            uint s = distribution(generator);
            uint i = restrictedCandidateList[s].first;
            uint deltaUtility = restrictedCandidateList[s].second;
            result.first[i] = 1;
            result.second += deltaUtility;
            // update incremental utility
            (*incrementalUtility)[i] += result.first[i] * A[i][i] * result.first[i];
            for (uint j = 0; j < A.size(); j++) {
                if (j != i) {
                    (*incrementalUtility)[j] += result.first[j] * A[j][i] * result.first[i];
                }
            }
        } else {
            // if there is no candidate, break out the loop
            break;
        }
    }
    return result;
}

bool isFeasible(tSolution solution) {
    for (uint i = 0; i < solution.first.size(); i++) {
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

bool oneFlip (matrix A, vector <double> * incrementalUtility, default_random_engine generator, tSolution * solution) {
    bool result = false;
    vector <uint> neighborhood;
    for (uint i = 0; i < A.size(); i++) {
        neighborhood.push_back(i);
    }
    bool flag = true;
    while (flag) {
        flag = false;
        shuffle (neighborhood.begin(), neighborhood.end(), generator);
        for (vector <uint> :: iterator it = neighborhood.begin(); it != neighborhood.end(); it++) {
            uint i = *it;
            tSolution newSolution = make_pair(vector <uint> ((*solution).first), (*solution).second);
            if (newSolution.first[i] == 0) {
                newSolution.first[i] = 1;
                newSolution.second += (*incrementalUtility)[i];
            } else {
                newSolution.first[i] = 0;
                newSolution.second -= (*incrementalUtility)[i];
            }
            if (isFeasible(newSolution) && newSolution.second >= (*solution).second) {
                result = true;
                flag = true;
                (*solution).first = vector <uint> (newSolution.first);
                (*solution).second = newSolution.second;
                // atualizando utilidade incremental
                if ((*solution).first[i] == 1) {
                    (*incrementalUtility)[i] += A[i][i];
                    for (uint j = 0; j < A.size(); j++) {
                        if (j != i) {
                            (*incrementalUtility)[j] += (*solution).first[j] * (A[j][i] + A[i][j]);
                        }
                    }
                } else {
                    (*incrementalUtility)[i] -= A[i][i];
                    for (uint j = 0; j < A.size(); j++) {
                        if (j != i) {
                            (*incrementalUtility)[j] -= (*solution).first[j] * (A[j][i] + A[i][j]);
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool twoFlip (matrix A, vector <double> * incrementalUtility, default_random_engine generator, tSolution * solution) {
    bool result = false;
    vector < vector <uint> > neighborhood;
    for (uint i = 0; i < A.size(); i++) {
        for (uint j = i + 1; j < A.size(); j++) {
            vector <uint> neighbor;
            neighbor.push_back(i);
            neighbor.push_back(j);
            neighborhood.push_back(neighbor);
        }
    }
    bool flag = true;
    while (flag) {
        flag = false;
        shuffle (neighborhood.begin(), neighborhood.end(), generator);
        for (vector < vector <uint> > :: iterator it = neighborhood.begin(); it != neighborhood.end(); it++) {
            vector <uint> neighbor = *it;
            tSolution newSolution = make_pair(vector <uint> ((*solution).first), (*solution).second);
            
            for (vector <uint> :: iterator it2 = neighbor.begin(); it2 != neighbor.end(); it2++) {
                uint k = *it2;

            }
        }
    }
    return result;
}

void localSearch (matrix A, vector <double> * incrementalUtility, default_random_engine generator, tSolution * solution) {

}

bool termination (chrono :: high_resolution_clock :: time_point tBegin, ulint timeLimit) {
    chrono :: high_resolution_clock :: time_point tCurrent = chrono :: high_resolution_clock :: now();
    chrono :: seconds elapsedTime = chrono :: duration_cast <chrono :: seconds> (tCurrent - tBegin);
    if ((ulint) elapsedTime.count() >= timeLimit) {
        return true;
    }
    return false;
}

void repair(matrix A, vector <double> * incrementalUtility, tSolution * solution) {
    // desligar bits até se tornar factível
    // dar preferencia pros bits que violam mais restricões
    // em caso de empate, dar preferencia pros que diminuem menos a utilidade
}

tSolution grasp (matrix A, chrono :: high_resolution_clock :: time_point tBegin, ulint timeLimit, double alpha, ulint seed) {
    tSolution result;

    default_random_engine generator (seed);

    vector <double> incrementalUtility (A.size(), 0.0);
    bool flag = true;
    while (termination (tBegin, timeLimit) != true) {
        tSolution solution = greedyRandomizedConstruction(A, &incrementalUtility, alpha, generator);
        localSearch(A, &incrementalUtility, generator, &solution);

        if (!isFeasible(solution)) {
            repair(A, &incrementalUtility, &solution);
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
    ulint seed = 0;
    ulint timeLimit = 10;
    double alpha = 0.3;
    int searchMethod; // 0 = first-improving; 1 = best-improving

    if (argc >= 2) {
        seed = atoi(argv[1]);
    }

    if (argc >= 3) {
        timeLimit = atoi(argv[2]);
    }

    if (argc >= 4) {
        alpha = atof(argv[3]);
    }

    if (argc >= 5) {
        searchMethod = atoi(argv[4]);
    }

    if (alpha < 0.0) {
        alpha = 0.0;
    } else if (alpha > 1.0) {
        alpha = 1.0;
    }

    if (searchMethod < 0) {
        searchMethod = 0;
    } else if (searchMethod > 1) {
        searchMethod = 1;
    }

    uint n;

    cin >> n;

    matrix A (n, vector <double> (n, 0.0));

    for (uint i = 0; i < n; i++) {
        for (uint j = i; j < n; j++) {
            cin >> A[i][j];
        }
    }

    tSolution solution = grasp(A, tBegin, timeLimit, alpha, seed);

    cout << "maxVal = " << solution.second << endl;

    chrono :: high_resolution_clock :: time_point tEnd = chrono :: high_resolution_clock :: now();
    chrono :: nanoseconds elapsedTime = chrono :: duration_cast <chrono :: nanoseconds> (tEnd - tBegin);

    cout << "Time = " << elapsedTime.count() << " seg" << endl;

    cout << "Solution: " << endl;
    for (uint i = 0; i < n; i++) {
        cout << solution.first[i] << endl;
    }

    return 0;
}
