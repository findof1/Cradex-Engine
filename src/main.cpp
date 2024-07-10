#include <Application.hpp>

int main()
{

  Application app;
  app.run();
  app.exportGame();
  app.serialize();
  return 0;
  /*
  Application app(Build);
  app.runGame();
  return 0;
  */
}