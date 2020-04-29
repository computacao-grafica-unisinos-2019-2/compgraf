//
// Created by Viegas, Jorge on 21/04/20.
//

#ifndef COMPGRAF_OBJ3D_H
#define COMPGRAF_OBJ3D_H


#include "Camera.h"

class Obj3D {
public:
    Mesh* mesh;
    float rotate;
    float scale;
    glm::vec3* translate;
    int status;
    bool canBeDestroyed;
    void draw(Camera *camera, Shader *shader){
        glm::mat4 projection = camera->getProjectionMatrix();
        glm::mat4 view = camera->getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, *translate);
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "model"), 1, GL_FALSE, &model[0][0]);

        for (int j = 0; j < mesh->groups.size(); ++j) {
            Group *group = mesh->groups[j];
            glBindVertexArray(group->VAO);
            glDrawArrays(GL_TRIANGLES, 0, group->numOfVertices);
        }
    }
};


#endif //COMPGRAF_OBJ3D_H
