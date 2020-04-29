//
// Created by Viegas, Jorge on 07/04/20.
//

#ifndef LITTLELIONMAN_FACE_H
#define LITTLELIONMAN_FACE_H


#include <vector>

class Face {
public:

    std::vector<int> textures;
    std::vector<int> vertices;
    std::vector<int> normals;

    int verticesCount(){
        return vertices.size();
    }



};


#endif //LITTLELIONMAN_FACE_H
