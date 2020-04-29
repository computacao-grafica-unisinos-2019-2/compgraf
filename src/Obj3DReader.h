//
// Created by Viegas, Jorge on 21/04/20.
//

#ifndef COMPGRAF_OBJ3DREADER_H
#define COMPGRAF_OBJ3DREADER_H

#include "Obj3D.h"


class Obj3DReader {

public:

    Obj3D *readObjFromFile(std::string fileName) {

        Obj3D *obj3D = new Obj3D();
        Mesh *mesh = new Mesh();
        Group *currentGroup = new Group();
        obj3D->mesh = mesh;
        int verticesCount = 0;

        std::ifstream objFile(fileName);

        int count = 0;
        while (!objFile.eof()){

            count++;
            std::string line;
            getline(objFile, line);
            std::stringstream sline;
            sline << line;
            std::string firstToken;
            sline >> firstToken;

            std::cout << "\n Linha: " << count << " Token: " << firstToken;

            if(firstToken == "v"){
                float x, y, z;
                sline >> x >> y >> z;
                mesh->vertices.push_back(new glm::vec3(x,y,z));
            } else if(firstToken == "vn"){
                float x, y, z;
                sline >> x >> y >> z;
                mesh->normals.push_back(new glm::vec3(x,y,z));
            } else if(firstToken == "vt"){
                float x, y;
                sline >> x >> y ;
                mesh->mappings.push_back(new glm::vec2(x,y));
            } else if(firstToken == "g"){
                currentGroup = new Group();
                mesh->addGroup(currentGroup);
            } else if(firstToken == "f"){
                processFLine(sline, currentGroup);
            }
        }

        return obj3D;
    }

private:

    void processTriplet(Face *face, std::string token){
        std::stringstream stoken(token);
        std::string aux;


        getline(stoken, aux , '/');
        if(!aux.empty()) {
            face->vertices.push_back(stoi(aux) - 1);
        }

        getline(stoken, aux , '/');
        if(!aux.empty()){
            face->textures.push_back(stoi(aux) - 1);
        }
        getline(stoken, aux);
        if(!aux.empty()) {
            face->normals.push_back(stoi(aux) - 1);
        }
    }

    void processFLine(std::stringstream &sline, Group *group){
        Face *face = new Face();
        std::string first, second, third, fourth, token;
        sline >> first;
        processTriplet(face, first);
        group->numOfVertices++;

        sline >> second;
        processTriplet(face, second);
        group->numOfVertices++;

        sline >> third;
        processTriplet(face, third);
        group->numOfVertices++;

        sline >> fourth;
        if (!fourth.empty()){
            std::stringstream newFLine(first + " " + third + " " + fourth);
            processFLine(newFLine, group);
        }

        group->addFace(face);
    }
};





#endif //COMPGRAF_OBJ3DREADER_H
