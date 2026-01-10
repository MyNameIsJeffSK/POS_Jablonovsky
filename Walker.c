//
// Created by jakub on 06/01/2026.
//

#include "Walker.h"


Direction getDirection(int num, Walker *walker) {
    if (num <= walker->up_p) {
        return Up;
    }
    if (num <= (walker->down_p + walker->up_p)) {
        return Down;
    }
    if (num <= (walker->down_p + walker->up_p + walker->right_p)) {
        return Right;
    }
    return Left;
}

/*int main() {

    return 0;
}*/
