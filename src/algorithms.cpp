#include "algorithms.hpp"
#include <stdio.h>
#include <string>
#include <ctime>
#include <iostream>
#include <cstdlib>

/* Various useful alogrithms should be added here. */

std::string
RandomNameGenerator() {
    std::string name("");
    const char Vowels[6]  = { 'a', 'e', 'i', 'o', 'u' };
    const char Conson[20] = { 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
    char ch;

    /* Seed the randomizer. */
    srand((unsigned)time(0));

    /* The form of the names will be VCC-VCV or CVC-CVC. */
    int j = rand() % 2;

    while (name.size() < 7) {

        /* Name starts with a vowel. */
        if (j != 0) {
            ch = toupper(Vowels[(rand() % 5)]);
            name.insert(0, 1, ch);
            ch = Conson[(rand() % 19)];
            name.insert(1, 1, ch);
            ch = Conson[(rand() % 19)];
            name.insert(2, 1, ch);
            ch = Vowels[(rand() % 5)];
            name.insert(3, 1, ch);
            ch = Conson[(rand() % 19)];
            name.insert(4, 1, ch);
            ch = Vowels[(rand() % 5)];
            name.insert(5, 1, ch);
            break;
        }
        else {
            /* Name starts with a consonate. */
            ch = toupper(Conson[(rand() % 19)]);
            name.insert(0, 1, ch);
            ch = Vowels[(rand() % 5)];
            name.insert(1, 1, ch);
            ch = Conson[(rand() % 19)];
            name.insert(2, 1, ch);
            ch = Conson[(rand() % 19)];
            name.insert(3, 1, ch);
            ch = Vowels[(rand() % 5)];
            name.insert(4, 1, ch);
            ch = Conson[(rand() % 19)];
            name.insert(5, 1, ch);
            break;
        }
    }
 
    return name;
}
