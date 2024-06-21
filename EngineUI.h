#ifndef ENGINE_UI_H
#define ENGINE_UI_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer.h"
#include "Application.h"

class UI
{
public:
  UI(Renderer *renderer) : renderer(renderer)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(renderer->window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    textureWidth = renderer->ScreenW / 2;
    textureHeight = renderer->ScreenH / 2;
    texture = CreateOpenGLTexture(textureWidth, textureHeight);
    fbo = CreateFramebuffer(texture, textureWidth, textureHeight);
    aspectRatio = static_cast<float>(textureWidth) / static_cast<float>(textureHeight);

    glfwSetDropCallback(renderer->window, drop_callback);
  }

  void draw()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    displayEditor();
    displayHierarchy();
    displayPropertiesEditor();
    displayTextureManager();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

private:
  Renderer *renderer;

  int textureWidth;
  int textureHeight;
  GLuint texture;
  GLuint fbo;
  float aspectRatio;

  const char *editing = "";

  static int numbersOnlyTextCallback(ImGuiInputTextCallbackData *data)
  {

    if (data->EventChar < '0' || data->EventChar > '9')
    {
      if (data->EventChar != '-' && data->EventChar != '.' && data->EventChar != '\b' && data->EventChar != '\t' && data->EventChar != '\r' && data->EventChar != '\n')
        return 1;
    }

    return 0;
  }

  static void drop_callback(GLFWwindow *window, int count, const char **paths)
  {
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    ImVec2 tex_drop_pos = ImVec2(renderer->ScreenW - (renderer->ScreenW / 2 * 1.5), renderer->ScreenH / 2 * 1.5);
    ImVec2 tex_drop_size = ImVec2(renderer->ScreenW / 2 * 1.5, renderer->ScreenH - (renderer->ScreenH / 2 * 1.5));
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    if (mouse_x >= tex_drop_pos.x && mouse_x <= (tex_drop_pos.x + tex_drop_size.x) &&
        mouse_y >= tex_drop_pos.y && mouse_y <= (tex_drop_pos.y + tex_drop_size.y))
    {
      for (int i = 0; i < count; i++)
      {
        renderer->dropped_files.push_back(paths[i]);

        const std::string destination_folder = "Textures";

        for (int i = 0; i < count; i++)
        {
          std::string new_path = renderer->copy_file_to_folder(paths[i], destination_folder);
          if (!new_path.empty())
          {
            renderer->dropped_files.push_back(new_path);
          }
          else
          {
            std::cerr << "Failed to copy file: " << paths[i] << std::endl;
          }
        }
      }
    }
  }

  void displayHierarchy()
  {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(renderer->ScreenW - (renderer->ScreenW / 2 * 1.5), renderer->ScreenH / 2), ImGuiCond_Always);
    ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Columns(1);
    ImGui::Text("Game Objects");
    ImGui::SameLine();
    if (ImGui::Button("Add Cube"))
    {
      renderer->addGameObjectDefault(0, "Textures/diffuse.jpg", "Textures/specular.jpg");
    }
    for (auto &object : renderer->GameObjects)
    {
      if (ImGui::Button(object.first.c_str()))
      {
        editing = object.first.c_str();
      }
    }
    ImGui::Text("Point Lights");
    ImGui::SameLine();
    if (ImGui::Button("Add Point Light"))
    {
      renderer->addPointLightDefault(glm::vec3(0.0f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f, 1.0f);
    }
    for (auto &object : renderer->PointLights)
    {
      if (ImGui::Button(object.first.c_str()))
      {
        editing = object.first.c_str();
      }
    }
    ImGui::Text("Spot Lights");
    ImGui::SameLine();
    if (ImGui::Button("Add Spot Light"))
    {
      renderer->addSpotLightDefault(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f, 12.5f, 17.5f);
    }
    for (auto &object : renderer->SpotLights)
    {
      if (ImGui::Button(object.first.c_str()))
      {
        editing = object.first.c_str();
      }
    }
    ImGui::Text("Directional Light");
    if (ImGui::Button("Directional Light"))
    {
      editing = "Directional Light";
    }
    ImGui::Text("Models");
    for (auto &object : renderer->Models)
    {
      if (ImGui::Button(object.first.c_str()))
      {
        editing = object.first.c_str();
      }
    }
    ImGui::Columns(1);
    ImGui::End();
  }

