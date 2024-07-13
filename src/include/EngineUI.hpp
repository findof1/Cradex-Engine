#ifndef ENGINE_UI_H
#define ENGINE_UI_H

#include <glad/glad.h>

#include <string>
#include <renderer.hpp>

class Application;

class UI
{
public:
  int textureWidth;
  int textureHeight;

  UI(Renderer *renderer, Application *app);

  void draw();

private:
  Application *app;
  Renderer *renderer;
  GLuint texture;
  GLuint fbo;
  float aspectRatio;
  std::string modelPath = "";
  std::string editing = "";
  bool showSettingsWindow = false;

  bool staticWindowsSetting;

  void displaySettings();

  void displayDropdownBar();

  void displayHierarchy();

  void displayFileManager();

  void displayApplyingTexture();

  void displayPropertiesEditor();

  void displayEditingGameObject();

  void displayEditingCamera();

  void displayEditingPointLight();

  void displayEditingSpotLight();

  void displayEditingModel();

  void displayEditor();

  GLuint CreateOpenGLTexture(int width, int height);

  void resetBuffer(char *buffer);

  GLuint CreateFramebuffer(GLuint texture, int width, int height);

  void RenderToTexture(GLuint fbo, int width, int height);
};

#endif