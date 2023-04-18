#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
const string fileString = "C:\\FirstCursProgramm\\C++\\stepik_lab_2.3\\text.txt";
const vector<char> alphabet = {'(', ')', '+', '-', '*', '/'};

struct Stack{
    char data = 0;
    Stack *next = nullptr;
};

void push(Stack *&head, char data){
    Stack *newNode = new Stack;
    newNode->data = data;
    if(head == nullptr){
        head = newNode;
        newNode->next = nullptr;
    }else{
        if(head->data == 0){
            head->data = data;
            return;
        }
        newNode->next = head;
        head = newNode;
    }
}

char pop(Stack *&head){
    if(head == nullptr){
        cout << "Stack is empty";
        return 0;
    }
    char data = head->data;
    Stack *help = head;
    head = head->next;
    delete help;
    return data;
}

char top(Stack *&head){
    if(head == nullptr){
        cout << "Stack is empty";
        return 0;
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

int priority(char c){
    switch (c) {
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

vector<string> splitString(string str){
    vector<string> tokens;
    string str1 = "";
    for(char i : str){
        if(find(alphabet.begin(), alphabet.end(),i) != alphabet.end()){
            if (str1 != "") tokens.push_back(str1);
            str1 = i;
            tokens.push_back(str1);
            str1 = "";
        }else if(i >= '0' && i <= '9'){
            str1 += i;
        }
    }
    if (str1 != "") tokens.push_back(str1);
    return tokens;
}

string fromExpressionToReversePolishNotation(Stack *&stack, string str) {
    string res = "";
    vector<string> tokens = splitString(str);
    if (tokens.size() == 0){
        return "Wrong input!";
    }
    for (int i = 0; i < tokens.size(); i++) {
        if (find(alphabet.begin(), alphabet.end(), tokens[i][0]) != alphabet.end()) {
            if (tokens[i][0] == '(') {
                push(stack, tokens[i][0]);
            } else if (tokens[i][0] == ')') {
                while (top(stack) != '(') {
                    res += pop(stack);
                    res += ' ';
                }
                pop(stack);
            } else {
                if (!(isEmpty(stack) || priority(top(stack)) < priority(tokens[i][0]))) {
                    while (!isEmpty(stack) && priority(top(stack)) >= priority(tokens[i][0])) {
                        res += pop(stack);
                        res += ' ';
                    }
                }
                push(stack, tokens[i][0]);
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

int main() {
    Stack *stack = new Stack;
    string inputString = "";
    try {
        ifstream file(fileString);
        if (file.is_open()) {
            getline(file, inputString);
        }
        inputString.erase(remove_if(inputString.begin(), inputString.end(), ::isspace), inputString.end());
        cout << fromExpressionToReversePolishNotation(stack,  inputString);
        file.close();
    }catch (int i){
        cout << "Some exception!";
    }
}