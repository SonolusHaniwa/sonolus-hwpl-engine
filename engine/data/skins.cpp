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
    {SkinSpriteName.StageCover, Sprites.Stage},
    {SkinSpriteName.JudgmentLine, Sprites.JudgeLine},
    {SkinSpriteName.NoteHeadGreen, Sprites.NormalNote},
    {SkinSpriteName.NoteTailGreen, Sprites.HighlightedNote},
    {SkinSpriteName.NoteHeadRed, Sprites.NormalFlick},
    {SkinSpriteName.NoteTailRed, Sprites.HighlightedFlick},
    {SkinSpriteName.NoteHeadBlue, Sprites.NormalHold},
    {SkinSpriteName.NoteTailBlue, Sprites.HighlightedHold},
    {SkinSpriteName.NoteConnectionBlue, Sprites.HoldLine},
    {SkinSpriteName.NoteTickBlue, Sprites.JudgeNote},
    {SkinSpriteName.NoteSlot, Sprites.HoldBody},
    {SkinSpriteName.SimultaneousConnectionNeutral, Sprites.SyncLine}
});