  void displayTextureManager()
  {

    ImGui::SetNextWindowPos(ImVec2(renderer->ScreenW - (renderer->ScreenW / 2 * 1.5), renderer->ScreenH / 2 * 1.5), ImGuiCond_Always);

    ImGui::SetNextWindowSize(ImVec2(renderer->ScreenW / 2 * 1.5, renderer->ScreenH - (renderer->ScreenH / 2 * 1.5)), ImGuiCond_Always);

    ImGui::Begin("Texture Manager", NULL, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::End();
  }

  void displayPropertiesEditor()
  {
    ImGui::SetNextWindowPos(ImVec2(0, renderer->ScreenH / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(renderer->ScreenW - (renderer->ScreenW / 2 * 1.5), renderer->ScreenH / 2), ImGuiCond_Always);
    ImGui::Begin("Properties Editor", NULL, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (editing == "")
    {

      ImGui::End();
      return;
    }

    if (renderer->GameObjects.find(editing) != renderer->GameObjects.end())
    {

      displayEditingGameObject();
      ImGui::End();
      return;
    }

    if (renderer->Models.find(editing) != renderer->Models.end())
    {

      displayEditingModel();
      ImGui::End();
      return;
    }

    if (renderer->PointLights.find(editing) != renderer->PointLights.end())
    {

      displayEditingPointLight();
      ImGui::End();
      return;
    }

    if (renderer->SpotLights.find(editing) != renderer->SpotLights.end())
    {

      displayEditingSpotLight();
      ImGui::End();
      return;
    }

    ImGui::End();
  }

  void displayEditingGameObject()
  {
    ImGui::Columns(1);
    ImGui::Text(editing);
    ImGui::Text("Position");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XPos");
    ImGui::SetNextItemWidth(30);
    std::string valueStr = std::to_string(renderer->GameObjects.at(editing)->position.x);
    static char buf[32];
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->position.x = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->position.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->position.y = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->position.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->position.z = atof(buf);
    }
    ImGui::PopID();

    ImGui::Text("Scale");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XScale");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->scale.x);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->scale.x = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YScale");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->scale.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->scale.y = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZScale");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->scale.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->scale.z = atof(buf);
    }
    ImGui::PopID();

    ImGui::Text("Rotation");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XRotation");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->rotation.x);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->rotation.x = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YRotation");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->rotation.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->rotation.y = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZRotation");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->GameObjects.at(editing)->rotation.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->GameObjects.at(editing)->rotation.z = atof(buf);
    }
    ImGui::PopID();
    ImGui::Columns(1);
  }

  void displayEditingPointLight()
  {
    ImGui::Columns(1);
    ImGui::Text(editing);
    ImGui::Text("Position");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XPos");
    ImGui::SetNextItemWidth(30);

    std::string valueStr = std::to_string(renderer->PointLights.at(editing).position.x);

    static char buf[32];
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {

      renderer->setPointLightPosition(editing, glm::vec3(atof(buf), renderer->PointLights.at(editing).position.y, renderer->PointLights.at(editing).position.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).position.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightPosition(editing, glm::vec3(renderer->PointLights.at(editing).position.x, atof(buf), renderer->PointLights.at(editing).position.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).position.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightPosition(editing, glm::vec3(renderer->PointLights.at(editing).position.x, renderer->PointLights.at(editing).position.y, atof(buf)));
    }
    ImGui::PopID();

    ImGui::Text("Ambient");

    ImGui::Text("R:");
    ImGui::SameLine();
    ImGui::PushID("RAmbient");
    ImGui::SetNextItemWidth(30);

    valueStr = std::to_string(renderer->PointLights.at(editing).ambient.x * 255);

    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##R", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {

      renderer->setPointLightAmbient(editing, glm::vec3(atof(buf) / 255, renderer->PointLights.at(editing).ambient.y, renderer->PointLights.at(editing).ambient.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("G:");
    ImGui::SameLine();
    ImGui::PushID("GAmbient");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).ambient.y * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##G", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightAmbient(editing, glm::vec3(renderer->PointLights.at(editing).ambient.x, atof(buf) / 255, renderer->PointLights.at(editing).ambient.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("B:");
    ImGui::SameLine();
    ImGui::PushID("BAmbient");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).ambient.z * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##B", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightAmbient(editing, glm::vec3(renderer->PointLights.at(editing).ambient.x, renderer->PointLights.at(editing).ambient.y, atof(buf) / 255));
    }
    ImGui::PopID();

    ImGui::Text("Diffuse");

    ImGui::Text("R:");
    ImGui::SameLine();
    ImGui::PushID("RDiffuse");
    ImGui::SetNextItemWidth(30);

    valueStr = std::to_string(renderer->PointLights.at(editing).diffuse.x * 255);

    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##R", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {

      renderer->setPointLightDiffuse(editing, glm::vec3(atof(buf) / 255, renderer->PointLights.at(editing).diffuse.y, renderer->PointLights.at(editing).diffuse.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("G:");
    ImGui::SameLine();
    ImGui::PushID("GDiffuse");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).diffuse.y * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##G", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightDiffuse(editing, glm::vec3(renderer->PointLights.at(editing).diffuse.x, atof(buf) / 255, renderer->PointLights.at(editing).diffuse.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("B:");
    ImGui::SameLine();
    ImGui::PushID("BDiffuse");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).diffuse.z * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##B", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightDiffuse(editing, glm::vec3(renderer->PointLights.at(editing).diffuse.x, renderer->PointLights.at(editing).diffuse.y, atof(buf) / 255));
    }
    ImGui::PopID();

    ImGui::Text("Specular");

    ImGui::Text("R:");
    ImGui::SameLine();
    ImGui::PushID("RSpecular");
    ImGui::SetNextItemWidth(30);

    valueStr = std::to_string(renderer->PointLights.at(editing).specular.x * 255);

    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##R", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {

      renderer->setPointLightSpecular(editing, glm::vec3(atof(buf) / 255, renderer->PointLights.at(editing).specular.y, renderer->PointLights.at(editing).specular.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("G:");
    ImGui::SameLine();
    ImGui::PushID("GSpecular");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).specular.y * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##G", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightSpecular(editing, glm::vec3(renderer->PointLights.at(editing).specular.x, atof(buf) / 255, renderer->PointLights.at(editing).specular.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("B:");
    ImGui::SameLine();
    ImGui::PushID("BSpecular");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->PointLights.at(editing).specular.z * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##B", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setPointLightSpecular(editing, glm::vec3(renderer->PointLights.at(editing).specular.x, renderer->PointLights.at(editing).specular.y, atof(buf) / 255));
    }
    ImGui::PopID();

    ImGui::Columns(1);
  }

  void displayEditingSpotLight()
  {
    ImGui::Columns(1);
    ImGui::Text(editing);
    ImGui::Text("Position");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XPos");
    ImGui::SetNextItemWidth(30);

    std::string valueStr = std::to_string(renderer->SpotLights.at(editing).position.x);

    static char buf[32];
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {

      renderer->setSpotLightPosition(editing, glm::vec3(atof(buf), renderer->SpotLights.at(editing).position.y, renderer->SpotLights.at(editing).position.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->SpotLights.at(editing).position.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setSpotLightPosition(editing, glm::vec3(renderer->SpotLights.at(editing).position.x, atof(buf), renderer->SpotLights.at(editing).position.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->SpotLights.at(editing).position.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setSpotLightPosition(editing, glm::vec3(renderer->SpotLights.at(editing).position.x, renderer->SpotLights.at(editing).position.y, atof(buf)));
    }
    ImGui::PopID();

    ImGui::Text("Diffuse");

    ImGui::Text("R:");
    ImGui::SameLine();
    ImGui::PushID("RDiffuse");
    ImGui::SetNextItemWidth(30);

    valueStr = std::to_string(renderer->SpotLights.at(editing).diffuse.x * 255);

    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##R", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {

      renderer->setSpotLightDiffuse(editing, glm::vec3(atof(buf) / 255, renderer->SpotLights.at(editing).diffuse.y, renderer->SpotLights.at(editing).diffuse.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("G:");
    ImGui::SameLine();
    ImGui::PushID("GDiffuse");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->SpotLights.at(editing).diffuse.y * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##G", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setSpotLightDiffuse(editing, glm::vec3(renderer->SpotLights.at(editing).diffuse.x, atof(buf) / 255, renderer->SpotLights.at(editing).diffuse.z));
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("B:");
    ImGui::SameLine();
    ImGui::PushID("BDiffuse");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->SpotLights.at(editing).diffuse.z * 255);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##B", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->setSpotLightDiffuse(editing, glm::vec3(renderer->SpotLights.at(editing).diffuse.x, renderer->SpotLights.at(editing).diffuse.y, atof(buf) / 255));
    }
    ImGui::PopID();

    ImGui::Columns(1);
  }

  void displayEditingModel()
  {
    ImGui::Columns(1);
    ImGui::Text(editing);
    ImGui::Text("Position");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XPos");
    ImGui::SetNextItemWidth(30);
    std::string valueStr = std::to_string(renderer->Models.at(editing)->position.x);
    static char buf[32];
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->position.x = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->position.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->position.y = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZPos");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->position.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->position.z = atof(buf);
    }
    ImGui::PopID();

    ImGui::Text("Scale");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XScale");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->scale.x);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->scale.x = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YScale");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->scale.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->scale.y = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZScale");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->scale.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->scale.z = atof(buf);
    }
    ImGui::PopID();

    ImGui::Text("Rotation");

    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::PushID("XRotation");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->rotation.x);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##X", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->rotation.x = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::PushID("YRotation");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->rotation.y);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Y", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->rotation.y = atof(buf);
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::PushID("ZRotation");
    ImGui::SetNextItemWidth(30);
    valueStr = std::to_string(renderer->Models.at(editing)->rotation.z);
    resetBuffer(buf);
    strcpy(buf, valueStr.c_str());

    if (ImGui::InputText("##Z", buf, sizeof(buf), ImGuiInputTextFlags_CharsDecimal, numbersOnlyTextCallback))
    {
      renderer->Models.at(editing)->rotation.z = atof(buf);
    }
    ImGui::PopID();
    ImGui::Columns(1);
  }

  void displayEditor()
  {
    ImGui::SetNextWindowPos(ImVec2(renderer->ScreenW - (renderer->ScreenW / 2 * 1.5), 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(renderer->ScreenW / 2 * 1.5, renderer->ScreenH / 2 * 1.5), ImGuiCond_Always);

    ImGui::Begin("Editor", NULL, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImVec2 availSize = ImGui::GetContentRegionAvail();

    float imageWidth = availSize.x;
    float imageHeight = imageWidth / aspectRatio;

    if (imageHeight > availSize.y)
    {
      imageHeight = availSize.y;
      imageWidth = imageHeight * aspectRatio;
    }

    RenderToTexture(fbo, textureWidth, textureHeight);

    ImGui::Image(reinterpret_cast<ImTextureID>(texture), ImVec2(imageWidth, imageHeight), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
  }

  GLuint CreateOpenGLTexture(int width, int height)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
  }

  void resetBuffer(char *buffer)
  {
    for (int i = 0; i < sizeof(buffer); i++)
    {
      buffer[i] = '\0';
    }
  }

  GLuint CreateFramebuffer(GLuint texture, int width, int height)
  {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return fbo;
  }

  void RenderToTexture(GLuint fbo, int width, int height)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
};

#endif