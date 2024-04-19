#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 5000
#define CHAR_TO_INT_COEF 48
#define BUFFER_LENGTH 15
#define NO_NUMBER -100

#define DivideByZeroException -18281828

enum Operation {
    SUM = -1, // Sum.
    SUB = -2, // Substraction.
    MUL = -3, // Multiplication.
    DIV = -4, // Division.
};

typedef struct TagStack {
    char data;
    int data_int;
    struct TagStack* next;
} Stack;

Stack* PushChar(Stack* top, char data) {
    Stack* ptr = malloc(sizeof(Stack));
    ptr->data = data;
    ptr->next = top;
    return ptr;
}

Stack* PushInt(Stack* top, int data_int) {
    Stack* ptr = malloc(sizeof(Stack));
    ptr->data_int = data_int;
    ptr->next = top;
    return ptr;
}

Stack* Pop(Stack* top) {
    if (top == NULL) {
        return top;
    }
    Stack* ptr_next = top->next;
    free(top);
    return ptr_next;
}

void DeleteStack(Stack* top) {
    while (top != NULL) {
        top = Pop(top);
    }
    free(top);
}

void ClearString(char* string, size_t length) {
    for (size_t i = 0; i < length; i++) {
        string[i] = '\0';
    }
}

int LinearPow(int number, int power) {
    int res = 1;
    if (power > 0) {
        for (int i = 0; i < power; i++) {
            res *= number;
        }
    }
    return res;
}

int GetPriority(char operation) { 
    if (operation == '(' || operation == ')') {
        return 1;
    }
    if (operation == '+' || operation == '-') {
        return 2;
    }
    if (operation == '*' || operation == '/') {
        return 3;
    }
    return -1;
}

bool IsNumber(char elem) { 
    return elem >= '0' && elem <= '9'; 
}

bool IsAritmeticOperation(char elem) { 
    return elem >= '*' && elem <= '/';
}

bool IsAlhabetCorrect(char elem) {
    return elem >= '(' && elem <= '9' && elem != '.' && elem != ',';
}

bool IsInputCorrect(char* input, size_t input_length) {
    if (input[0] == 0) {
        return false;
    }
    // Checking the brackets and the alhabet.
    short opened_brackets_amount = 0;
    short closed_brackets_amount = 0;
    for (size_t i = 0; i < input_length; i++) {
        if (!IsAlhabetCorrect(input[i])) {
            return false;
        }
        if (input[i] == '(') {
            ++opened_brackets_amount;
            if (i < input_length - 1) {
                if ((input[i + 1] < '0' || input[i + 1] > '9') && input[i + 1] != '(') {
                    return false;
                }
            }
        } else if (input[i] == ')') {
            ++closed_brackets_amount;
        }
        if (closed_brackets_amount > opened_brackets_amount) {
            return false;
        }
    }
    if (opened_brackets_amount != closed_brackets_amount) {
        return false;
    }
    // Checking the arithmetic operations.
    for (size_t i = 0; i < input_length - 1; i++) {
        if (IsAritmeticOperation(input[i])) {
            if (IsAritmeticOperation(input[i + 1])) {
                return false;
            } else if ((input[i + 1] < '0' || input[i + 1] > '9') && input[i + 1] != '(') {
                return false;
            }
        }
    }
    if (IsAritmeticOperation(input[input_length - 1])) {
        return false;
    }
    return true;
}

int ConvertOperToNumber(char operation) { 
    if (operation == '+') {
        return SUM;
    }
    if (operation == '-') {
        return SUB;
    }
    if (operation == '*') {
        return MUL;
    }
    if (operation == '/') {
        return DIV;
    }
    return 0;
}

long ConvertCharToNumber(char* char_number, size_t length) {
    long result = 0;
    long power = LinearPow(10, length - 1);
    for (size_t i = 0; i < length; i++) {
        result += ((int)char_number[i] - CHAR_TO_INT_COEF) * power;
        power /= 10;
    }
    return result;
}

void RecordBuffer(int* output, char* buffer, int* buffer_count, int* last_elem) {
    output[*last_elem] = ConvertCharToNumber(buffer, strlen(buffer));
    ClearString(buffer, BUFFER_LENGTH);
    *buffer_count = 0;
    ++*last_elem;
}

