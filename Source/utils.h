#ifndef COMS30115_UTILITY_H
#define COMS30115_UTILITY_H

#include "settings.h"
#include "scene.h"

#include <vector>

namespace scg
{

Settings loadSettings();

void loadTransferFunction(Settings &settings);

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
Scene loadTestModel();

}

#endif //COMS30115_UTILITY_H
