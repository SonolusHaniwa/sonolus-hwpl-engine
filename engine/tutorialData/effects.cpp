using namespace std;

class Clips {
    public:

	int Perfect = 0;
	int Great = 1;
	int Good = 2;
	int Stage = 3;
	int Flick = 4;
	int Hold = 5;
}Clips;

auto effects = defineEffects<class Clips>({
    {EffectClipName.Perfect, Clips.Perfect},
    {EffectClipName.Great, Clips.Great},
    {EffectClipName.Good, Clips.Good},
    {EffectClipName.Stage, Clips.Stage},
    {EffectClipName.PerfectAlternative, Clips.Flick},
    {EffectClipName.Hold, Clips.Hold}
});