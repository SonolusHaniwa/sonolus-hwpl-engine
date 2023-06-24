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

// <------ 预定义枚举类型 ------>

enum OptionType {
	Option_Autoplay,
	Option_StrictJudge,
	Option_NotesSpeed,
	Option_NotesSize,
	Option_SyncLine,
	Option_LockAspectRadio
};

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
	Sprite_JudgeLine,
	Sprite_HoldBody
};

enum EffectType {
	Effect_Perfect,
	Effect_Great,
	Effect_Good,
	Effect_Stage,
	Effect_Flick,
	Effect_Hold
};

enum ParticleType {

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

// <------ 配置集合 ------>

const double maxSize = 1.2;               // 按键最大大小
const string Scope = "hwpl";
const string dist = "./dist";
const var interfaceGap = 0.05;            // 组件间距
const var targetAspectRadio = 1920.0 / 1080.0;// 目标屏幕宽高比
const var lineNumber = 6;                 // 按键数
const var highWidth = 0.1;                // 高位宽度与低位宽度比
const var defaultAppearTime = 10.0;       // Note 默认出现时间
const var stageMaxPercentage = 0.7;       // 舞台最大占比
const var minSFXDistance = 0.02;          // 最小音效时间
class stage {
	public:

	const var w = If(
		LevelOption.get(Option_LockAspectRadio),
		If(
			targetAspectRadio < screen.aspectRadio,
			screen.h * stageMaxPercentage * targetAspectRadio,
			screen.w * stageMaxPercentage
	    ), screen.w * stageMaxPercentage
	);
	const var h = If(
		LevelOption.get(Option_LockAspectRadio),
		If(
			targetAspectRadio < screen.aspectRadio,
			screen.h * stageMaxPercentage,
			screen.w * stageMaxPercentage / targetAspectRadio
		), screen.h * stageMaxPercentage
	);
}stage;
const var t = If(
	LevelOption.get(Option_LockAspectRadio) && screen.aspectRadio < targetAspectRadio,
	screen.w / targetAspectRadio * 0.5,
	screen.t
);
const var b = t - stage.h;
const var speed = stage.h / defaultAppearTime / 5.0 *LevelOption.get(Option_NotesSpeed);
const var appearTimeLength = stage.h / speed;
class line {
	public:

	int i = 0;
	line(){}
	line(int i):i(i){};
	line operator [] (int i) {
		return line(i);
	}

