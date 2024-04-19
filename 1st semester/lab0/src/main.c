#include <stdio.h>
#include <stdlib.h>

const int kDeltaCharToInt = 48;
const int kDeltaAbcdefToInt = 87;
const int kDeltaAbcdefToIntForBigLetters = 55;
const int kNoNumber = -1000;

double powOptimised(int number, int power);

int checkForApprovedAlphabet(int num);

int main(void) {
    int b1, b2;
    int numbersBeforePoint[13];
    for (size_t i = 0; i < 13; i++) numbersBeforePoint[i] = kNoNumber;

    int numbersAfterPoint[13];
    for (size_t i = 0; i < 13; i++) numbersAfterPoint[i] = kNoNumber;

    if (scanf("%d %d", &b1, &b2) == 0) {
        printf("bad input");
        return 0;
    }
    if (b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16) {
        printf("bad input");
        return 0;
    }
    char inputNumber[14] = {0};

    if (scanf("%13s", inputNumber) == 0) {
        printf("bad input");
        return 0;
    }

    for (size_t i = 0; i < 13; i++) {
        if (i == 12 && inputNumber[i] == '.') {
            printf("bad input");
            return 0;
        } else if (i == 0) {
            if (inputNumber[i] == '.') {
                printf("bad input");
                return 0;
            }
        } else if (inputNumber[i] == '.' && checkForApprovedAlphabet(inputNumber[i + 1]) == 0) {
            printf("bad input");
            return 0;
        } else if (inputNumber[i] != '.' && checkForApprovedAlphabet(inputNumber[i]) == 0 && (inputNumber[i] != '0') && (inputNumber[i] != '\0')) {
            printf("bad input");
            return 0;
        }
    }
    for (size_t i = 0; i < 13; i++) {
        if ((int) inputNumber[i] >= '0' && (int) inputNumber[i] <= '9') {
            if (((int)inputNumber[i] - kDeltaCharToInt) >= b1) {
                printf("bad input");
                return 0;
            }
        }
        if ((int) inputNumber[i] >= 'a' && (int) inputNumber[i] <= 'f') {
            if (((int)inputNumber[i] - kDeltaAbcdefToInt) >= b1) {
                printf("bad input");
                return 0;
            }
        } else if ((int) inputNumber[i] >= 'A' && (int) inputNumber[i] <= 'F') {
            if (((int)inputNumber[i] - kDeltaAbcdefToIntForBigLetters) >= b1) {
                printf("bad input");
                return 0;
            }
        }
    }

    // Splitting the number and converting letters to numbers.
    short flag = 0;
    for (size_t i = 0; i < 13; i++) {
        if (inputNumber[i] == '.') {
            flag = 1;
        }
        if (flag == 0) {
            if ((int)inputNumber[i] >= '0' && (int)inputNumber[i] <= '9') {
                numbersBeforePoint[i] = (((int)inputNumber[i]) - kDeltaCharToInt);
            } else if ((int)inputNumber[i] >= 'a' && (int)inputNumber[i] <= 'f') {
                numbersBeforePoint[i] = (((int)inputNumber[i]) - kDeltaAbcdefToInt);
            } else if ((int)inputNumber[i] >= 'A' && (int)inputNumber[i] <= 'F') {
                numbersBeforePoint[i] = (((int)inputNumber[i]) - kDeltaAbcdefToIntForBigLetters);
            }
        } else if (flag == 1) {
            if ((int)inputNumber[i] >= '0' && (int)inputNumber[i] <= '9') {
                numbersAfterPoint[i] = (((int)inputNumber[i]) - kDeltaCharToInt);
            } else if ((int)inputNumber[i] >= 'a' && (int)inputNumber[i] <= 'f') {
                numbersAfterPoint[i] = (((int)inputNumber[i]) - kDeltaAbcdefToInt);
            } else if ((int)inputNumber[i] >= 'A' && (int)inputNumber[i] <= 'F') {
                numbersBeforePoint[i] = (((int)inputNumber[i]) - kDeltaAbcdefToIntForBigLetters);
            }
        }
    }
    // Conversion to decimal sys.
    int power = 0;
    long long integer_res10 = 0;

    for (int i = 12; i != -1; i--) {
        if (numbersBeforePoint[i] != kNoNumber) {
            integer_res10 = integer_res10 + numbersBeforePoint[i] * powOptimised(b1, power);
            power++;
        }
    }
    double fractional_res10 = 0;
    power = -1;
    for (int i = 0; i != 13; i++) {
        if (numbersAfterPoint[i] != kNoNumber) {
            fractional_res10 = fractional_res10 + numbersAfterPoint[i] * powOptimised(b1, power);
            power--;
        }
    }
    // Conversion from decimal to b2 sys.
    int integer_result[50] = {0};
    long long cloned_integer_res10 = integer_res10;
    int i = 49;
    while (cloned_integer_res10 != 0) {
        integer_result[i] = (cloned_integer_res10 % b2);
        cloned_integer_res10 /= b2;
        i--;
    }
    int fractional_result[14] = {0};
    double tmp = 0;
    tmp = fractional_res10;
    i = 0;
    while (1) {
        fractional_result[i] = (int)(tmp * b2);
        tmp *= b2;
        tmp -= ((int)(tmp));
        i++;
        if (i == 12) break;
    }
    flag = 0;
    int flag_to_check_no_output = 0;
    for (size_t i = 0; i < 50; i++) {
        if (integer_result[i] != 0) {
            flag = 1;
        }
        if (flag) {
            flag_to_check_no_output = 1;
            if (integer_result[i] >= 0 && integer_result[i] <= 9) {
                printf("%d", integer_result[i]);
            } else {
                switch (integer_result[i]) {
                    case 10:
                        printf("a");
                        break;
                    case 11:
                        printf("b");
                        break;
                    case 12:
                        printf("c");
                        break;
                    case 13:
                        printf("d");
                        break;
                    case 14:
                        printf("e");
                        break;
                    case 15:
                        printf("f");
                        break;
                }
            }
        }
    }
    if (flag_to_check_no_output == 0) {
        printf("0");
    }
    flag = 0;
    for (int j = 0; j < 12; j++) {
        if (fractional_result[j] != 0) {
            flag = 1;
        }
    }
    printf(".");
    for (size_t i = 0; i < 12; i++) {
        flag = 0;
        for (int j = i; j < 12; j++) {
            if (fractional_result[j] != 0) {
                flag = 1;
            }
        }
        if (fractional_result[i] >= 0 && fractional_result[i] <= 9) {
            printf("%d", fractional_result[i]);
        } else {
            switch (fractional_result[i]) {
                case 10:
                    printf("a");
                    break;
                case 11:
                    printf("b");
                    break;
                case 12:
                    printf("c");
                    break;
                case 13:
                    printf("d");
                    break;
                case 14:
                    printf("e");
                    break;
                case 15:
                    printf("f");
                    break;
            }
        }
    }
}

double powOptimised(int number, int power) {
    double res = 1;
    if (power > 0) {
        for (int i = 0; i < power; i++) {
            res *= number;
        }
    } else {
        for (int i = -1; i >= power; i--) {
            res /= number;
        }
    }
    return res;
}

int checkForApprovedAlphabet(int num) {
    if ((num >= '0' && num <= '9') || (num >= 'a' && num <= 'f') || (num >= 'A' && num <= 'F')) {
        return 1;
    } else {
        return 0;
    }
}
