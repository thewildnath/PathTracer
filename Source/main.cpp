#include "vector_type.h"

//#include <iostream>

/* ----------------------------------------------------------------------------*/


int main(int argc, char *argv[])
{
    scg::Vec3f a2(1, 2, 3);
    scg::Vec3f b2(4, 5, 6);

    float sum = 0;
    scg::Vec3f s2(0, 0, 0);

    for (int i = 1; i <= 30000000; ++i)
    {
        //sum += (a2 * 0.5f).dot(b2);
        s2 += a2 + b2;
        a2.x = a2.y = a2.z = i;
        b2.x = b2.y = b2.z = i;
    }

    //std::cout << "Sum: " << sum << " " << " " << s2 << std::endl;

    return 0;
}
