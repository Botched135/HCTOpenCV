#include "../include/hct.hpp"

using namespace cv;
int main(int argc, char** argv )
{
    const char* filename = argc >=2 ? argv[1] : "data/ultra_m_magnet/";
    int count = atoi(argv[2]);

    return blob(filename);
}