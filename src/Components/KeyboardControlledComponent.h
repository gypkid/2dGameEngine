#ifndef KEYBOARDCONTROLLEDCOMPONENT_H
#define KEYBOARDCONTROLLEDCOMPONENT_H

#include <glm/glm.hpp>

struct KeyboardControlledComponent
{   
    glm::vec2 upVelocity;
    glm::vec2 rightVelocity;
    glm::vec2 leftVelocity;
    glm::vec2 downVelocity;

    KeyboardControlledComponent(
        glm::vec2 upVelocity = glm::vec2(0), 
        glm::vec2 rightVelocity = glm::vec2(0),
        glm::vec2 leftVelocity = glm::vec2(0),
        glm::vec2 downVelocity = glm::vec2(0)) {
            this->upVelocity = upVelocity;
            this->rightVelocity = rightVelocity;
            this->leftVelocity = leftVelocity;
            this->downVelocity = downVelocity;
    }
};

#endif