#include<bits/stdc++.h>
#include"sonolus.h"
using namespace std;
string readFile(string path) {
    ifstream fin(path.c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) return "";
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    string buffer;
    for (int i = 0; i < len; i++) buffer.push_back(ch[i]);
    delete[] ch;
    return buffer;
}

const string Scope = "hwpl";
const string dist = "./dist";

enum NoteType {
	CommonNote = 1000,
	HighlightedNote = 2000,
	CommonFlick = 1001,
	HighlightedFlick = 2001,
	CommonHoldStart = 1002,
	HighlightedHoldStart = 2002,
	CommonHoldEnd = 1003,
	HighlighedHoldEnd = 2003,
	FlickHoldEnd = 1004,
	HighlightedFlickHoldEnd = 2004,
	Drag = 1005
};

enum SpriteType {
	Sprite_NormalNote,
	Sprite_HighlightedNote,
	Sprite_NormalFlick,
	Sprite_HighlightedFlick,
	Sprite_NormalHold,
	Sprite_HighlightedHold,
	Sprite_HoldLine,
	Sprite_JudgeNote,
	Sprite_Stage,
	Sprite_JudgeLine
};

struct Note {
	NoteType type;
	double clickTime;
	int lastNote;
	bool isFake = false;
	bool isMulti = false;
	int Line;
};

Json::Value fromHanipure(Json::Value chart) {

}

int main() {
    EngineData engineData;
    EngineConfiguration engineConfiguration;

    // Write your code here
	engineConfiguration = EngineConfiguration(
		{
			EngineConfigurationToggleOption("Autoplay", 0, true, Scope),
			EngineConfigurationToggleOption("Strict Judge", 0, true, Scope),
			EngineConfigurationSliderOption("Notes Speed", 5, 1, 50, 0.1, false, Scope),
			EngineConfigurationSliderOption("Notes Size", 1, 0.8, 1.2, 0.05, false, Scope, "#PERCENTAGE"),
			EngineConfigurationToggleOption("Sync Line", 1, false, Scope)
		}, EngineConfigurationUI(
			arcade, life,
			EngineConfigurationVisibility(1, 1),
			EngineConfigurationVisibility(0, 0),
			EngineConfigurationVisibility(1, 1),
			EngineConfigurationVisibility(1, 1),
			EngineConfigurationVisibility(1, 1),
			EngineConfigurationAnimation({0, 0, 0, Linear}, {0, 0, 0, Linear}),
			EngineConfigurationAnimation({0.2, 1, 0.2, InSine}, {1, 1, 0, Linear}),
			none, both, 20, Scope
		)
	);

	engineData = EngineData({
			{"Hanipure Stage", Sprite_Stage},
			{"Hanipure Judge Line", Sprite_JudgeLine},
			{"Hanipure Normal Note", Sprite_NormalNote},
			{"Hanipure Highlighted Note", Sprite_HighlightedNote},
			{"Hanipure Normal Flick", Sprite_NormalFlick},
			{"Hanipure Highlighted Flick", Sprite_HighlightedFlick},
			{"Hanipure Normal Hold", Sprite_NormalHold},
			{"Hanipure Highlighted Hold", Sprite_HighlightedHold},
			{"Hanipure Hold Note", Sprite_HoldLine},
			{"Hanipure Judge Note", Sprite_JudgeNote}
		}, {

		}, {

		}, {

		}, {}, {}
	);

    // Compile the engine
    buffer resData, resConfiguration;
    build(engineConfiguration, engineData, resConfiguration, resData);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < resConfiguration.size(); i++) fout << resConfiguration.v[i];
    fout.close(); fout.open((dist + "/EngineData"));
    for (int i = 0; i < resData.size(); i++) fout << resData.v[i];
}
