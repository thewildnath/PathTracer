#ifndef COMS30115_UTILITY_H
#define COMS30115_UTILITY_H

#include "scene.h"

#include <vector>

namespace scg
{

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
Scene LoadTestModel1();
Scene LoadTestModel2();
Scene LoadTestModel3();
}

#endif //COMS30115_UTILITY_H
