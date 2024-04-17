#include <iostream>

using namespace std;

#define MAXINTLENGTH 15 //decimal digits
#define MIN 14
#define MAX 15

void PrintingOperators(int operands, int commas);


class NodeStack {
public:
    int commas = 0;
    int data;
    bool special;
    NodeStack* next;

    NodeStack(int val, bool spec, int comm) {
        commas = comm;
        data = val;
        special = spec;
        next = nullptr;
    }
};

class Stack {
private:
    NodeStack* top; 
    int size;      

public:
    Stack() {
        top = nullptr;
        size = 0;
    }

    // Push function to add an element 
    void push(int val, bool spec, int comm) {
        NodeStack* newNode = new NodeStack(val, spec, comm);
        newNode->next = top;
        top = newNode;
        size++;
    }

    // Pop function to remove and return 
    int pop() {
        if (isEmpty()) {
            return -1;
        }
        NodeStack* tmp = top;
        int poppedValue = tmp->data;
        top = top->next;
        delete tmp;
        size--;
        return poppedValue;
    }

    int seeTheTop() {
        if (isEmpty()) {
            return -1;
        }
        return top->data;
    }

    void printStack() {
        if (isEmpty()) {
            return;
        }
        NodeStack* tmp = top;
        while (tmp != nullptr) {
            if (tmp->special) {
                PrintingOperators(tmp->data, tmp->commas);
            }
            else {
                cout << tmp->data << " ";
            }
            tmp = tmp->next;
        }
        cout << endl;
    }

    void addCommas() {
        top->commas++;
    }

    int countCommasAtTop() {
        if (isEmpty()) {
            return 0;
        }
        return top->commas;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};


class NodeQueue {
public:
    int commas = 0;
    int data;
    bool special;
    NodeQueue* next;

