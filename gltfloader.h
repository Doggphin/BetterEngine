#ifndef GLTFLOADER_H
#define GLTFLOADER_H

#include <vector>
#include <string>


struct GLTFData {
    std::vector<float> vertices;
    std::vector<float> indices;
    std::vector<float> normals;
    std::vector<float> texcoords;
};

class GLTFLoader {
    static GLTFData* loadMesh(std::string name);
};



#endif