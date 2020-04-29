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

    int addNormal(glm::vec3 *normal){
        normals.push_back(normal);
        return normals.size() - 1;
    };

    int addVertex(glm::vec3 *vertex){
        vertices.push_back(vertex);
        return vertices.size() - 1;
    };

    int addMapping(glm::vec2 *mapping){
        mappings.push_back(mapping);
        return mappings.size() - 1;
    };

    int addGroup(Group *group){
        groups.push_back(group);
        return groups.size() - 1;
    };
};


#endif //LITTLELIONMAN_MESH_H
