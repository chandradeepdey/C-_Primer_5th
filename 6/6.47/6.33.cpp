#include <iostream>
#include <vector>

void recursivePrint(std::vector<int>::const_iterator curr, std::vector<int>::const_iterator last) {
#ifndef NDEBUG
    std::clog << "In " << __func__ << ". Current vector size: " << last - curr << '\n';
#endif

    if (curr == last)
        return;
    else {
        std::cout << *curr << '\n';
        return recursivePrint(++curr, last);
    }
}

int main(void) {
    std::vector<int> vect{1, 2, 5, 6, 7, 8, 10};
    recursivePrint(vect.cbegin(), vect.cend());

    return 0;
}
