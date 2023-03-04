#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using VecS = std::vector<char>;

enum Operator {
    Plus,
    Minus,
    Equals,
    None
};

std::map<char, VecS> combinations {
    {'1', VecS {'7'} },
    {'3', VecS {'9'} },
    {'5', VecS {'6', '9'} },
    {'6', VecS {'8'} },
    {'9', VecS {'8'} },
    {'0', VecS {'8'} },
    {'-', VecS {'+', '='}},
};

std::map<char, VecS> revCombinations{
    {'6', VecS {'5'} },
    {'7', VecS {'1'}},
    {'8', VecS {'6', '9', '0'}},
    {'9', VecS {'3', '5'}},
    {'+', VecS {'-'}},
    {'=', VecS {'-'}},
};

std::map<char, VecS> selfCombinations{
    {'1', VecS {'+'}},
    {'2', VecS {'3'}},
    {'3', VecS {'2', '5'}},
    {'5', VecS {'3'}},
    {'6', VecS {'0'}},
    {'9', VecS {'0'}},
    {'0', VecS {'6', '9'}},
    {'=', VecS {'+'}},
    {'+', VecS {'='}}
};

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

Operator getOperatorFromChar(const char op) {
    if (op == '+')
        return Operator::Plus;
    
    if (op == '-')
        return Operator::Minus;

    if (op == '=')
        return Operator::Equals;

    return Operator::None;
}

bool evalIsEquationTrue(std::string equation) {
    std::vector<int> totals;
    std::vector<std::string> parts = split(equation, '=');

    if (parts.size() <= 1)
        return false;

    for (int cPart = 0; cPart < parts.size(); cPart++) {
        bool wasLastCharOperator = true;
        std::string equationPart = parts[cPart];

        std::vector<short> numbers;
        std::vector<Operator> operators;
        std::string currentNumStr = "";

        for (int i = 0; i < equationPart.length(); i++) {
            const char currentChar = equationPart[i];

            if (std::isdigit(currentChar)) {
                wasLastCharOperator = false;
                currentNumStr += currentChar;
            }
            else {
                Operator currentOp = getOperatorFromChar(currentChar);

                if (wasLastCharOperator)
                    return false;

                if (currentOp == Operator::None)
                    continue;

                wasLastCharOperator = true;

                if (currentNumStr != "") {
                    numbers.push_back(std::stoi(currentNumStr));
                    currentNumStr = "";
                }

                operators.push_back(currentOp);
            }
        }

        if (currentNumStr != "") {
            numbers.push_back(std::stoi(currentNumStr));
        }

        if (numbers.size() <= 0)
            return false;

        totals.push_back(numbers[0]);

        for (int i = 0; i < numbers.size() - 1; i++) {
            const int currentNum = numbers[i + 1];
            const Operator currentOp = operators[i];

            if (currentOp == Operator::Plus)
                totals[cPart] += currentNum;

            else if (currentOp == Operator::Minus)
                totals[cPart] -= currentNum;
        }
    }

    int lastSum = totals[0];

    for (int sum : totals) {
        if (sum != lastSum) {
            return false;
        }
    }

    return true;
}

std::vector<std::string> getAllCombinations(std::string equation, short moves, short currentMove = 0) {
    std::vector<std::string> results;

    currentMove += 1;

    for (int i = 0; i < equation.length(); i++) {
        const char currentChar = equation[i];

        if (!combinations.count(currentChar))
            continue;

        if (selfCombinations.count(currentChar)) {
            for (const char replacement1 : selfCombinations[currentChar]) {
                std::string equationCopy = equation;
                std::string newEquation = equationCopy.replace(i, 1, std::string(1, replacement1));

                if (currentMove < moves) {
                    std::vector<std::string> newResults = getAllCombinations(newEquation, moves, currentMove + 1);
                    results.insert(results.end(), newResults.begin(), newResults.end());
                }
                else if (evalIsEquationTrue(newEquation)) {
                    results.push_back(newEquation);
                }
            }
        }

        for (const char replacement1 : combinations[currentChar]) {

            for (int j = 0; j < equation.length(); j++) {
                const char currentRevChar = equation[j];

                if (!revCombinations.count(currentRevChar)) {
                    continue;
                }

                for (const char replacement2 : revCombinations[currentRevChar]) {
                    std::string equationCopy = equation;

                    std::string newEquation = equationCopy.
                        replace(i, 1, std::string(1, replacement1)).
                        replace(j, 1, std::string(1, replacement2))
                    ;

                    if (currentMove < moves) {
                        std::vector<std::string> newResults = getAllCombinations(newEquation, moves, currentMove + 1);
                        results.insert(results.end(), newResults.begin(), newResults.end());
                    }
                    else if (evalIsEquationTrue(newEquation)) {
                        results.push_back(newEquation);
                    }
                }
            }
        }
    }

    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());

    return results;
}

int main()
{
    while (true) {
        std::string equation, movesStr;

        std::cout << "Enter the equation:" << std::endl << "> ";
        std::getline(std::cin, equation);

        std::cout << std::endl << "Enter the amount of moves:" << std::endl << "> ";
        std::getline(std::cin, movesStr);

        short moves = std::stoi(movesStr);

        std::cout << std::endl << "Here are all the correct equations you can receive from " << moves << " moves:" << std::endl;
        std::vector<std::string> results = getAllCombinations(equation, moves);

        for (std::string eq : results) {
            std::cout << eq << std::endl;
        }

        std::cout << std::endl;
    }  
}
