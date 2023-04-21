#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
const string fileTextString = "C:\\FirstCursProgramm\\C++\\stepik_lab_2.3\\text.txt";
const string fileResultString = "C:\\FirstCursProgramm\\C++\\stepik_lab_2.3\\result.txt";
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

string printStack(Stack *head){
    string res = "";
    while(!isEmpty(head)){
        res += head->data;
        res += " ";
        head = head->next;
    }
    return res;
}

int sizeStack(Stack *head){
    int count = 0;
    while(!isEmpty(head)){
        count++;
        head = head->next;
    }
    return count;
}
void addString(string &resultString, string addString, bool flag){
    resultString += addString;
    if(flag) resultString += "\n";
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
        }else{
            tokens.clear();
            return tokens;
        }
    }
    if (str1 != "" && str1 != " ") tokens.push_back(str1);
    return tokens;
}

string fromExpressionToReversePolishNotation(Stack *&stack, string str, string &resultString) {
    string res = "";
    int count1 = 0, count2 = 0;
    vector<string> tokens = splitString(str);
    if(tokens.size() == 0) return "В выражении допущена ошибка, невозможно вычислить обратную польскую нотацию:(";
    if (tokens.size() == 0){
        return "Wrong input!";
    }
    for (int i = 0; i < tokens.size(); i++) {
        if (find(alphabet.begin(), alphabet.end(), tokens[i]) != alphabet.end()) {
            if (tokens[i] == "(") {
                push(stack, tokens[i]);
                count1++;
            } else if (tokens[i] == ")") {
                count2++;
                while (top(stack) != "(" && !isEmpty(stack)) {
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
        addString(resultString, "Выходная строка:" + res, 0);
        addString(resultString, " Стек:" + printStack(stack), 1);
    }
    while(!isEmpty(stack)){
        res += pop(stack);
        res += ' ';
    }
    if(count1 != count2)
        return "Несоответсвие скобок, невозможно вычислить обратную польскую нотацию:(";
    return res;
}

string solveExpression(Stack *& stack, string str){
    if(str == "В выражении допущена ошибка, невозможно вычислить обратную польскую нотацию:(" || str == "Несоответсвие скобок, невозможно вычислить обратную польскую нотацию:(")
        return "ОШИБКА!";
    vector<string> tokens = splitString(str);
    for(int i = 0; i < tokens.size(); i++){
        if (find(alphabet.begin(), alphabet.end(), tokens[i]) != alphabet.end()) {
            if(sizeStack(stack) < 2) return "В выражении допущена ошибка, невозможно посчитать значение:(";
            push(stack, to_string(action(atof(pop(stack).c_str()), atof(pop(stack).c_str()), tokens[i])));

        }else{
            push(stack, tokens[i]);
        }
    }
    if(sizeStack(stack) == 1) return pop(stack);
    else{
        return "В выражении допущена ошибка, невозможно посчитать значение:(";
    }
}

int main() {
    Stack *stack = new Stack;
    string inputString, polishString, answerString, resultString = "";
    try {
        ifstream file(fileTextString);
        if (file.is_open()) {
            while(getline(file, inputString)){
                addString(resultString, "Изначальное выражение:", 0);
                addString(resultString, inputString, 1);
                inputString.erase(remove_if(inputString.begin(), inputString.end(), ::isspace), inputString.end());
                deleteStack(stack);
                polishString = fromExpressionToReversePolishNotation(stack,  inputString, resultString);
                deleteStack(stack);
                answerString = solveExpression(stack, polishString);
                addString(resultString, "Обратная польская нотация: ",0);
                addString(resultString, polishString, 1);
                addString(resultString, "Ответ: ", 0);
                addString(resultString, answerString, 1);
                addString(resultString, "--------------------------------------------------------------------", 1);
            }
        }
        file.close();
    }catch (int i){
        cout << "Some exception in input file!";
    }
    try{
        ofstream fileExit(fileResultString, ios::out | ios::trunc);
        if(fileExit.is_open()){
            fileExit << resultString;
        }
        fileExit.close();
    }catch (int i){
        cout << "Some exception in exit file!";
    }
    cout << resultString;
    getchar();
}