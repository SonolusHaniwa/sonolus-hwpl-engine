using namespace std;

class Texts {
    public:

    int tap = 0;
    int flick = 1;
    int holdStart = 2;
    int holdEnd = 3;
    int holdFlickEnd = 4;
    int holdLine = 5;
}Texts;

auto texts = defineInstructionTexts<class Texts>({
    {InstructionText.Tap, Texts.tap},
    {InstructionText.TapAndFlick, Texts.flick},
    {InstructionText.TapAndHold, Texts.holdStart},
    {InstructionText.Release, Texts.holdEnd},
    {InstructionText.Flick, Texts.holdFlickEnd},
    {InstructionText.Hold, Texts.holdLine}
});