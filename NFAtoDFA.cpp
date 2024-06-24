#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

using namespace std;

// Define structures for NFA and DFA
typedef set<int> State;
typedef map<char, State> TransitionFunction;
typedef map<State, TransitionFunction> NFA;
typedef map<pair<State, char>, State> DFATransitionFunction;

// Function to perform epsilon closure (subset of NFA)
State epsilonClosure(const NFA& nfa, const State& states) {
    State closure = states;
    queue<int> q;

    for (int state : states) {
        q.push(state);
    }

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        // Ensure '\0' transitions exist for the current state
        if (nfa.find({current}) != nfa.end() && nfa.at({current}).find('\0') != nfa.at({current}).end()) {
            for (int next : nfa.at({current}).at('\0')) {
                if (closure.find(next) == closure.end()) {
                    closure.insert(next);
                    q.push(next);
                }
            }
        }
    }

    return closure;
}

// Function to convert NFA to DFA using subset construction
void convertNFAtoDFA(const NFA& nfa, NFA& dfa, DFATransitionFunction& dfaTransition, State& initialState) {
    queue<State> q;
    map<State, bool> visited;

    // Calculate epsilon closure of initial state
    State initialClosure = epsilonClosure(nfa, initialState);
    dfa[initialClosure] = {};

    q.push(initialClosure);
    visited[initialClosure] = true;

    while (!q.empty()) {
        State current = q.front();
        q.pop();

        // Iterate over each character in the alphabet
        for (char symbol = 'a'; symbol <= 'z'; ++symbol) {
            State next;

            // Calculate states reachable from current state with the symbol
            for (int state : current) {
                if (nfa.find({state}) != nfa.end() && nfa.at({state}).find(symbol) != nfa.at({state}).end()) {
                    for (int nextState : nfa.at({state}).at(symbol)) {
                        next.insert(nextState);
                    }
                }
            }

            // Calculate epsilon closure of the set of states 'next'
            State closure = epsilonClosure(nfa, next);

            // If closure is not empty, add to DFA states
            if (!closure.empty()) {
                dfaTransition[{current, symbol}] = closure;
                if (!visited[closure]) {
                    visited[closure] = true;
                    dfa[closure] = {};
                    q.push(closure);
                }
            }
        }
    }
}

// Function to print the NFA
void printNFA(const NFA& nfa) {
    cout << "NFA Transitions:" << endl;
    for (auto stateTransitions : nfa) {
        cout << "State {";
        for (int state : stateTransitions.first) {
            cout << state << " ";
        }
        cout << "} -> ";
        for (auto transition : stateTransitions.second) {
            cout << "{" << transition.first << " -> {";
            for (int nextState : transition.second) {
                cout << nextState << " ";
            }
            cout << "}";
        }
        cout << endl;
    }
}

int main() {
    // Example NFA definition
    // NFA nfa = {
    //     {{0}, {{'\0', {1}}}},
    //     {{1}, {{'a', {2}}, {'b', {3}}}},
    //     {{2}, {{'b', {4}}}},
    //     {{3}, {{'a', {4}}}}
    // };

    // State initialState = {0};
    // State finalState = {4};

    // second example
    // Example NFA definition
    NFA nfa = {
    {{0}, {{'a', {0}}, {'b', {0}}, {'\0', {1}}}},
    {{1}, {{'b', {2}}}},
    {{2}, {{'a', {2}}, {'b', {2}}, {'b', {4}}}},
    {{3}, {{'b', {4}}}},
    {{4}, {{'a', {5}}, {'b', {5}}}},
    {{5}, {{'a', {5}}, {'b', {5}}}}
    };

    State initialState = {0};
    State finalState = {5};
    printNFA(nfa);

    // Convert NFA to DFA
    NFA dfa;
    DFATransitionFunction dfaTransition;
    convertNFAtoDFA(nfa, dfa, dfaTransition, initialState);

    // Output DFA transitions
    cout << "\nDFA Transitions:" << endl;
    for (auto transition : dfaTransition) {
        cout << "{";
        for (int state : transition.first.first) {
            cout << state << " ";
        }
        cout << ", " << transition.first.second << "} -> {";
        for (int state : transition.second) {
            cout << state << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}
