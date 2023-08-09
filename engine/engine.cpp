using namespace std;

const string Scope = "hwpl";

#include"configuration/options.cpp"
#include"configuration/ui.cpp"
#ifdef play
using namespace playData;
#include"data/skins.cpp"
#include"data/effects.cpp"
#include"data/particles.cpp"
#include"data/buckets.cpp"
#include"data/constants.cpp"
#include"data/utils.cpp"
#include"data/archetypes/Initialization.cpp"
#include"data/archetypes/InputManager.cpp"
#include"data/archetypes/Stage.cpp"
#include"data/archetypes/NormalNote.cpp"
#include"data/archetypes/NormalFlick.cpp"
#include"data/archetypes/HoldStart.cpp"
#include"data/archetypes/HoldEnd.cpp"
#include"data/archetypes/HoldFlickEnd.cpp"
#include"data/archetypes/HoldLine.cpp"
#include"data/archetypes/SyncLine.cpp"
#elif tutorial
using namespace tutorialData;
#include"tutorialData/skins.cpp"
#include"tutorialData/effects.cpp"
#include"tutorialData/particles.cpp"
#include"tutorialData/constants.cpp"
#include"tutorialData/utils.cpp"
#include"tutorialData/instructionTexts.cpp"
#include"tutorialData/instructionIcons.cpp"
#include"tutorialData/preprocess.cpp"
#include"tutorialData/navigate.cpp"
#include"tutorialData/update.cpp"
#endif