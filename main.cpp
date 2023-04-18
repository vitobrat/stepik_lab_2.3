#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
const string fileString = "C:\\FirstCursProgramm\\C++\\stepik_lab_2.3\\text.txt";
const vector<string> alphabet = {"(", ")", "+", "-", "*", "/", " "};

struct Stack{
    string data = "";
    Stack *next = nullptr;
};

void push(Stack *&head, string data){
    Stack *newNode = new Stack;
    newNode->data = data;
    if(head == nullptr){
        head = newNode;
        newNode->next = nullptr;
    }else{
        if(head->data == ""){
            head->data = data;
            return;
        }
        newNode->next = head;
        head = newNode;
    }
}

string pop(Stack *&head){
    if(head == nullptr){
        cout << "Stack is empty";
        return "";
    }
    string data = head->data;
    Stack *help = head;
    head = head->next;
    delete help;
    return data;
}

string top(Stack *&head){
    if(head == nullptr){
        cout << "Stack is empty";
        return "";
    }
    return head->data;
}

bool isEmpty(Stack *&head){
    return head == nullptr;
}

void printStack(Stack *head){
    while(!isEmpty(head)){
        cout << head->data << " ";
        head = head->next;
    }
}

void deleteStack(Stack *&head){
    while(!isEmpty(head)){
        Stack *help = head;
        head = head->next;
        delete help;
    }
}

int priority(string str){
    switch (str[0]) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '(':
        case ')':
            return 0;
        default:
            return -1;
    }
}

double action(double value1, double value2, string operation) {
    switch (operation[0]) {
        case '+':
            return value1 + value2;
        case '-':
            return value1 - value2;
        case '*':
            return value1 * value2;
        case '/':
            return value1 / value2;
        default:
            break;
    }
}

vector<string> splitString(string str){
    vector<string> tokens;
    string str1 = "";
    for(char i : str){
        string help(1, i);
        if(find(alphabet.begin(), alphabet.end(),help) != alphabet.end()){
            if (str1 != "" && str1 != " ") tokens.push_back(str1);
            str1 = i;
            if (str1 != "" && str1 != " ") tokens.push_back(str1);
            str1 = "";
        }else if(i >= '0' && i <= '9'){
            str1 += i;
        }
    }
    if (str1 != "" && str1 != " ") tokens.push_back(str1);
    return tokens;
}

string fromExpressionToReversePolishNotation(Stack *&stack, string str) {
    string res = "";
    vector<string> tokens = splitString(str);
    if (tokens.size() == 0){
        return "Wrong input!";
    }
    for (int i = 0; i < tokens.size(); i++) {
        if (find(alphabet.begin(), alphabet.end(), tokens[i]) != alphabet.end()) {
            if (tokens[i] == "(") {
                push(stack, tokens[i]);
            } else if (tokens[i] == ")") {
                while (top(stack) != "(") {
                    res += pop(stack);
                    res += ' ';
                }
                pop(stack);
            } else {
                if (!(isEmpty(stack) || priority(top(stack)) < priority(tokens[i]))) {
                    while (!isEmpty(stack) && priority(top(stack)) >= priority(tokens[i])) {
                        res += pop(stack);
                        res += ' ';
                    }
                }
                push(stack, tokens[i]);
            }
        }else{
            res += tokens[i];
            res += ' ';
        }
    }
    while(!isEmpty(stack)){
        res += pop(stack);
        res += ' ';
    }
    return res;
}

string solveExpression(Stack *& stack, string str){
    vector<string> tokens = splitString(str);
    for(int i = 0; i < tokens.size(); i++){
        if (find(alphabet.begin(), alphabet.end(), tokens[i]) != alphabet.end()) {
            push(stack, to_string(action(atof(pop(stack).c_str()), atof(pop(stack).c_str()), tokens[i])));

        }else{
            push(stack, tokens[i]);
        }
    }
    return pop(stack);
}

int main() {
    Stack *stack = new Stack;
    string inputString, resultString, answerString;
    try {
        ifstream file(fileString);
        if (file.is_open()) {
            getline(file, inputString);
        }
        inputString.erase(remove_if(inputString.begin(), inputString.end(), ::isspace), inputString.end());
        resultString = fromExpressionToReversePolishNotation(stack,  inputString);
        deleteStack(stack);
        answerString = solveExpression(stack, resultString);
        cout << resultString << "\n" << answerString;
        file.close();
    }catch (int i){
        cout << "Some exception!";
    }
}