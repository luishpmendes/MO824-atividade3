#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iterator>
#include <set>

using namespace std;

typedef unsigned int uint;
typedef long int lint;
typedef unsigned long int ulint;
typedef vector < vector <double> > matrix;
typedef pair < vector <uint>, double > tSolution;

double evaluateUtility (matrix A, vector <uint> solution) {
    double result = 0.0;
    for (uint i = 0; i < A.size(); i++) {
        for (uint j = 0; j < A[i].size(); j++) {
            result += solution[i] * A[i][j] * solution[j];
        }
    }
    return result;
}

double evaluatecontribuition (matrix A, vector <uint> solution, uint i) {
    double result = A[i][i];
    for (uint j = 0; j < A[i].size(); j++) {
        if (i != j) {
            result += solution[j] * (A[i][j] + A[j][i]);
        }
    }
    return result;
}

tSolution greedyRandomizedConstruction (matrix A, double alpha, default_random_engine generator) {
    tSolution result = make_pair(vector <uint> (A.size(), 0), 0.0);
    bool flag = true;
    while (flag) {
        double minUtility = 0, maxUtility = 0;
        bool flag2 = true;
        vector < pair <uint, double> > candidateList;
        for (uint i = 0; i < A.size(); i++) {
            tSolution solution;
            solution.first = vector <uint> (result.first);
            solution.second = result.second;
            if (solution.first[i] == 0) { // if 'i' is not in solution
                solution.first[i] = 1;
                double contribuition = evaluatecontribuition(A, solution.first, i);
                solution.second += contribuition;
                if (solution.second >= result.second) { // if 'i' can improve solution
                    candidateList.push_back(make_pair(i, contribuition));
                    if (flag2) {
                        flag2 = false;
                        minUtility = contribuition;
                        maxUtility = contribuition;
                    }
                    if (minUtility > contribuition) {
                        minUtility = contribuition;
                    }
                    if (maxUtility < contribuition) {
                        maxUtility = contribuition;
                    }
                }
            }
        }
        // compute restriction
        double restriction = maxUtility - alpha * (maxUtility - minUtility);
        // populate RCL
        vector < pair <uint, double> > restrictedCandidateList;
        for (vector < pair <uint, double> > :: iterator it = candidateList.begin(); it != candidateList.end(); it++) {
            pair <uint, double> candidate = *it;
            if (candidate.second >= restriction) {
                restrictedCandidateList.push_back(candidate);
            }
        }
        if (restrictedCandidateList.size() > 0) {
            uniform_int_distribution <uint> distribution (0, restrictedCandidateList.size() - 1);
            uint s = distribution(generator);
            pair <uint, double> candidate = restrictedCandidateList[s];
            uint i = candidate.first;
            uint deltaUtility = candidate.second;
            result.first[i] = 1;
            result.second += deltaUtility;
        } else {
            // if there is no candidate, break out of the loop
            flag = false;
        }
    }
    return result;
}

