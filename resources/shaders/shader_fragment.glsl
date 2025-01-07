#version 330 core

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERE 0
#define BUNNY  1
#define PLANE  2

#define FAUSTAO_HAIR 4
#define FAUSTAO_FACE 5
#define FAUSTAO_CLOTHES 6

#define WALL 3
#define FLOOR 7
#define TV 8
#define SCREEN 9

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".

in vec4 position_world;
in vec4 normal;
in vec4 position_model;
in vec2 texcoords;
in vec3 gouraud_shading_term;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;
uniform sampler2D TextureImage8;

uniform int object_id;
uniform vec3 objectColor;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l + 2*n*(dot(n,l)); // PREENCHA AQUI o vetor de reflexão especular ideal

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd; // Refletância difusa
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    vec3 texture_color = vec3(1.5, 0.0, 0.0);

    if (object_id >=10)
    {
        // Spectral properties of the sphere
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.0,0.0,0.0);
        q = 1.0;

        // Scaling factor for texture coordinates
        float scaling = 0.5; // Adjust this value as needed

        // Calculate spherical UV coordinates
        vec3 normalized_position = normalize(position_model.xyz); // Normalize position to sphere
        float U = 0.5 + atan(normalized_position.z, normalized_position.x) / (2.0 * M_PI); // Longitude
        float V = 0.5 - asin(normalized_position.y) / M_PI;                                // Latitude

        // Apply scaling
        vec2 texcoords_sphere = vec2(U, V) * scaling;

        // Sample texture
        texture_color = texture(TextureImage4, texcoords_sphere).rgb;
    }
    else if ( object_id == BUNNY )
    {
        // PREENCHA AQUI
        // Propriedades espectrais do coelho
        Kd = vec3(0.08,0.4,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.04,0.2,0.4);
        q = 32.0;
        //Kd = texture(TextureImage3, texcoords).rgb;
    }
    else if ( object_id == PLANE )
    {
        // PREENCHA AQUI
        // Propriedades espectrais do plano
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.0,0.0,0.0);
        q = 32.0;

        float scaling = 1.5;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;
        float miny = bbox_min.y;
        float maxy = bbox_max.y;
        float minz = bbox_min.z;
        float maxz = bbox_max.z;
  
        //float U = normal.x > 0.0 ? fract(position_model.x * scaling) : fract(position_model.z * scaling);
        //float V = fract(position_model.y * scaling);
        float U = fract(position_model.x * scaling);
        float V = fract(position_model.z * scaling);
        vec2 texcoords_planar = vec2(U, V);

        texture_color = texture(TextureImage3, texcoords_planar).rgb;
    }
    else if (object_id == FLOOR)
    {
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.0,0.0,0.0);
        q = 32.0;

        float scaling = 1.5;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;
        float miny = bbox_min.y;
        float maxy = bbox_max.y;
        float minz = bbox_min.z;
        float maxz = bbox_max.z;
  
        //float U = normal.x > 0.0 ? fract(position_model.x * scaling) : fract(position_model.z * scaling);
        //float V = fract(position_model.y * scaling);
        float U = fract(position_model.x * scaling);
        float V = fract(position_model.z * scaling);
        vec2 texcoords_planar = vec2(U, V);

        texture_color = texture(TextureImage5, texcoords_planar).rgb;
    }
    else if (object_id == WALL)
    {
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.0,0.0,0.0);
        q = 32.0;

        float scaling = 1.5;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;
        float miny = bbox_min.y;
        float maxy = bbox_max.y;
        float minz = bbox_min.z;
        float maxz = bbox_max.z;
  
        //float U = normal.x > 0.0 ? fract(position_model.x * scaling) : fract(position_model.z * scaling);
        //float V = fract(position_model.y * scaling);
        float U = fract(position_model.x * scaling);
        float V = fract(position_model.z * scaling);
        vec2 texcoords_planar = vec2(U, V);

        texture_color = texture(TextureImage6, texcoords_planar).rgb;
    }
    else if (object_id == FAUSTAO_HAIR)
    {
        texture_color = texture(TextureImage1, texcoords).rgb;
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(1.5,1.5,1.5);
        q = 32.0;
    }
    else if (object_id == FAUSTAO_FACE)
    {
        texture_color = texture(TextureImage0, texcoords).rgb;
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(1.5,1.5,1.5);
        q = 32.0;
    }
    else if (object_id == FAUSTAO_CLOTHES)
    {
        texture_color = texture(TextureImage2, texcoords).rgb;
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(1.0,1.0,1.0);
        Ka = vec3(1.5,1.5,1.5);
        q = 32.0;
    }
    else if (object_id == TV)
    {
        texture_color = texture(TextureImage7, texcoords).rgb;
        Kd = vec3(0.8,0.8,0.8);
        Ks = vec3(1.0,1.0,1.0);
        Ka = vec3(1.5,1.5,1.5);
        q = 32.0;
    }
    else if (object_id == SCREEN)
    {
        // Set spectral properties for the screen object
        Kd = vec3(0.8, 0.8, 0.8);
        Ks = vec3(0.8, 0.8, 0.8);
        Ka = vec3(0.0, 0.0, 0.0);
        q = 32.0;

        float scaling = 0.5; // Adjust as needed

        // Generate texture coordinates for the screen
        float U = (position_model.x + 1.0) / 2.0; // Since bbox_min.x = -1, bbox_max.x = 1
        float V = (position_model.z + 1.0) / 2.0; // Since bbox_min.z = -1, bbox_max.z = 1

        U = clamp(U, 0.0, 1.0);
        V = clamp(V, 0.0, 1.0);
        vec2 texcoords_planar = vec2(U, V);

        texture_color = texture(TextureImage8, texcoords_planar).rgb;
    }
    else // Objeto desconhecido = preto 
    {
        Kd = objectColor;
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.4,0.2,0.04);
        q = 1.0;
    }

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0); 

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2); 

    // Termo ambiente
    vec3 ambient_term = Ka * Ia; 

    // Termo difuso utilizando Lambert
    vec3 lambert_diffuse_term = Kd * (max(0.1, dot(n, l)) + 0.01);
    
    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks * I * pow(max(0, dot(r,v)), q); 

    // REQUISITO: Iluminação difusa (Lambert) e Blinn-Phong
    if (object_id == FAUSTAO_HAIR || object_id == FAUSTAO_CLOTHES || object_id == FAUSTAO_FACE)
    {
        color.rgb = (lambert_diffuse_term + ambient_term) * texture_color;
    }

    // REQUISITO: Modelo de Gouraud -> calcular no shader_vertex POR ENQUANTO NORMAL
    else if (object_id == PLANE || object_id == SCREEN)
    {
        color.rgb = (lambert_diffuse_term + ambient_term + gouraud_shading_term) * texture_color;
    }

    // REQUISITO: Modelo de Phong
    else if (object_id == SPHERE || object_id == WALL || object_id == FLOOR || object_id == TV)
    {
        color.rgb = (lambert_diffuse_term + ambient_term + phong_specular_term) * texture_color;
    }

    color.a = 1;
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
    
    if(texture_color == vec3(0.0,0.0,0.0))
    {
        color.rgb = vec3(1.0,0.0,0.0);
    }

    } 

