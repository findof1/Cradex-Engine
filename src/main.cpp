#include <Application.hpp>

int main()
{

  Application app;
  app.run();
  app.serialize();
  return 0;
  /*
Application app(Build);
app.runGame();
return 0;
*/
}