bool isFeasible (tSolution solution) {
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

void repair (matrix A, tSolution * solution) {
    // desligar bits até se tornar factível
    // dar preferencia pros bits que violam mais restricões
    // em caso de empate, dar preferencia pros que diminuem menos a utilidade
    vector <uint> restrictionsViolatedCounter (A.size(), 0);
    set <uint> invalidBits;
    for (uint i = 0; i < A.size(); i++) {
        if ((*solution).first[i] == 1) {
            if (i >= 1) {
                if ((*solution).first[i - 1] == 1) {
                    restrictionsViolatedCounter[i]++;
                    invalidBits.insert(i);
                }
            }
            if (i < A.size() - 1) {
                if ((*solution).first[i + 1] == 1) {
                    restrictionsViolatedCounter[i]++;
                    invalidBits.insert(i);
                }
            }
        }
    }
    while (!isFeasible(*solution) && invalidBits.size() > 0) {
        uint chosenBit = *(invalidBits.begin());
        for (set <uint> :: iterator it = invalidBits.begin(); it != invalidBits.end(); it++) {
            uint i = *it;
            if (restrictionsViolatedCounter[chosenBit] < restrictionsViolatedCounter[i]) {
                chosenBit = i;
            } else if (restrictionsViolatedCounter[chosenBit] == restrictionsViolatedCounter[i]) {
                tSolution chosenNewSolution;
                chosenNewSolution.first = vector <uint> ((*solution).first);
                chosenNewSolution.second = (*solution).second;
                double chosenContribution = evaluatecontribuition(A, chosenNewSolution.first, chosenBit);
                chosenNewSolution.first[chosenBit] = 0;
                chosenNewSolution.second -= chosenContribution;
                tSolution newSolution;
                newSolution.first = vector <uint> ((*solution).first);
                newSolution.second = (*solution).second;
                double newContribution = evaluatecontribuition(A, newSolution.first, i);
                newSolution.first[i] = 0;
                newSolution.second -= newContribution;
                if (chosenNewSolution.second < newSolution.second) {
                    chosenBit = i;
                }
            }
        }
        double contribuition = evaluatecontribuition(A, (*solution).first, chosenBit);
        (*solution).first[chosenBit] = 0;
        (*solution).second -= contribuition;
        invalidBits.erase(chosenBit);
    }
}

void localSearch (matrix A, int searchMethod, default_random_engine generator, tSolution * solution) {
    vector < vector <uint> > neighborhood;
    // 1Flip neighborhood
    for (uint i = 0; i < A.size(); i++) {
        vector <uint> neighbor;
        neighbor.push_back(i);
        neighborhood.push_back(neighbor);
    }
    // 2Flip neighborhood
    for (uint i = 0; i < A.size(); i++) {
        for (uint j = i + 1; j < A.size(); j++) {
            vector <uint> neighbor;
            neighbor.push_back(i);
            neighbor.push_back(j);
            neighborhood.push_back(neighbor);
        }
    }
    // 3Flip neighborhood
/*    for (uint i = 0; i < A.size(); i++) {
        for (uint j = i + 1; j < A.size(); j++) {
            for (uint k = j + 1; k < A.size(); k++) {
                vector <uint> neighbor;
                neighbor.push_back(i);
                neighbor.push_back(j);
                neighbor.push_back(k);
                neighborhood.push_back(neighbor);
            }
        }
    }
*/
    shuffle (neighborhood.begin(), neighborhood.end(), generator);
    bool flag = true;
    for (vector < vector <uint> > :: iterator it = neighborhood.begin(); flag && it != neighborhood.end(); it++) {
        vector <uint> neighbor = *it;
        tSolution newSolution = make_pair(vector <uint> ((*solution).first), (*solution).second);
        for (vector <uint> :: iterator it2 = neighbor.begin(); it2 != neighbor.end(); it2++) {
            uint i = *it2;
            double contribuition = evaluatecontribuition(A, newSolution.first, i);
            if (newSolution.first[i] == 0) {
                newSolution.first[i] = 1;
                newSolution.second += contribuition;
            } else {
                newSolution.first[i] = 0;
                newSolution.second -= contribuition;
            }
        }
/*
        if (!isFeasible(newSolution)) {
            repair (A, &newSolution);
        }
*/
        if (isFeasible(newSolution) && newSolution.second > (*solution).second) {
            (*solution).first = vector <uint> (newSolution.first);
            (*solution).second = newSolution.second;
            if (searchMethod == 0) {
                flag = false;
            }
        }
    }
}

bool termination (chrono :: high_resolution_clock :: time_point tBegin, ulint timeLimit) {
    cout << "termination" << endl;
    chrono :: high_resolution_clock :: time_point tCurrent = chrono :: high_resolution_clock :: now();
    chrono :: seconds elapsedTime = chrono :: duration_cast <chrono :: seconds> (tCurrent - tBegin);
    if ((ulint) elapsedTime.count() >= timeLimit) {
        return true;
    }
    return false;
}

double average (vector <double> v) {
    double result = 0.0;
    for (uint i = 0; i < v.size(); i++) {
        result += v[i];
    }
    if (v.size() > 0) {
        result /= (double (v.size()));
    }
    return result;
}

tSolution grasp (matrix A, ulint seed, ulint timeLimit, int searchMethod, vector < pair < double, pair <double, vector <double> > > > psi, chrono :: high_resolution_clock :: time_point tBegin) {
    tSolution result;

    default_random_engine generator (seed);

    bool psiFlag = false;

    bool flag = true;
    cout << "vai entrar loop" << endl;
    while (termination (tBegin, timeLimit) != true) {
        cout << "vai escolher alpha" << endl;
        uniform_real_distribution <double> distribution(0.0, 1.0);
        double r = distribution(generator);
        double alpha = psi[0].first;
        double sum = 0.0;
        uint chosenAlphaIndex = 0;
        for (chosenAlphaIndex = 0; chosenAlphaIndex < psi.size() && sum < r; chosenAlphaIndex++) {
            alpha = psi[chosenAlphaIndex].first;
            sum += psi[chosenAlphaIndex].second.first;
        }
        cout << "alpha: " << alpha << endl;
        cout << "vai entrar greedyRandomizedConstruction" << endl;
        tSolution solution = greedyRandomizedConstruction(A, alpha, generator);
        cout << "saiu greedyRandomizedConstruction" << endl;
        cout << "vai entrar localSearch" << endl;
        localSearch(A, searchMethod, generator, &solution);
        cout << "saiu localSearch" << endl;
        if (!isFeasible(solution)) {
            repair(A, &solution);
        }
        cout << "reparou se necessario" << endl;

        if (flag || result.second < solution.second) {
            flag = false;
            result = solution;
        }

        cout << "vai atualizar psi" << endl;

        psi[chosenAlphaIndex].second.second.push_back(solution.second);

        if (!psiFlag) {
            cout << "adasd" << endl;
            psiFlag = true;
            for (uint i = 0; psiFlag && i < psi.size(); i++) {
                if (psi[i].second.second.size() < 1) {
                    psiFlag = false;
                }
            }
        } else {
            cout << "zxczxcxzc" << endl;
            vector <double> q (psi.size(), 0.0);
            double sumQ = 0.0;
            for (uint i = 0; i < psi.size(); i++) {
                q[i] = result.second / average(psi[i].second.second);
                sumQ += q[i];
            }
            for (uint i = 0; i < psi.size(); i++) {
                psi[i].second.first = q[i] / sumQ;
            }
        }

        cout << "atualizou psi" << endl;
    }
    cout << "vai sair grasp" << endl;
    return result;
}

int main (int argc, char * argv[]) {
    chrono :: high_resolution_clock :: time_point tBegin = chrono :: high_resolution_clock :: now();
    ulint seed = 0;
    ulint timeLimit = 10;
    int searchMethod; // 0 = first-improving; 1 = best-improving
    uint m = 1;

    if (argc >= 2) {
        seed = atoi(argv[1]);
    }

    if (argc >= 3) {
        timeLimit = atoi(argv[2]);
    }

    if (argc >= 4) {
        searchMethod = atoi(argv[3]);
    }

    if (argc >= 5) {
        m = atoi(argv[4]);
    }

    // (alpha_i, p_i, A_i)
    vector < pair < double, pair <double, vector <double> > > > psi;
    for (int i = 0; (uint (i)) < m; i++) {
        if (argc >= 6 + i) {
            double alpha = atof(argv[5 + i]);
            if (alpha < 0.0) {
                alpha = 0.0;
            } else if (alpha > 1.0) {
                alpha = 1.0;
            }
            double p = double (1.0 / (double (m)));
            psi.push_back(make_pair(alpha, make_pair(p, vector <double> ())));
        }
    }

    if (seed == 0) {
        seed = tBegin.time_since_epoch().count();
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

    tSolution solution = grasp(A, seed, timeLimit, searchMethod, psi, tBegin);

    cout << "qwqeqwe" << endl;

    cout << "maxVal = " << solution.second << endl << endl;

    chrono :: high_resolution_clock :: time_point tEnd = chrono :: high_resolution_clock :: now();
    chrono :: seconds elapsedTime = chrono :: duration_cast <chrono :: seconds> (tEnd - tBegin);

    cout << "Time = " << elapsedTime.count() << " seg" << endl << endl;

    cout << "Solution: " << endl;
    for (uint i = 0; i < n; i++) {
        cout << solution.first[i] << endl;
    }
    cout << endl;

    cout << "Psi: " << endl;
    cout << "alpha - p - A" << endl;
    for (uint i = 0; i < psi.size(); i++) {
        cout << psi[i].first << " - " << psi[i].second.first << " - " << average(psi[i].second.second) << endl;;
    }
    cout << endl;

    return 0;
}
