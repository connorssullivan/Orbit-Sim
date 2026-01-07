#pragma monce

#include "shader.h"
#include "mesh.h"

#include <vector>
#include <string>

#include <assimp/scene.h>      
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>

namespace ModelPaths
{
    inline constexpr const char* SUN_MODEL = "../recources/solarSystem/sun/sun.gltf";

    inline constexpr const char* MERCURY_MODEL = "../recources/solarSystem/mercury/mercury.gltf";

    inline constexpr const char* VENUS_MODEL = "../recources/solarSystem/venus/venus.gltf";

    inline constexpr const char* EARTH_MODEL = "../recources/solarSystem/earth/earthgltf.gltf";
    inline constexpr const char* EARTH_MOON_MODEL = "../recources/solarSystem/earth/moon/earth_moon.gltf";

    inline constexpr const char* MARS_MODEL = "../recources/solarSystem/mars/mars.gltf";

    inline constexpr const char* JUIPITER_MODEL = "../recources/solarSystem/jupiter/juipiter.gltf";

    inline constexpr const char* SATURN_MODEL = "../recources/solarSystem/saturn/saturn.gltf";
    inline constexpr const char* SATURN_RING_MODEL = "../recources/solarSystem/saturn/saturnring.gltf";

    inline constexpr const char* URANUS_MODEL = "../recources/solarSystem/uranus/uranus.gltf";

    inline constexpr const char* NEPTUNE_MODEL = "../recources/solarSystem/neptune/neptune.gltf";

    inline constexpr const char* PLUTO_MODEL = "../";
    

};

class Model 
{
    public:
        Model(const char *path)
        {
            loadModel(path);
        }

        void Draw(Shader &shader)
        {
            for (unsigned int i {}; i < m_meshes.size(); i++)
                m_meshes[i].Draw(shader);
        }	

        unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma=false);
    private:
        // model data
        std::vector<Mesh> m_meshes;
        std::string m_directory;
        std::vector<Texture> m_textures_loaded; 

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);       
};