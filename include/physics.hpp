//
// Created by benji on 22.06.26.
//

#ifndef RAYLIBPLAYGROUND_PHYSICS_HPP
#define RAYLIBPLAYGROUND_PHYSICS_HPP

#include <raylib.h>

inline bool colliding(BoundingBox a, BoundingBox b) {
    bool xOverlap = false;
    bool yOverlap = false;
    if (a.max.x > b.min.x || a.min.x < b.max.x) {
        xOverlap = true;
    }
    if (a.max.y > b.min.y || a.min.y < b.max.y) {
        yOverlap = true;
    }
}

#endif //RAYLIBPLAYGROUND_PHYSICS_HPP
