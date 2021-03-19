#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <bitset>

enum class Input { Right, Left, Grab };

struct Data
{
    std::bitset<3> m_input;
};

#endif