	const var highPosition = (i - lineNumber / 2.0 - 0.5) * highWidth * stage.w / lineNumber;
	const var lowPosition = (i - lineNumber / 2.0 - 0.5) * stage.w / lineNumber;
	var y(var t) {
		return t / appearTimeLength * stage.h - stage.h / 2;
	}
	var x(var t) {
		return Lerp(lowPosition, highPosition, t / appearTimeLength);
	}
	var width(var t) {
		return Lerp(highWidth / lineNumber, 1.0 / lineNumber, t / appearTimeLength) * stage.w;
	}
}lines;
const double noteSize = 128.0 / 1080.0;
var clickBoxl = stage.w / (-2.0) - stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxr = stage.w / 2.0 + stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;
auto inClickBox = [](Touch touch){
	return clickBoxl <= touch.x && touch.x <= clickBoxr && clickBoxb <= touch.y && touch.y <= clickBoxt;
};

int main() {
    EngineData engineData;
    EngineConfiguration engineConfiguration;

    // <------ 初始化开始 ------>

	// 界面开关参数
	FuncNode menuWidth = 0.15 * ui.menuConfiguration.scale;
	FuncNode menuHeight = 0.15 * ui.menuConfiguration.scale;
	FuncNode menuX = screen.r - interfaceGap;
	FuncNode menuY = screen.t - interfaceGap;
	// 首要指标参数
	FuncNode primaryMetricBarWidth = 0.75 * ui.primaryMetricConfiguration.scale;
	FuncNode primaryMetricBarHeight = 0.15 * ui.primaryMetricConfiguration.scale;
	FuncNode primaryMetricBarX = screen.l + interfaceGap;
	FuncNode primaryMetricBarY = screen.t - interfaceGap;
	FuncNode primaryMetricValueWidth = 0 * ui.primaryMetricConfiguration.scale;
	FuncNode primaryMetricValueHeight = 0.08 * ui.primaryMetricConfiguration.scale;
	FuncNode primaryMetricValueX = primaryMetricBarX + 0.715 * ui.primaryMetricConfiguration.scale;
	FuncNode primaryMetricValueY = primaryMetricBarY - 0.035 * ui.primaryMetricConfiguration.scale;
	// 次要指标参数
	FuncNode secondaryMetricBarWidth = 0.55 * ui.secondaryMetricConfiguration.scale;
	FuncNode secondaryMetricBarHeight = 0.15 * ui.secondaryMetricConfiguration.scale;
	FuncNode secondaryMetricBarX = screen.r - interfaceGap - menuWidth - interfaceGap;
	FuncNode secondaryMetricBarY = screen.t - interfaceGap;
	FuncNode secondaryMetricValueWidth = 0 * ui.secondaryMetricConfiguration.scale;
	FuncNode secondaryMetricValueHeight = 0.08 * ui.secondaryMetricConfiguration.scale;
	FuncNode secondaryMetricValueX = secondaryMetricBarX - 0.035 * ui.secondaryMetricConfiguration.scale;
	FuncNode secondaryMetricValueY = secondaryMetricBarY - 0.035 * ui.secondaryMetricConfiguration.scale;
	// 连击指标参数
	FuncNode comboValueWidth = 0 * ui.comboConfiguration.scale;
	FuncNode comboValueHeight = stage.h * 0.12 * ui.comboConfiguration.scale;
	FuncNode comboValueX = stage.w * 0.4;
	FuncNode comboValueY = Lerp(t, b, 0.5);
	FuncNode comboTextWidth = 0 * ui.comboConfiguration.scale;
	FuncNode comboTextHeight = stage.h * 0.05 * ui.comboConfiguration.scale;
	FuncNode comboTextX = stage.w * 0.4;
	FuncNode comboTextY = Lerp(t, b, 0.5);
	EngineDataArchetype initialization = EngineDataArchetype(
		"Initialization", false, {},
		Execute({
			ui.menu.set(menuX, menuY, 1, 1, menuWidth, menuHeight, 0, ui.menuConfiguration.alpha, HorizontalAlign.Center, true),
			ui.primaryMetricBar.set(primaryMetricBarX, primaryMetricBarY, 0, 1, primaryMetricBarWidth, primaryMetricBarHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Left, true),
			ui.primaryMetricValue.set(primaryMetricValueX, primaryMetricValueY, 1, 1, primaryMetricValueWidth, primaryMetricValueHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Right, false),
			ui.secondaryMetricBar.set(secondaryMetricBarX, secondaryMetricBarY, 1, 1, secondaryMetricBarWidth, secondaryMetricBarHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Left, true),
			ui.secondaryMetricValue.set(secondaryMetricValueX, secondaryMetricValueY, 1, 1, secondaryMetricValueWidth, secondaryMetricValueHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Right, false),
			ui.comboValue.set(comboValueX, comboValueY, 0.5, 0.5, comboValueWidth, comboValueHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false),
			ui.comboText.set(comboTextX, comboTextY, 0.5, 2.75, comboTextWidth, comboTextHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false)
		}), 0, 1, Execute({}), 
		Execute({
			EntityDespawn.set(0, true),
		}), Execute({}), Execute({}), Execute({})
	);

	// <------ 输入管理器模块 ------>

	Array<LevelMemoryId> usedTouchIds = Array<LevelMemoryId>(16);
	auto isUsed = [&](Touch touch){return usedTouchIds.has(touch.id);};
	auto markAsUsed = [&](Touch touch){usedTouchIds.add(touch.id);};
	EngineDataArchetype inputManager = EngineDataArchetype(
		"Input Manager", true, {},
		Execute({}), 1, entityInfo[0].state == EntityState.Despawned, 
		Execute({}), Execute({}), Execute({
			usedTouchIds.clear()
		}), Execute({}), Execute({})
	);

	// <------ 舞台模块 ------>

	class StageFunction {
		public:

		var drawStage() {
			var stagel = stage.w / -2, stager = stage.w / 2, staget = stage.h / 2, stageb = stage.h / -2;
			var linel = screen.l, liner = screen.r, lineb = stageb - 20.0 / 1080.0, linet = stageb + 20.0 / 1080.0;
			var drawFunc = Execute({
				Draw(Sprite_Stage, stagel, stageb, stagel, staget, stager, staget, stager, stageb, 1, 1),
				Draw(Sprite_JudgeLine, linel, lineb, linel, linet, liner, linet, liner, lineb, 2, 1)
			});
			for (int i = 1; i <= 6; i++) {
				var notel = lines[i].lowPosition - noteSize / 2;
				var notet = stageb + noteSize / 2;
				var noter = lines[i].lowPosition + noteSize / 2;
				var noteb = stageb - noteSize / 2;
				drawFunc.args.push_back(Draw(Sprite_JudgeNote, notel, noteb, notel, notet, noter, notet, noter, noteb, 3, 1));
			} return drawFunc;
		}

		var onEmptyTop() {
			return Play(Effect_Stage, minSFXDistance);
		}
	}StageFunction;
	EngineDataArchetype stageArchetype = EngineDataArchetype(
		"Stage", true, {},
		Execute({}), 2, 1, Execute({}),
		StageFunction.drawStage(), Execute({
			If(
				LevelOption.get(Option_Autoplay),
				Execute({}),
				Execute({
					touchCounter.set(0),
					While(
						touchCounter.get() < touches.size,
						Execute({
							If (
								touches[touchCounter.get()].started &&
								!isUsed(touches[touchCounter.get()]) &&
								inClickBox(touches[touchCounter.get()]),
								StageFunction.onEmptyTop(),
								Execute({})
							), touchCounter.add(1)
						})
					)
				})
			)
		}), Execute({}), Execute({})
	);

	engineConfiguration = EngineConfiguration(
		{
			EngineConfigurationToggleOption("Autoplay", 0, true, Scope),
			EngineConfigurationToggleOption("Strict Judge", 0, true, Scope),
			EngineConfigurationSliderOption("Notes Speed", 5, 1, 50, 0.1, false, Scope),
			EngineConfigurationSliderOption("Notes Size", 1, 0.8, 1.2, 0.05, false, Scope, "#PERCENTAGE"),
			EngineConfigurationToggleOption("Sync Line", 1, false, Scope),
			EngineConfigurationToggleOption("Lock Aspect Radio", 1, false, Scope)
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
			{"Hanipure Judge Note", Sprite_JudgeNote},
			{"Hanipure Hold Body", Sprite_HoldBody}
		}, {
			{"Hanipure Perfect", Effect_Perfect},
			{"Hanipure Great", Effect_Great},
			{"Hanipure Good", Effect_Good},
			{"Hanipure Stage", Effect_Stage},
			{"Hanipure Flick", Effect_Flick},
			{"Hanipure Hold", Effect_Hold}
		}, {
			// Particles
		}, {
			// Buckets
		}, {
			// Archetypes
			initialization,
			inputManager,
			stageArchetype
		}, {}
	);

    // Compile the engine
    buffer resData, resConfiguration;
    build(engineConfiguration, engineData, resConfiguration, resData);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < resConfiguration.size(); i++) fout << resConfiguration.v[i];
    fout.close(); fout.open((dist + "/EngineData"));
    for (int i = 0; i < resData.size(); i++) fout << resData.v[i];
}
