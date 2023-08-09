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
    {"Hanipure Perfect", Clips.Perfect},
    {"Hanipure Great", Clips.Great},
    {"Hanipure Good", Clips.Good},
    {"Hanipure Stage", Clips.Stage},
    {"Hanipure Flick", Clips.Flick},
    {"Hanipure Hold", Clips.Hold}
});