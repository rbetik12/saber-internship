#include <iostream>
#include <unordered_map>
#include "Math.h"

void InitVertices(Vec3* vertices) {
    vertices[0].x = -1.0f;
    vertices[0].y = -1.0f;
    vertices[0].z = 0.0f;

    vertices[1].x = 1.0f;
    vertices[1].y = -1.0f;
    vertices[1].z = 0.0f;

    vertices[2].x = 1.0f;
    vertices[2].y = 1.0f;
    vertices[2].z = 0.0f;

    vertices[3].x = -1.0f;
    vertices[3].y = 1.0f;
    vertices[3].z = 0.0f;
}

void CheckIndexMap(std::unordered_map<int, Vec3>& map, int vertexIndex, int faceIndex) {
    if (map.find(vertexIndex) == map.end()) {
        map[vertexIndex] = Vec3();
        
    }
}

/*
* Here we calculate average per-vertex normals.
* First of all we calculate normal for each triangle.
* 
* Because we need to take average normal per each vertex (I decided to take weighted sum of each face normal that is adjacent for the vertex), we need to calculate normal weight for vertex normal.
* The weight of the normal for vertex is calculated as sin(angle), where angle is the angle between edges of the face that are adjacent to the vertex. Then we add weighted normal to vertex index. 
* 
* After all calculations done we iterate through the vertex index map and normalize vertices normal.
*/
void CalcMeshNormals(Vec3* normals, const Vec3* verts, const int* faces, int nverts, int nfaces) {
    IVec3* facesVec = (IVec3*)faces;
    std::unordered_map<int, Vec3> vertexIndexToWeigthedNormal;
    std::vector<Vec3> vertexNormals(nverts);

    for (int faceIndex = 0; faceIndex < nfaces; faceIndex++) {
        CheckIndexMap(vertexIndexToWeigthedNormal, facesVec[faceIndex].x, faceIndex);
        CheckIndexMap(vertexIndexToWeigthedNormal, facesVec[faceIndex].y, faceIndex);
        CheckIndexMap(vertexIndexToWeigthedNormal, facesVec[faceIndex].z, faceIndex);

        Vec3 normal = CalcFaceNormal(facesVec[faceIndex], verts);
        Vec3 p1 = verts[facesVec[faceIndex].x];
        Vec3 p2 = verts[facesVec[faceIndex].y];
        Vec3 p3 = verts[facesVec[faceIndex].z];

        float angle1 = sin((p2 - p1).Angle(p3 - p1));
        float angle2 = sin((p1 - p3).Angle(p2 - p3));
        float angle3 = sin((p1 - p2).Angle(p3 - p2));

        Vec3 normal1 = normal * angle1;
        Vec3 normal2 = normal * angle2;
        Vec3 normal3 = normal * angle3;

        vertexIndexToWeigthedNormal[facesVec[faceIndex].x] += normal1;
        vertexIndexToWeigthedNormal[facesVec[faceIndex].y] += normal2;
        vertexIndexToWeigthedNormal[facesVec[faceIndex].z] += normal3;
    }

    uint64_t index = 0;
    for (auto itr = vertexIndexToWeigthedNormal.begin(); itr != vertexIndexToWeigthedNormal.end(); itr++) {
        itr->second.Normalize();
        vertexNormals[index] = itr->second;
        index++;
    }
    std::memcpy(normals, vertexNormals.data(), sizeof(Vec3) * nverts);
}

int main() {
    Vec3 vertices[4];
    InitVertices(vertices);
    int faces[] = {
        0, 1, 2,
        2, 3, 0
    };
    Vec3* normals = new Vec3[4];
    CalcMeshNormals(normals, vertices, faces, 4, 2);
    for (uint64_t i = 0; i < 4; i++) {
        std::cout << "X: " << normals[i].x << " Y: " << normals[i].y << " Z: " << normals[i].z << std::endl;
    }

    delete[] normals;
    return 0;
}