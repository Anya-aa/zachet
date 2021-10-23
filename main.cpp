#include <iostream>
#include "LinkedList.h"
#include "fstream"
#include <vector>
#include <sstream>
#include <iterator>
int main()
{


    std::ifstream input;
    input.open("./input");
    if (!input.is_open())
    {
        throw "ERROR";
    }
    SinglyLinkedList<std::string> list1;
    std::copy(std::istream_iterator<std::string>(input), std::istream_iterator<std::string>(), std::back_inserter(list1));

    std::ofstream output;
    output.open("./output");
    if (!output.is_open())
    {
        throw "ERROR";
    }
    std::copy(list1.begin(), list1.end(), std::ostream_iterator<std::string>(output, "\n"));
    return 0;
}