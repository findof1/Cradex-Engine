#include <model.hpp>
#include <texture_util.hpp>
#include <shader.hpp>
#include <stb_image.h>
#include <BinaryUtils.hpp>

Model::Model(const std::string &path, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale), path(path)
{
  stbi_set_flip_vertically_on_load(true);
  loadModel(path);
}

void Model::draw(Shader &shader)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);

  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  model = glm::scale(model, scale);

  shader.use();
  shader.setMat4("model", model);
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader);
}

json Model::serialize()
{
  json serializedObject;

  json serializedPosition;
  serializedPosition["x"] = position.x;
  serializedPosition["y"] = position.y;
  serializedPosition["z"] = position.z;

  serializedObject["position"] = serializedPosition;

  json serializedRotation;
  serializedRotation["x"] = rotation.x;
  serializedRotation["y"] = rotation.y;
  serializedRotation["z"] = rotation.z;

  serializedObject["rotation"] = serializedRotation;

  json serializedScale;
  serializedScale["x"] = scale.x;
  serializedScale["y"] = scale.y;
  serializedScale["z"] = scale.z;

  serializedObject["scale"] = serializedScale;

  serializedObject["path"] = path;

  return serializedObject;
}

void Model::binSerialize(std::ofstream &ofs)
{
  binSerializeVector3(ofs, position);
  binSerializeVector3(ofs, rotation);
  binSerializeVector3(ofs, scale);
  binSerializeString(ofs, path);
}

void Model::loadModel(std::string path)
{
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
                                                   aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{

  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;

    vertex.Position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;

    vertex.Normal = vector;

    if (mesh->mTextureCoords[0])
    {

      glm::vec2 vec;

      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;

      vertex.TexCoords = vec;
    }
    else
    {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;

    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }

    if (!skip)
    {
      Texture texture;

      texture.id = GetTextureFromFile(str.C_Str(), directory);
      texture.type = typeName;
      texture.path = str.C_Str();

      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }

  return textures;
}