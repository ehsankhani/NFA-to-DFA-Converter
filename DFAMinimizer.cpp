#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

class DFA {
public:
    int numStates;
    vector<char> alphabet;
    vector<vector<int>> transition;
    int startState;
    set<int> acceptStates;

    DFA(int n, vector<char> alpha, int start, set<int> accept)
        : numStates(n), alphabet(alpha), startState(start), acceptStates(accept) {
        transition.resize(n, vector<int>(alpha.size(), -1));
    }

    void addTransition(int state, char symbol, int nextState) {
        int symbolIndex = find(alphabet.begin(), alphabet.end(), symbol) - alphabet.begin();
        transition[state][symbolIndex] = nextState;
    }

    void minimize() {
        // Step 1: Initialize partitions based on accept and non-accept states
        vector<set<int>> partitions = {acceptStates};
        set<int> nonAcceptStates;
        for (int i = 0; i < numStates; i++) {
            if (acceptStates.find(i) == acceptStates.end()) {
                nonAcceptStates.insert(i);
            }
        }
        if (!nonAcceptStates.empty()) {
            partitions.push_back(nonAcceptStates);
        }

        // Step 2: Split states until no further splits are possible
        queue<pair<int, char>> workQueue;
        for (char c : alphabet) {
            for (int state : acceptStates) {
                workQueue.push({state, c});
            }
        }

        while (!workQueue.empty()) {
            auto [state, symbol] = workQueue.front();
            workQueue.pop();
            int symbolIndex = find(alphabet.begin(), alphabet.end(), symbol) - alphabet.begin();

            map<set<int>, set<int>> affected;
            for (auto &partition : partitions) {
                set<int> splitter;
                for (int s : partition) {
                    if (transition[s][symbolIndex] == state) {
                        splitter.insert(s);
                    }
                }
                if (!splitter.empty() && splitter.size() < partition.size()) {
                    affected[splitter] = partition;
                }
            }

            for (auto &[splitter, partition] : affected) {
                partitions.erase(find(partitions.begin(), partitions.end(), partition));
                set<int> diff;
                set_difference(partition.begin(), partition.end(), splitter.begin(), splitter.end(), inserter(diff, diff.end()));
                partitions.push_back(splitter);
                partitions.push_back(diff);

                for (char c : alphabet) {
                    for (int s : splitter) {
                        workQueue.push({s, c});
                    }
                }
            }
        }

        // Step 3: Create a mapping from old states to new states
        map<int, int> stateMap;
        int newState = 0;
        for (auto &partition : partitions) {
            for (int s : partition) {
                stateMap[s] = newState;
            }
            newState++;
        }

        // Step 4: Construct the minimized DFA transition table and accept states
        vector<vector<int>> newTransition(newState, vector<int>(alphabet.size(), -1));
        set<int> newAcceptStates;
        int newStartState = stateMap[startState];
        for (int i = 0; i < numStates; i++) {
            for (int j = 0; j < alphabet.size(); j++) {
                if (transition[i][j] != -1) {
                    newTransition[stateMap[i]][j] = stateMap[transition[i][j]];
                }
            }
            if (acceptStates.find(i) != acceptStates.end()) {
                newAcceptStates.insert(stateMap[i]);
            }
        }

        // Step 5: Update the DFA with minimized states and transitions
        numStates = newState;
        transition = newTransition;
        startState = newStartState;
        acceptStates = newAcceptStates;
    }

    void print() {
        cout << "Number of states: " << numStates << endl;
        cout << "Start state: " << startState << endl;
        cout << "Accept states: ";
        for (int s : acceptStates) {
            cout << s << " ";
        }
        cout << endl;
        cout << "Transitions:" << endl;
        for (int i = 0; i < numStates; i++) {
            for (int j = 0; j < alphabet.size(); j++) {
                if (transition[i][j] != -1) {
                    cout << "State " << i << " --" << alphabet[j] << "--> " << transition[i][j] << endl;
                }
            }
        }
    }
};

int main() {
    vector<char> alphabet = {'a', 'b'};
    DFA dfa(6, alphabet, 0, {1, 3, 5});

    dfa.addTransition(0, 'a', 1);
    dfa.addTransition(0, 'b', 2);
    dfa.addTransition(1, 'a', 1);
    dfa.addTransition(1, 'b', 3);
    dfa.addTransition(2, 'a', 4);
    dfa.addTransition(2, 'b', 2);
    dfa.addTransition(3, 'a', 1);
    dfa.addTransition(3, 'b', 3);
    dfa.addTransition(4, 'a', 5);
    dfa.addTransition(4, 'b', 2);
    dfa.addTransition(5, 'a', 5);
    dfa.addTransition(5, 'b', 3);

    cout << "Original DFA:" << endl;
    dfa.print();

    dfa.minimize();

    cout << "Minimized DFA:" << endl;
    dfa.print();

    return 0;
}
