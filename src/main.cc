#include <iostream>
#include <cstdlib>
#include "julia.hh"


int main(int argc, char* argv[])
{
    float x = -0.8;
    float y = 0.156;

    if (argc == 3)
    {
        x = atof(argv[1]);
        y = atof(argv[2]);
    }

    Julia engine("Julia", 1920, 1080, x, y);
    engine.loop();

    return 0;
}
