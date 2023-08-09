using namespace std;

class Sprites {
    public:

	int NormalNote = 0;
	int HighlightedNote = 1;
	int NormalFlick = 2;
	int HighlightedFlick = 3;
	int NormalHold = 4;
	int HighlightedHold = 5;
	int HoldLine = 6;
	int JudgeNote = 7;
	int Stage = 8;
	int JudgeLine = 9;
	int HoldBody = 10;
	int SyncLine = 11;
}Sprites;

auto skins = defineSkins<class Sprites>({
    {"Hanipure Stage", Sprites.Stage},
    {"Hanipure Judge Line", Sprites.JudgeLine},
    {"Hanipure Normal Note", Sprites.NormalNote},
    {"Hanipure Highlighted Note", Sprites.HighlightedNote},
    {"Hanipure Normal Flick", Sprites.NormalFlick},
    {"Hanipure Highlighted Flick", Sprites.HighlightedFlick},
    {"Hanipure Normal Hold", Sprites.NormalHold},
    {"Hanipure Highlighted Hold", Sprites.HighlightedHold},
    {"Hanipure Hold Line", Sprites.HoldLine},
    {"Hanipure Judge Note", Sprites.JudgeNote},
    {"Hanipure Hold Body", Sprites.HoldBody},
    {"Hanipure Sync Line", Sprites.SyncLine}
});