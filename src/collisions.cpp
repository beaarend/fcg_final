#include <collisions.hpp>
#include <iostream>
#include <vector>
#include "matrices.h"

glm::vec4 vec3ToVec4(const glm::vec3& v, float w = 0.0f) {
    return glm::vec4(v, w);
}

bool Collisions::AABBsTest(const glm::vec3& min1, const glm::vec3& max1, const glm::vec3& min2, const glm::vec3& max2){
    
  //std::cout<<"testando colisao entre os pontos: "<<min1.x<<" "<<min1.y<<" "<<min1.z<<" e "<<max1.x<<" "<<max1.y<<" "<<max1.z<<std::endl;

  glm::vec3 temp1=min1;
  glm::vec3 temp2=max1;
  glm::vec3 temp3=min2;
  glm::vec3 temp4=max2;

  if(temp1.x > temp2.x) std::swap(temp1.x, temp2.x);
  if(temp1.y > temp2.y) std::swap(temp1.y, temp2.y);
  if(temp1.z > temp2.z) std::swap(temp1.z, temp2.z);
  if(temp3.x > temp4.x) std::swap(temp3.x, temp4.x);
  if(temp3.y > temp4.y) std::swap(temp3.y, temp4.y);
  if(temp3.z > temp4.z) std::swap(temp3.z, temp4.z);

    return (temp1.x <= temp4.x && temp2.x >= temp3.x) &&
           (temp1.y <= temp4.y && temp2.y >= temp3.y) &&
           (temp1.z <= temp4.z && temp2.z >= temp3.z);
}

// Função auxiliar para verificar se há um plano separador entre dois OBBs
bool GetSeparatingPlane(
    const glm::vec3& relativePos, const glm::vec3& plane,
    const glm::vec3& halfSize1, const glm::vec3 axis1[3],
    const glm::vec3& halfSize2, const glm::vec3 axis2[3]
) {
    float projection1 =
        fabs(Matrices::DotProduct(vec3ToVec4(axis1[0] * halfSize1.x), vec3ToVec4(plane))) +
        fabs(Matrices::DotProduct(vec3ToVec4(axis1[1] * halfSize1.y), vec3ToVec4(plane))) +
        fabs(Matrices::DotProduct(vec3ToVec4(axis1[2] * halfSize1.z), vec3ToVec4(plane)));

    float projection2 =
        fabs(Matrices::DotProduct(vec3ToVec4(axis2[0] * halfSize2.x), vec3ToVec4(plane))) +
        fabs(Matrices::DotProduct(vec3ToVec4(axis2[1] * halfSize2.y), vec3ToVec4(plane))) +
        fabs(Matrices::DotProduct(vec3ToVec4(axis2[2] * halfSize2.z), vec3ToVec4(plane)));

    float distance = fabs(Matrices::DotProduct(vec3ToVec4(relativePos), vec3ToVec4(plane)));

    return distance > (projection1 + projection2);
}

bool Collisions::OBBsTest(
    const glm::vec3& center1, const glm::vec3& halfSize1, const glm::vec3 axis1[3],
    const glm::vec3& center2, const glm::vec3& halfSize2, const glm::vec3 axis2[3]
) {
    // std::cout<<"testando colisoes entre OBBs"<<std::endl;
    // std::cout<<"center1: "<<center1.x<<" "<<center1.y<<" "<<center1.z<<std::endl;
    // std::cout<<"center2: "<<center2.x<<" "<<center2.y<<" "<<center2.z<<std::endl;
    // std::cout<<"halfSize1: "<<halfSize1.x<<" "<<halfSize1.y<<" "<<halfSize1.z<<std::endl;
    // std::cout<<"halfSize2: "<<halfSize2.x<<" "<<halfSize2.y<<" "<<halfSize2.z<<std::endl;
    // for(int i=0;i<3;i++){
    //   std::cout<<"axis1: "<<axis1[i].x<<" "<<axis1[i].y<<" "<<axis1[i].z<<std::endl;
    //   std::cout<<"axis2: "<<axis2[i].x<<" "<<axis2[i].y<<" "<<axis2[i].z<<std::endl;
    // }
    glm::vec3 relativePos = center2 - center1;

    if (GetSeparatingPlane(relativePos, axis1[0], halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, axis1[1], halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, axis1[2], halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, axis2[0], halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, axis2[1], halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, axis2[2], halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[0]), vec3ToVec4(axis2[0]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[0]), vec3ToVec4(axis2[1]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[0]), vec3ToVec4(axis2[2]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[1]), vec3ToVec4(axis2[0]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[1]), vec3ToVec4(axis2[1]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[1]), vec3ToVec4(axis2[2]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[2]), vec3ToVec4(axis2[0]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[2]), vec3ToVec4(axis2[1]))), halfSize1, axis1, halfSize2, axis2) ||
        GetSeparatingPlane(relativePos, vec3ToVec4(Matrices::CrossProduct(vec3ToVec4(axis1[2]), vec3ToVec4(axis2[2]))), halfSize1, axis1, halfSize2, axis2)) {
        return false; // Existe um plano separador
    }

    // Não há plano separador, os OBBs colidem
    return true;
}