void ConvertToInfixNotation(char* input, int* output, Stack* op_stack) {
    int count = MAX_INPUT_LENGTH - 1;
    int input_count = 0;
    int last_elem = 0;
    char buffer[BUFFER_LENGTH];
    ClearString(buffer, BUFFER_LENGTH);
    int buffer_count = 0;
    while (count >= 0) {
        char elem = input[input_count];
        if (elem == '\0') {
            if (input[strlen(input) - 1] == ')') {
                last_elem++;
                break;
            }
            RecordBuffer(output, buffer, &buffer_count, &last_elem);
            break;
        }
        if (IsNumber(elem)) {
            buffer[buffer_count] = elem;
            ++buffer_count;
        } else if (elem == '(') {
            if (buffer[0] != '\0') {
                RecordBuffer(output, buffer, &buffer_count, &last_elem);
            }
            op_stack = PushChar(op_stack, elem);
        } else if (elem == ')') {
            if (buffer[0] != '\0') {
                RecordBuffer(output, buffer, &buffer_count, &last_elem);
            }
            while (op_stack->data != '(') {
                output[last_elem] = ConvertOperToNumber(op_stack->data);
                op_stack = Pop(op_stack);
                ++last_elem;
            }
            op_stack = Pop(op_stack);
        } else {
            if (buffer[0] != '\0') {
                RecordBuffer(output, buffer, &buffer_count, &last_elem);
            }
            int priority = GetPriority(elem);
            while (op_stack != NULL && GetPriority(op_stack->data) >= priority) {
                output[last_elem] = ConvertOperToNumber(op_stack->data);
                op_stack = Pop(op_stack);
                ++last_elem;
            }
            op_stack = PushChar(op_stack, elem);
        }
        --count;
        ++input_count;
    }
    if (input[strlen(input) - 1] == ')') {
        --last_elem;
    }
    while (op_stack != NULL) {
        output[last_elem] = ConvertOperToNumber(op_stack->data);
        op_stack = Pop(op_stack);
        ++last_elem;
    }
}

int CalculateInfix(int* output) {
    Stack* queue = NULL;
    short count = 0;
    int num1 = 0, num2 = 0, res = 0;
    while (output[count] != NO_NUMBER) {
        if (output[count] > -1) {
            queue = PushInt(queue, output[count]);
            ++count;
        } else {
            num1 = queue->data_int;
            queue = Pop(queue);
            num2 = queue->data_int;
            queue = Pop(queue);
            if (output[count] == SUM) {
                res = num1 + num2;
            }
            else if (output[count] == SUB) {
                res = num2 - num1;
            }
            else if (output[count] == MUL) {
                res = num2 * num1;
            }
            else if (output[count] == DIV && num1 != 0) {
                res = num2 / num1;
            }
            else {
                res = DivideByZeroException;
            }
            if (res == DivideByZeroException) {
                DeleteStack(queue);
                return DivideByZeroException;
            }
            queue = PushInt(queue, res);
            ++count;
        }
    }
    res = queue->data_int;
    queue = Pop(queue);
    return res;
}

int Calculator (char* input) {
    Stack* op_stack = NULL;
    int* output = calloc(MAX_INPUT_LENGTH, sizeof(int));
    for (size_t i = 0; i < MAX_INPUT_LENGTH; i++) {
        output[i] = NO_NUMBER;
    }
    ConvertToInfixNotation(input, output, op_stack);
    DeleteStack(op_stack);
    int res = CalculateInfix(output);
    if (res == DivideByZeroException) {
        printf("division by zero");
        free(output);
        return DivideByZeroException;
    }
    free(output);
    return res;
}

int main(void) {
    char* input = calloc(MAX_INPUT_LENGTH, sizeof(char));
    char temp;
    int last_position = 0;
    while (scanf("%c", &temp)) {
        if (temp == '\n') break;
        input[last_position] = temp;
        last_position++;
    }
    if (!IsInputCorrect(input, strlen(input))) {
        printf("syntax error");
        free(input);
        return EXIT_SUCCESS;
    }
    int res = Calculator(input);
    if (res != DivideByZeroException) {
        printf("%d", res);
    }
    free(input);
    return EXIT_SUCCESS;
}
