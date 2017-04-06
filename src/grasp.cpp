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
    
    double minUtility = numeric_limits <double> :: max(), maxUtility = numeric_limits <double> :: min();

    while (true) {
        for (ulint i = 0; i < A.size(); i++) {
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
        vector < pair <ulint, double> > restrictedCandidateList;
        for (ulint i = 0; i < A.size(); i++) {
            if (result.first[i] == 0 && (i <= 0 || result.first[i - 1] == 0) && (i >= A.size() - 1 || result.first[i + 1] == 0)) { // if 'i' can be put in solution
                if ((*incrementalUtility)[i] > 0) { // if 'i' can improve solution
                    if ((*incrementalUtility)[i] >= restriction) {
                        restrictedCandidateList.push_back(make_pair(i, (*incrementalUtility)[i]));
                    }
                }
            }
        }
        if (restrictedCandidateList.size() > 0) {
            uniform_int_distribution <ulint> distribution (0, restrictedCandidateList.size() - 1);
            ulint s = distribution(generator);
            ulint k = restrictedCandidateList[s].first;
            ulint deltaUtility = restrictedCandidateList[s].second;
            result.first[k] = 1;
            result.second += deltaUtility;
            // update incremental utility
            (*incrementalUtility)[k] += result.first[k] * A[k][k] * result.first[k];
            for (ulint i = 0; i < A.size(); i++) {
                if (i != k) {
                    (*incrementalUtility)[i] += result.first[i] * A[i][k] * result.first[k];
                }
            }
            for (ulint j = 0; j < A.size(); j++) {
                if (j != k) {
                    (*incrementalUtility)[j] += result.first[k] * A[k][j] * result.first[j];
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

bool oneFlip (matrix A, vector <double> * incrementalUtility, default_random_engine generator, tSolution * solution) {
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
            ulint k = *it;
            tSolution newSolution = make_pair(vector <ulint> ((*solution).first), (*solution).second);
            if (newSolution.first[k] == 0) {
                newSolution.first[k] = 1;
                newSolution.second += (*incrementalUtility)[k];
            } else {
                newSolution.first[k] = 0;
                newSolution.second -= (*incrementalUtility)[k];
            }
            if (isFeasible(newSolution) && newSolution.second >= (*solution).second) {
                result = true;
                flag = true;
                (*solution).first = vector <ulint> (newSolution.first);
                (*solution).second = newSolution.second;
                // atualizando utilidade incremental
                if ((*solution).first[k] == 1) {
                    (*incrementalUtility)[k] += A[k][k];
                    for (ulint i = 0; i < A.size(); i++) {
                        if (i != k) {
                            (*incrementalUtility)[i] += result.first[i] * A[i][k];
                        }
                    }
                    for (ulint j = 0; j < A.size(); j++) {
                        if (j != k) {
                            (*incrementalUtility)[j] += A[k][j] * result.first[j];
                        }
                    }
                } else {
                    (*incrementalUtility)[k] -= A[k][k];
                    for (ulint i = 0; i < A.size(); i++) {
                        if (i != k) {
                            (*incrementalUtility)[i] -= result.first[i] * A[i][k];
                        }
                    }
                    for (ulint j = 0; j < A.size(); j++) {
                        if (j != k) {
                            (*incrementalUtility)[j] -= A[k][j] * result.first[j];
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
                // atualizar utilidade incremental
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