bool Collisions::TestOBBCollisionVertices(glm::vec3* verticesA, glm::vec3* verticesB) {

    glm::vec3 verticeMaxA = verticesA[0];
    glm::vec3 verticeMinA = verticesA[0];
    glm::vec3 verticeMaxB = verticesB[0];
    glm::vec3 verticeMinB = verticesB[0];
    for(int i=0;i<8;i++){
        verticeMaxA = glm::max(verticeMaxA, verticesA[i]);
        verticeMinA = glm::min(verticeMinA, verticesA[i]);
        verticeMaxB = glm::max(verticeMaxB, verticesB[i]);
        verticeMinB = glm::min(verticeMinB, verticesB[i]);
    }

    // Calcula o centro de cada OBB
    glm::vec3 centerA = (verticeMaxA + verticeMinA) * 0.5f; 
    glm::vec3 centerB = (verticeMaxB + verticeMinB) * 0.5f;
    

    // Calcula os eixos principais de cada OBB
    glm::vec3 axisA[3] = {
        glm::normalize(verticesA[1] - verticesA[0]), // Eixo X de A
        glm::normalize(verticesA[3] - verticesA[0]), // Eixo Y de A
        glm::normalize(verticesA[4] - verticesA[0])  // Eixo Z de A
    };

    glm::vec3 axisB[3] = {
        glm::normalize(verticesB[1] - verticesB[0]), // Eixo X de B
        glm::normalize(verticesB[3] - verticesB[0]), // Eixo Y de B
        glm::normalize(verticesB[4] - verticesB[0])  // Eixo Z de B
    };

    // Calcula o halfSize de cada OBB
    glm::vec3 halfSizeA = (verticeMaxA - verticeMinA) * 0.5f; 
    glm::vec3 halfSizeB = (verticeMaxB - verticeMinB) * 0.5f;

    // Vetor entre os centros dos OBBs
    glm::vec3 translation = centerB - centerA;

    // Matriz de produtos escalares entre os eixos dos dois OBBs
    float rotation[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            rotation[i][j] = glm::dot(axisA[i], axisB[j]);
        }
    }

    // Testa os eixos principais dos dois OBBs
    for (int i = 0; i < 3; i++) {
        float projectionA = halfSizeA[i];
        float projectionB =
            glm::abs(rotation[i][0]) * halfSizeB.x +
            glm::abs(rotation[i][1]) * halfSizeB.y +
            glm::abs(rotation[i][2]) * halfSizeB.z;

        if (glm::abs(glm::dot(translation, axisA[i])) > projectionA + projectionB)
            return false; // Separação encontrada
    }

    for (int j = 0; j < 3; j++) {
        float projectionA =
            glm::abs(rotation[0][j]) * halfSizeA.x +
            glm::abs(rotation[1][j]) * halfSizeA.y +
            glm::abs(rotation[2][j]) * halfSizeA.z;

        float projectionB = halfSizeB[j];

        if (glm::abs(glm::dot(translation, axisB[j])) > projectionA + projectionB)
            return false; // Separação encontrada
    }

    // Testa os nove eixos cruzados (produtos vetoriais dos eixos)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glm::vec3 crossAxis = glm::cross(axisA[i], axisB[j]);

            float projectionA =
                glm::abs(glm::dot(crossAxis, axisA[0])) * halfSizeA.x +
                glm::abs(glm::dot(crossAxis, axisA[1])) * halfSizeA.y +
                glm::abs(glm::dot(crossAxis, axisA[2])) * halfSizeA.z;

            float projectionB =
                glm::abs(glm::dot(crossAxis, axisB[0])) * halfSizeB.x +
                glm::abs(glm::dot(crossAxis, axisB[1])) * halfSizeB.y +
                glm::abs(glm::dot(crossAxis, axisB[2])) * halfSizeB.z;

            if (glm::abs(glm::dot(translation, crossAxis)) > projectionA + projectionB)
                return false; // Separação encontrada
        }
    }

    // Se nenhuma separação foi encontrada, os OBBs colidem
    return true;
}

bool Collisions::SpheresTest(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2) {
    float distance = glm::distance(center1, center2);
    return distance < radius1 + radius2;
}
