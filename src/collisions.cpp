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

    std::cout<<"testando colisao entre OBBs"<<std::endl;
    std::cout<<"Center1: "<<center1.x<<" "<<center1.y<<" "<<center1.z<<std::endl;
    std::cout<<"Center2: "<<center2.x<<" "<<center2.y<<" "<<center2.z<<std::endl;
    std::cout<<"HalfSize1: "<<halfSize1.x<<" "<<halfSize1.y<<" "<<halfSize1.z<<std::endl;
    std::cout<<"HalfSize2: "<<halfSize2.x<<" "<<halfSize2.y<<" "<<halfSize2.z<<std::endl;
    std::cout<<"Axis1: "<<std::endl;
    for(int i=0;i<3;i++){
      std::cout<<axis1[i].x<<" "<<axis1[i].y<<" "<<axis1[i].z<<std::endl;
    }
    std::cout<<"Axis2: "<<std::endl;
    for(int i=0;i<3;i++){
      std::cout<<axis2[i].x<<" "<<axis2[i].y<<" "<<axis2[i].z<<std::endl;
    }
    // Vetor entre os centros dos dois OBBs
    glm::vec3 d = center2 - center1;

    // Matriz de transformações entre os sistemas de coordenadas dos OBBs
    float R[3][3];
    float AbsR[3][3]; // Matriz com valores absolutos de R

    // Calcular R e AbsR
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            R[i][j] = glm::dot(axis1[i], axis2[j]);
            AbsR[i][j] = std::abs(R[i][j]) + 1e-6f; // Adiciona um pequeno valor para evitar problemas numéricos
        }
    }

    // Testar os eixos locais do primeiro OBB (L = A0, A1, A2)
    for (int i = 0; i < 3; ++i) {
        float ra = halfSize1[i];
        float rb = halfSize2.x * AbsR[i][0] + halfSize2.y * AbsR[i][1] + halfSize2.z * AbsR[i][2];
        if (std::abs(glm::dot(d, axis1[i])) > ra + rb) return false;
    }

    // Testar os eixos locais do segundo OBB (L = B0, B1, B2)
    for (int i = 0; i < 3; ++i) {
        float ra = halfSize1.x * AbsR[0][i] + halfSize1.y * AbsR[1][i] + halfSize1.z * AbsR[2][i];
        float rb = halfSize2[i];
        if (std::abs(glm::dot(d, axis2[i])) > ra + rb) return false;
    }

    // Testar os eixos cruzados (L = A0 x B0, A0 x B1, ..., A2 x B2)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            float ra = halfSize1[(i + 1) % 3] * AbsR[(i + 2) % 3][j] + halfSize1[(i + 2) % 3] * AbsR[(i + 1) % 3][j];
            float rb = halfSize2[(j + 1) % 3] * AbsR[i][(j + 2) % 3] + halfSize2[(j + 2) % 3] * AbsR[i][(j + 1) % 3];
            if (std::abs(d[(i + 2) % 3] * R[(i + 1) % 3][j] - d[(i + 1) % 3] * R[(i + 2) % 3][j]) > ra + rb)
                return false;
        }
    }

    // Nenhum eixo separador foi encontrado: os OBBs estão colidindo
    return true;
}

bool Collisions::SpheresTest(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2) {
    float distance = glm::distance(center1, center2);
    return distance < radius1 + radius2;
}
