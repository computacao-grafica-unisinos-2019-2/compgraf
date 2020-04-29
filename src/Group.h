//
// Created by Viegas, Jorge on 06/04/20.
//

#ifndef LITTLELIONMAN_GROUP_H
#define LITTLELIONMAN_GROUP_H

#include "Face.h"
#include <vector>


class Group {

public:
    std::vector<Face *> faces;

    int addFace(Face *face){
        faces.push_back(face);
        return faces.size() - 1;
    };

    int numOfVertices;
    GLuint VAO;

};


#endif //LITTLELIONMAN_GROUP_H