    NodeQueue(int val, bool spec, int comm ) {
        commas = comm;
        data = val;
        special = spec;
        next = nullptr;
    }
};

class Queue {
private:
    NodeQueue* front; 
    NodeQueue* rear;
    int size;       

public:
    Queue() {
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

    // Enqueue function to add an element 
    void enqueue(int val, bool spec, int comm) {
        NodeQueue* newNode = new NodeQueue(val, spec, comm);
        if (isEmpty()) {
            front = newNode;
        }
        else {
            rear->next = newNode;
        }
        rear = newNode;
        size++;
    }

    // Dequeue function to remove and return 
    int dequeue() {
        if (isEmpty()) {
            return -1; 
        }
        NodeQueue* tmp = front;
        int dequeuedValue = tmp->data;
        front = front->next;
        delete tmp;
        size--;
        if (front == nullptr) {
            rear = nullptr;
        }
        return dequeuedValue;
    }

    bool isSpecialAtFront() {
        if (isEmpty()) {
            return false;
        }
        return front->special;
    }

    void addCommasToFirst() {
        if (front != nullptr) {
            front->commas++;
        }
    }

    int countCommasAtFirst() {
        if (isEmpty()) {
            return 0;
        }
        return front->commas;
    }

    const bool isEmpty() {
        return front == nullptr;
    }
};

void ResetHolder(char holder[MAXINTLENGTH]) {
    for (int i = 0; i < MAXINTLENGTH; i++)
        holder[i] = '\0';
}

int SymbolIntoInt(char operand[MAXINTLENGTH]) {
    switch (operand[0])
    {
    case '+':
        return '+';
    case '-':
        return '-';
    case '*':
        return '*';
    case '/':
        return '/';
    case 'N':
        return 'N';
    case 'I':
        return 'I';
    case 'M':
        if (operand[2] == 'X')
            return MAX;
        else
            return MIN;
    default:
        break;
    }
    return 0;
}

int PriorityLevel(int token) {
    switch (token)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case 'N':
    case 'I':
    case MAX:
    case MIN:
        return 3;
    case '(':
        return 0;
    default:
        break;
    }
    return 0;
}

bool ImportanceLevel(int current_token, int token_on_the_stack) {
    int current_token_priority = PriorityLevel(current_token);
    int token_on_the_stack_priority = PriorityLevel(token_on_the_stack);
    if (token_on_the_stack_priority == 3 && current_token_priority == 3)
        return false;
    else if (token_on_the_stack_priority == 0)
        return false;
    return  token_on_the_stack_priority >= current_token_priority;
}

void PrintingOperators(int operands, int commas) {
    if (operands == 'I')
        cout << "IF" << " ";
    else if (operands == MAX)
        cout << "MAX" << commas << " ";
    else if (operands == MIN)
        cout << "MIN" << commas << " ";
    else
        cout << (char)operands << " ";
}

void RPNCalculator(Queue* RPN, bool* calculate) {
    cout << endl;
    cout << endl;
    Stack courent_calculation;
    while (!RPN->isEmpty() && *calculate == true) {
        while (!RPN->isSpecialAtFront() && !RPN->isEmpty()) {
            courent_calculation.push(RPN->dequeue(), false, NULL);
        }
        int comm_tmp = RPN->countCommasAtFirst();
        courent_calculation.push(RPN->dequeue(), true, comm_tmp);
        courent_calculation.printStack();
        int a, b, c;
        switch (courent_calculation.pop())
        {
        case '+':
            b = courent_calculation.pop();
            a = courent_calculation.pop();
            courent_calculation.push(a + b, false, NULL);
            break;
        case '-':
            b = courent_calculation.pop();
            a = courent_calculation.pop();
            courent_calculation.push(a - b, false, NULL);
            break;
        case '*':
            b = courent_calculation.pop();
            a = courent_calculation.pop();
            courent_calculation.push(a * b, false, NULL);
            break;
        case '/':
            b = courent_calculation.pop();
            a = courent_calculation.pop();
            if (b == 0) {
                cout << "ERROR" << endl;
                *calculate = false;
                break;
            }
            courent_calculation.push(a / b, false, NULL);
            break;
        case 'N':
            a = courent_calculation.pop();
            courent_calculation.push(a * (-1), false, NULL);
            break;
        case 'I':
            c = courent_calculation.pop();
            b = courent_calculation.pop();
            a = courent_calculation.pop();
            if (a > 0)
                courent_calculation.push(b, false, NULL);
            else
                courent_calculation.push(c, false, NULL);
            break;
        case MAX:
            if (comm_tmp == 1) {
            }
            else {
                int tmp = courent_calculation.pop();
                for (int k = 0; k < comm_tmp - 1; k++) {
                    a = courent_calculation.pop();
                    if (a > tmp)
                        tmp = a;
                }
                courent_calculation.push(tmp, false, NULL);
            }
            break;
        case MIN:
            if (comm_tmp == 1) {
            }
            else {
                int tmp = courent_calculation.pop();
                for (int k = 0; k < comm_tmp - 1; k++) {
                    a = courent_calculation.pop();
                    if (a < tmp)
                        tmp = a;
                }
                courent_calculation.push(tmp, false, NULL);
            }
            break;
        default:
            break;
        }

    }
    if (*calculate == true)
        cout << courent_calculation.pop();
    *calculate = false;
}

void SwichForRightParenteces(Stack* stack_operators, Queue* RPN) {
    while (true) {
        //printing  operators in the stack until matching parentesis
        int commas = stack_operators->countCommasAtTop() + 1;
        int tmp = stack_operators->pop();
        if (!stack_operators->isEmpty() && tmp != '(') {
            PrintingOperators(tmp, NULL);
            RPN->enqueue(tmp, true, NULL);
        }
        else if (stack_operators->seeTheTop() == 'I') {
            tmp = stack_operators->pop();
            PrintingOperators(tmp, NULL);
            RPN->enqueue(tmp, true, NULL);
            break;
        }
        else if (stack_operators->seeTheTop() == MAX) {
            tmp = stack_operators->pop();
            PrintingOperators(tmp, commas);
            RPN->enqueue(tmp, true, commas);
            break;
        }
        else if (stack_operators->seeTheTop() == MIN) {
            tmp = stack_operators->pop();
            PrintingOperators(tmp, commas);
            RPN->enqueue(tmp, true, commas);
            break;
        }
        else {
            break;
        }
    }
}

int main() {
    //reading number of formulas
    int nr_of_fomuals;
    cin >> nr_of_fomuals;
    cout << endl;
    //declaring the list and the stack

    char courent_symbol[MAXINTLENGTH];///15 znakow

    for (int i = 0; i < nr_of_fomuals; i++) {
        Stack stack_operators;
        Queue RPN;
        bool calculate = true;
        while (calculate) {
            ResetHolder(courent_symbol);

            //reading an input
            cin >> courent_symbol;

            // checking if that is a noumber at the input
            if (courent_symbol[0] >= '0' && courent_symbol[0] <= '9') {
                int tmp = atoi(courent_symbol);
                RPN.enqueue(tmp, false, NULL);
                cout << tmp << " ";
            }
            else {
                // if not convert an operator into number
                int operand = SymbolIntoInt(courent_symbol);

                if (courent_symbol[0] == '(') {
                    stack_operators.push('(', true, NULL);
                }
                else if (courent_symbol[0] == ')') {
                    SwichForRightParenteces(&stack_operators, &RPN);
                }
                else if (courent_symbol[0] == ',') {
                    while (stack_operators.seeTheTop() != '(') {
                        int tmp = stack_operators.pop();
                        PrintingOperators(tmp, NULL);
                        RPN.enqueue(tmp, true, NULL);
                    }
                    stack_operators.addCommas();
                }
                else if (courent_symbol[0] == '.') {
                    while (!stack_operators.isEmpty())
                    {
                        //printing all operators in the stack couse the line has ended
                        int tmp = stack_operators.pop();
                        PrintingOperators(tmp, NULL);
                        RPN.enqueue(tmp, true, NULL);
                    }
                    RPNCalculator(&RPN, &calculate);
                }
                else {
                    int skipper = 0;
                    while (!RPN.isEmpty() && !stack_operators.isEmpty() && ImportanceLevel(operand, stack_operators.seeTheTop())) {
                        int tmp = stack_operators.pop();
                        PrintingOperators(tmp, NULL);
                        RPN.enqueue(tmp, true, NULL);
                        skipper++;
                    }
                    if (skipper >= 1)
                        stack_operators.push(operand, true, NULL);
                    else if (skipper == 0) {
                        stack_operators.push(operand, true, NULL);
                    }
                }
            }
        }
        cout << endl;
        cout << endl;
        while (!stack_operators.isEmpty())
            stack_operators.pop();
        while (!RPN.isEmpty())
            RPN.dequeue();
    }
    return 0;
}
