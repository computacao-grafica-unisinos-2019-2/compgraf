//
// Created by Viegas, Jorge on 06/04/20.
//

#ifndef LITTLELIONMAN_MESH_H
#define LITTLELIONMAN_MESH_H

// GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Group.h"
#include <vector>


class Mesh {

public:
    std::vector<Group *> groups;
    std::vector<glm::vec2*> mappings;
    std::vector<glm::vec3*> vertices;
    std::vector<glm::vec3*> normals;

    int addGroup(Group *group){
        groups.push_back(group);
        return groups.size() - 1;
    };
};


#endif //LITTLELIONMAN_MESH_H
