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

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".

in vec4 position_world;
in vec4 normal;

in vec2 texcoords;

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

    if ( object_id == SPHERE )
    {
        // PREENCHA AQUI
        // Propriedades espectrais da esfera
        Kd = vec3(0.8,0.4,0.08);
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.4,0.2,0.04);
        q = 1.0;
    }
    else if ( object_id == BUNNY )
    {
        // PREENCHA AQUI
        // Propriedades espectrais do coelho
        Kd = vec3(0.08,0.4,0.8);
        Ks = vec3(0.8,0.8,0.8);
        Ka = vec3(0.04,0.2,0.4);
        q = 32.0;
    }
    else if ( object_id == PLANE )
    {
        // PREENCHA AQUI
        // Propriedades espectrais do plano
        // Kd = vec3(0.2,0.2,0.2);
        Kd = objectColor;
        Ks = vec3(0.3,0.3,0.3);
        Ka = vec3(0.0,0.0,0.0);
        q = 20.0;
    }
    else if (object_id == FAUSTAO_HAIR)
    {
        //Kd = vec3(0.08,0.4,0.8);
        //Ks = vec3(0.8,0.8,0.8);
        //Ka = vec3(0.04,0.2,0.4);
        //q = 32.0;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = clamp((position_world.x - bbox_min.x) / (bbox_max.x - bbox_min.x), 0.0, 1.0);
        V = clamp((position_world.y - bbox_min.y) / (bbox_max.y - bbox_min.y), 0.0, 1.0);

        Kd = texture(TextureImage1, vec2(U,V)).rgb; 
        //Kd = texture(TextureImage1, texcoords).rgb;
    }
    else if (object_id == FAUSTAO_FACE)
    {
        //Kd = vec3(0.6, 0.2, 0.6); 
        //Ks = vec3(0.8, 0.4, 0.8); 
        //Ka = vec3(0.2, 0.0, 0.2); 
        //q = 32.0;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = clamp((position_world.x - bbox_min.x) / (bbox_max.x - bbox_min.x), 0.0, 1.0);
        V = clamp((position_world.y - bbox_min.y) / (bbox_max.y - bbox_min.y), 0.0, 1.0);

        Kd = texture(TextureImage0, vec2(U,V)).rgb; 

        //Kd = texture(TextureImage1, texcoords).rgb;
    }
    else if (object_id == FAUSTAO_CLOTHES)
    {
        //Kd = vec3(0.8, 0.8, 0.2); 
        //Ks = vec3(1.0, 1.0, 0.4); 
        //Ka = vec3(0.2, 0.2, 0.0); 
        //q = 32.0;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        U = clamp((position_world.x - bbox_min.x) / (bbox_max.x - bbox_min.x), 0.0, 1.0);
        V = clamp((position_world.y - bbox_min.y) / (bbox_max.y - bbox_min.y), 0.0, 1.0);
        
        Kd = texture(TextureImage2, vec2(U,V)).rgb; 

        //Kd = texture(TextureImage2, texcoords).rgb;
    }
    else // Objeto desconhecido = preto //mudei para a mesma coisa da esfera
    {
        Kd = objectColor;
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.4,0.2,0.04);
        q = 1.0;
        // Kd = vec3(0.0,0.0,0.0);
        // Ks = vec3(0.0,0.0,0.0);
        // Ka = vec3(0.0,0.0,0.0);
        // q = 1.0;
    }

    // Espectro da fonte de iluminação
    //vec3 I = vec3(1.0,1.0,1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    //vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente


    // Termo ambiente
    //vec3 ambient_term = Ka * Ia; // PREENCHA AQUI o termo ambiente

    // Termo especular utilizando o modelo de iluminação de Phong
    //vec3 phong_specular_term  = Ks * I * pow(max(0, dot(r,v)), q); // PREENCH AQUI o termo especular de Phong
    
    Ks = vec3(0.8,0.8,0.8);
    q = 32.0;
    vec3 lambert_diffuse_term = Kd * (max(0.1, dot(n, l)) + 0.01);
    color.rgb = lambert_diffuse_term;

    // DEBUG
    //color.rgb = vec3(U, V, 0.0); // Visualize U and V
    //color.rgb = texture(TextureImage2, vec2(U, V)).rgb;
    //color.rgb = vec3((bbox_max.x - bbox_min.x), (bbox_max.y - bbox_min.y), (bbox_max.z - bbox_min.z));
    //color.rgb = texture(TextureImage1, texcoords).rgb;
    //float checker = mod(floor(U * 10.0) + floor(V * 10.0), 2.0);
    //if (checker < 1.0) {
    //    color.rgb = vec3(0.0, 0.0, 0.0); // Black
    //} else {
    //    color.rgb = vec3(1.0, 0.0, 0.0); // Red
    //}



    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
    
    //color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
} 

