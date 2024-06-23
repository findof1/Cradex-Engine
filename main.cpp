#include "Application.h"
int main()
{
  Application app(false);
  app.run();
  app.serialize();
  return 0;
}