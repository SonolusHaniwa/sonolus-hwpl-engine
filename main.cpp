// 1022013

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
	Option_Mirror,
	Option_NotesSpeed,
	Option_NotesSize,
	Option_SyncLine,
	Option_LockAspectRadio,
	Option_Stage
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
	Sprite_HoldBody,
	Sprite_SyncLine
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

enum BucketType {
	Bucket_NormalNote,
	Bucket_NormalFlick,
	Bucket_NormalHold,
	Bucket_HoldEnd,
	Bucket_HoldFlickEnd,
	Bucket_HoldLine
};

// <------ 谱面转换 ------>

struct Note {
	NoteType type;
	double clickTime;
	int lastNote;
	bool isFake = false;
	bool isMulti = false;
	int Line;
};

string randomRef(int len) {
	string res = "";
	for (int i = 0; i < len; i++) res += char(rand() % 26 + 'A');
	return res;
}

const int refLen = 32;
Json::Value fromHanipure(Json::Value chart, double bgmOffset = 0) {
	Json::Value res; map<int, string> lastNoteRef, stNoteRef;
	map<double, int> minLane, maxLane;
	srand(time(0));
	// 谱面转换
	Json::Value single;
	single["archetype"] = "Hanipure Initialization";
	single["data"][0]["name"] = "beat"; single["data"][0]["value"] = 0;
	res.append(single);
	single["archetype"] = "Hanipure Input Manager";
	res.append(single);
	single["archetype"] = "Hanipure Stage";
	res.append(single);
	for (int i = 0; i < chart.size(); i++) {
		Json::Value note = chart[i];
		Json::Value single, data; single["data"].resize(0);
		switch(note[0].asInt()) {
			case 1: {
				single["archetype"] = "Hanipure Normal Note";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
			}; break; // Normal Note
			case 2: {
				single["archetype"] = "Hanipure Normal Flick";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
			}; break; // Normal Flick
			case 11: case 21: {
				single["archetype"] = "Hanipure Normal Hold";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				string randomId = randomRef(refLen);
				lastNoteRef[note[3].asInt()] = randomId;
				stNoteRef[note[3].asInt()] = randomId;
				single["ref"] = randomId;
			}; break; // Normal Hold
			case 22: {
				single["archetype"] = "Hanipure Hold Line";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				single["data"][2]["name"] = "last";
				single["data"][2]["ref"] = lastNoteRef[note[3].asInt()];
				single["data"][3]["name"] = "start";
				single["data"][3]["ref"] = stNoteRef[note[3].asInt()];
				string randomId = randomRef(refLen);
				lastNoteRef[note[3].asInt()] = randomId;
				single["ref"] = randomId;
			} break; // Hold Line
			case 12: case 23: {
				single["archetype"] = "Hanipure Hold End";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				single["data"][2]["name"] = "last";
				single["data"][2]["ref"] = lastNoteRef[note[3].asInt()];
				single["data"][3]["name"] = "start";
				single["data"][3]["ref"] = stNoteRef[note[3].asInt()];
			} break; // Hold Normal Note
			case 13: case 24: {
				single["archetype"] = "Hanipure Hold Flick End";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				single["data"][2]["name"] = "last";
				single["data"][2]["ref"] = lastNoteRef[note[3].asInt()];
				single["data"][3]["name"] = "start";
				single["data"][3]["ref"] = stNoteRef[note[3].asInt()];
			} break; // Hold Flick Note
		}
		if (note[0].asInt() != 22) {
			double beat = note[1].asDouble(); int lane = note[2].asInt();
			if (minLane.find(beat) == minLane.end()) minLane[beat] = lane;
			else minLane[beat] = min(minLane[beat], lane);
			if (maxLane.find(beat) == maxLane.end()) maxLane[beat] = lane;
			else maxLane[beat] = max(maxLane[beat], lane);
		}
		res.append(single);
	}
	// 计算同步线
	for (auto v : minLane) {
		double beat = v.first; int lane = v.second;
		if (lane == maxLane[beat]) continue;
		Json::Value single;
		single["archetype"] = "Hanipure Sync Line";
		single["data"][0]["name"] = "beat";
		single["data"][0]["value"] = beat;
		single["data"][1]["name"] = "minLane";
		single["data"][1]["value"] = lane;
		single["data"][2]["name"] = "maxLane";
		single["data"][2]["value"] = maxLane[beat];
		res.append(single);
	}
	Json::Value data;
	data["bgmOffset"] = bgmOffset;
	data["entities"] = res;
	return data;
}

// <------ 配置集合 ------>

const double maxSize = 2.5;               // 判定大小
const string Scope = "hwpl";              // 引擎标识符
const string dist = "./dist";             // 资源文件输出地址
const var interfaceGap = 0.05;            // 组件间距
const var targetAspectRadio = 1920.0 / 1080.0; // 目标屏幕宽高比
const var lineNumber = 6;                 // 按键数
const var highWidth = 0.1;                // 高位宽度与低位宽度比
const var defaultAppearTime = 5.0;        // Note 默认出现时间
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
const var speed = stage.h / defaultAppearTime / 5.0 * LevelOption.get(Option_NotesSpeed);
const var appearTimeLength = stage.h / speed;
auto newLerp = [](var a, var b, var x){
	var K = b - a;
	var B = a;
	return x * K + B;
};
class line {
	public:

	var i = 0;
	line(){}
	line(var i):i(i){};
	line operator [] (var i) {
		return line(i);
	}
	var EaseVal(var t) {
		return 1 - (1 - t) * (1 - t);
	}

	const var highPosition = (i - lineNumber / 2.0 - 0.5) * highWidth * stage.w / lineNumber;
	const var lowPosition = (i - lineNumber / 2.0 - 0.5) * stage.w / lineNumber;
	var y(var t) {
		return EaseVal(1 - t / appearTimeLength) * stage.h - stage.h / 2;
	}
	var x(var t) {
		return newLerp(lowPosition, highPosition, EaseVal(1 - t / appearTimeLength));
	}
	var width(var t) {
		return newLerp(highWidth / lineNumber, 1.0 / lineNumber, 1 - (y(t) + stage.h / 2) / stage.h) * stage.w * LevelOption.get(Option_NotesSize) * 0.6;
	}
	var inClickBox(Touch touch){
		var clickBoxl = lowPosition - stage.w / lineNumber * 0.3 * maxSize;
		var clickBoxr = lowPosition + stage.w / lineNumber * 0.3 * maxSize;
		var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
		var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;
		return clickBoxl <= touch.x && touch.x <= clickBoxr && clickBoxb <= touch.y && touch.y <= clickBoxt;
	};
	var inClickBoxST(Touch touch){
		var clickBoxl = lowPosition - stage.w / lineNumber * 0.3 * maxSize;
		var clickBoxr = lowPosition + stage.w / lineNumber * 0.3 * maxSize;
		var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
		var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;
		return clickBoxl <= touch.sx && touch.sx <= clickBoxr && clickBoxb <= touch.sy && touch.sy <= clickBoxt;
	};
	var inClickBoxLast(Touch touch){
		var clickBoxl = lowPosition - stage.w / lineNumber * 0.3 * maxSize;
		var clickBoxr = lowPosition + stage.w / lineNumber * 0.3 * maxSize;
		var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
		var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;
		var x = touch.x - touch.dx, y = touch.y - touch.dy;
		return clickBoxl <= x && x <= clickBoxr && clickBoxb <= y && y <= clickBoxt;
	}
}lines;
const double noteSize = 128.0 / 1080.0;
var clickBoxl = stage.w / (-2.0) - stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxr = stage.w / 2.0 + stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;
class judgment {
	public:

	var perfect = 0.05;
	var great = 0.10;
	var good = 0.15;
}judgment;
class score {
	public:

	var perfect = 1;
	var great = 0.75;
	var good = 0.5;
}score;
auto inClickBox = [](Touch touch){
	return clickBoxl <= touch.x && touch.x <= clickBoxr && clickBoxb <= touch.y && touch.y <= clickBoxt;
};

int main(int argc, char** argv) {
    EngineData engineData;
    EngineConfiguration engineConfiguration;

	// <------ 谱面转换测试 ------>
	
	if (argc >= 3) {
		Json::Value chart;
		string chartJson = readFile(argv[1]);
		json_decode(chartJson, chart);
		Json::Value LevelData = fromHanipure(chart);
		string dataJson = json_encode(LevelData);
		buffer data = compress_gzip(dataJson, 9);
		ofstream preFout(argv[2]);
		for (int i = 0; i < data.size(); i++) preFout << data.v[i];
		preFout.close();
	}

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
		"Hanipure Initialization", false, {},
		Execute({
			ui.menu.set(menuX, menuY, 1, 1, menuWidth, menuHeight, 0, ui.menuConfiguration.alpha, HorizontalAlign.Center, true),
			ui.primaryMetricBar.set(primaryMetricBarX, primaryMetricBarY, 0, 1, primaryMetricBarWidth, primaryMetricBarHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Left, true),
			ui.primaryMetricValue.set(primaryMetricValueX, primaryMetricValueY, 1, 1, primaryMetricValueWidth, primaryMetricValueHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Right, false),
			ui.secondaryMetricBar.set(secondaryMetricBarX, secondaryMetricBarY, 1, 1, secondaryMetricBarWidth, secondaryMetricBarHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Left, true),
			ui.secondaryMetricValue.set(secondaryMetricValueX, secondaryMetricValueY, 1, 1, secondaryMetricValueWidth, secondaryMetricValueHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Right, false),
			ui.comboValue.set(comboValueX, comboValueY, 0.5, 0.5, comboValueWidth, comboValueHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false),
			ui.comboText.set(comboTextX, comboTextY, 0.5, 2.75, comboTextWidth, comboTextHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false),
			LevelScore.set(0, score.perfect),
			LevelScore.set(1, score.great),
			LevelScore.set(2, score.good),
			lifes[3].set(0, 0, 0, -100),
			lifes[4].set(0, 0, 0, -100),
			lifes[5].set(0, 0, 0, -100),
			lifes[6].set(0, 0, 0, -100),
			lifes[7].set(0, 0, 0, -100),
			lifes[8].set(0, 0, 0, -10),
			buckets[0].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good),
			buckets[1].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good),
			buckets[2].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good),
			buckets[3].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good),
			buckets[4].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good),
			buckets[5].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good)
		}), 0, 1, Execute({}), 
		Execute({
			EntityDespawn.set(0, true),
		}), Execute({}), Execute({}), Execute({})
	);

	// <------ 输入管理器模块 ------>

	Array<LevelMemoryId> usedTouchIds = Array<LevelMemoryId>(16);
	auto isUsed = [&](Touch touch){return Execute({usedTouchIds.has(touch.id)});};
	auto markAsUsed = [&](Touch touch){return Execute({usedTouchIds.add(touch.id)});};
	EngineDataArchetype inputManager = EngineDataArchetype(
		"Hanipure Input Manager", false, {},
		Execute({}), 1, EntityInfoArray.get(2) == EntityState.Despawned, 
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
				If(
					LevelOption.get(Option_Stage),
					Draw(Sprite_Stage, stagel, stageb, stagel, staget, stager, staget, stager, stageb, 1, 1),
					Execute({})
				), Draw(Sprite_JudgeLine, linel, lineb, linel, linet, liner, linet, liner, lineb, 2, 1)
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
		"Hanipure Stage", false, {},
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
								Execute({
									StageFunction.onEmptyTop(),
									markAsUsed(touches[touchCounter.get()])
								}),
								Execute({})
							), touchCounter.add(1)
						})
					)
				})
			)
		}), Execute({}), Execute({})
	);
	stageArchetype.touch.order = 1000;
	
	// 	<------- 普通按键模块 ------>
	
	class NoteFunction {
		public:

		var x = lines[EntityData.get(1)].x(RuntimeUpdate.get(0) - EntityData.get(0) + appearTimeLength);
		var y = lines[EntityData.get(1)].y(RuntimeUpdate.get(0) - EntityData.get(0) + appearTimeLength);
		var w = lines[EntityData.get(1)].width(RuntimeUpdate.get(0) - EntityData.get(0) + appearTimeLength);
		var l = x - w, r = x + w;
		var t = y + w, b = y - w;
		Variable<EntityMemoryId> isHighlighted;
		Variable<EntityMemoryId> touchCounter;
		Variable<EntityMemoryId> playLoopedId;
		Variable<EntityMemoryId> trackTouchId;
	}NoteFunction;
	auto judgeNote = [&](){
		return Switch(
			JudgeSimple(RuntimeUpdate.get(0), EntityData.get(0), judgment.perfect, judgment.great, judgment.good), {
				{1, Execute({
					EntityInput.set(0, 1),
					Play(Effect_Perfect, minSFXDistance)
				})}, {2, Execute({
					EntityInput.set(0, 2),
					Play(Effect_Great, minSFXDistance)
				})}, {3, Execute({
					EntityInput.set(0, 3),
					Play(Effect_Good, minSFXDistance)
				})}, {0, Execute({
					EntityInput.set(0, 0)
				})}
			}
		);
	};
	EngineDataArchetype noteArchetype = EngineDataArchetype(
		"Hanipure Normal Note", true, {{"beat", 0}, {"lane", 1}},
		Execute({}), 1000 + EntityData.get(0), RuntimeUpdate.get(0) >= EntityData.get(0) - appearTimeLength, 
		Execute({  // initialize
			NoteFunction.isHighlighted.set(If(
				RandomInteger(1, 4) == 1,
				1, 
				0
			))
		}), Execute({ // updateSequential
			If(
				NoteFunction.isHighlighted.get() && RuntimeUpdate.get(0) <= EntityData.get(0),
				Draw(Sprite_HighlightedNote, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1),
				Draw(Sprite_NormalNote, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1)
			), If(
				LevelOption.get(Option_Autoplay) && RuntimeUpdate.get(0) >= EntityData.get(0),
				Execute({
					EntityInput.set(0, 1),
					EntityInput.set(1, 0),
					EntityInput.set(2, Bucket_NormalNote),
					EntityInput.set(3, 0),
					Play(Effect_Perfect, minSFXDistance),
					EntityDespawn.set(0, 1)
				}), Execute({})
			)
		}), Execute({ // touch
			If(
				RuntimeUpdate.get(0) > EntityData.get(0) + judgment.good,
				Execute({}),	
				Execute({
					If(
						LevelOption.get(Option_Autoplay) || RuntimeUpdate.get(0) < EntityData.get(0) - judgment.good,
						Execute({}),
						Execute({
							NoteFunction.touchCounter.set(0),
							While(
								NoteFunction.touchCounter.get() < touches.size,
								Execute({
									If(
										touches[NoteFunction.touchCounter.get()].started && 
										!isUsed(touches[NoteFunction.touchCounter.get()]) &&
										lines[EntityData.get(1)].inClickBox(touches[NoteFunction.touchCounter.get()]),
										Execute({
											markAsUsed(touches[NoteFunction.touchCounter.get()]),
											EntityInput.set(1, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
											judgeNote(),
											EntityInput.set(2, Bucket_NormalNote),
											EntityInput.set(3, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
											EntityDespawn.set(0, 1)
										}), Execute({})
									),
									NoteFunction.touchCounter.add(1),
								})
							)
						})
					)
				})
			)
		}), Execute({ // updateParallel
			If(
				RuntimeUpdate.get(0) > EntityData.get(0) + judgment.good,
				Execute({
					EntityInput.set(0, 0),
					EntityDespawn.set(0, 1)
				}), Execute({})
			)
		}), Execute({}) // terminate
	);

	// <------ Flick 模块 ------>

	EngineDataArchetype flickArchetype = noteArchetype;
	flickArchetype.name = "Hanipure Normal Flick";
	flickArchetype.updateSequential = Execute({
		If(
			NoteFunction.isHighlighted.get() && RuntimeUpdate.get(0) <= EntityData.get(0),
			Draw(Sprite_HighlightedFlick, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1),
			Draw(Sprite_NormalFlick, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1)
		), If(
			LevelOption.get(Option_Autoplay) && RuntimeUpdate.get(0) >= EntityData.get(0),
			Execute({
				EntityInput.set(0, 1),
				EntityInput.set(1, 0),
				Play(Effect_Flick, minSFXDistance),
				EntityInput.set(2, Bucket_NormalFlick),
				EntityInput.set(3, 0),
				EntityDespawn.set(0, 1)
			}), Execute({})
		)
	});
	auto moved = [&](Touch touch) {
		return touch.x != touch.sx || touch.y != touch.sy;
	};
	auto movedLast = [&](Touch touch){
		return Abs(touch.dx) >= 0.01 || Abs(touch.dy) >= 0.01;
	};
	flickArchetype.touch = Execute({
		If(
			RuntimeUpdate.get(0) > EntityData.get(0) + judgment.good,
			Execute({}),
			Execute({
				If(
					LevelOption.get(Option_Autoplay) || RuntimeUpdate.get(0) < EntityData.get(0) - judgment.good,
					Execute({}),
					Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size,
							Execute({
								If(
									touches[NoteFunction.touchCounter.get()].st >= EntityData.get(0) - judgment.good && // 在判定时间之后
									!isUsed(touches[NoteFunction.touchCounter.get()]) && // 没被使用过的 touch
									lines[EntityData.get(1)].inClickBoxST(touches[NoteFunction.touchCounter.get()]) && // 在判定范围之内
									moved(touches[NoteFunction.touchCounter.get()]), // 有位移
									Execute({
										markAsUsed(touches[NoteFunction.touchCounter.get()]),
										EntityInput.set(1, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
										EntityInput.set(0, 1),
										Play(Effect_Flick, minSFXDistance),
										EntityInput.set(2, Bucket_NormalFlick),
										EntityInput.set(3, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
										EntityDespawn.set(0, 1)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					})
				)
			})
		)
	});

	// <------ HoldStart 模块 ------>
	
	EngineDataArchetype holdStartArchetype = noteArchetype;
	holdStartArchetype.name = "Hanipure Normal Hold";
	holdStartArchetype.updateSequential = Execute({
		If(
			NoteFunction.isHighlighted.get() && RuntimeUpdate.get(0) <= EntityData.get(0),
			Draw(Sprite_HighlightedHold, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1),
			Draw(Sprite_NormalHold, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1)
		), If(
			LevelOption.get(Option_Autoplay) && RuntimeUpdate.get(0) >= EntityData.get(0),
			Execute({
				EntityInput.set(0, 1),
				EntityInput.set(1, 0),
				Play(Effect_Perfect, minSFXDistance),
				EntityInput.set(2, Bucket_NormalHold),
				EntityInput.set(3, 0),
				EntityDespawn.set(0, 1)
			}), Execute({})
		)
	});
	holdStartArchetype.touch = Execute({ // touch
		If(
			RuntimeUpdate.get(0) > EntityData.get(0) + judgment.good,
			Execute({}),	
			Execute({
				If(
					LevelOption.get(Option_Autoplay) || RuntimeUpdate.get(0) < EntityData.get(0) - judgment.good,
					Execute({}),
					Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size,
							Execute({
								If(
									touches[NoteFunction.touchCounter.get()].started && 
									!isUsed(touches[NoteFunction.touchCounter.get()]) &&
									lines[EntityData.get(1)].inClickBox(touches[NoteFunction.touchCounter.get()]),
									Execute({
										EntityInput.set(1, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
//										Debuglog(touches[NoteFunction.touchCounter.get()].id),
										judgeNote(),
										EntityInput.set(2, Bucket_NormalHold),
										EntityInput.set(3, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
										EntityDespawn.set(0, 1)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					})
				)
			})
		)
	});
	
	auto DrawHoldBody = [&](){
		var lastId = EntityData.get(2);
		var lastBeat = EntityDataArray.get(lastId * 32);
		var lastLine = EntityDataArray.get(lastId * 32 + 1);
		var thisLine = EntityData.get(1);
		var timeNow = RuntimeUpdate.get(0);
		var lastTime = timeNow - lastBeat + appearTimeLength;
		var thisTime = timeNow - EntityData.get(0) + appearTimeLength;
		var realLastX = newLerp(lines[lastLine].lowPosition, lines[lastLine].highPosition, lines[lastLine].EaseVal(Min(1, 1 - lastTime / appearTimeLength)));
		var realThisX = newLerp(lines[thisLine].lowPosition, lines[thisLine].highPosition, lines[thisLine].EaseVal(Min(1, 1 - thisTime / appearTimeLength)));
		var lastX = If(
			lastTime > appearTimeLength,
			newLerp(lines[lastLine].lowPosition, lines[thisLine].lowPosition, (lastTime - appearTimeLength) / (EntityData.get(0) - lastBeat)),
			realLastX
		);
		var lastY = lines[lastLine].y(Min(appearTimeLength, lastTime));
		var lastW = lines[lastLine].width(Min(appearTimeLength, lastTime)) * 0.8;
		var thisX = If(
			thisTime < 0,
			newLerp(lines[thisLine].highPosition, lines[lastLine].highPosition, (0 - thisTime) / (EntityData.get(0) - lastBeat)),
			realThisX
		);
		var thisY = lines[thisLine].y(Min(appearTimeLength, Max(0, thisTime)));
		var thisW = lines[thisLine].width(Min(appearTimeLength, Max(0, thisTime))) * 0.8;
		return Draw(Sprite_HoldBody, lastX - lastW, lastY, thisX - thisW, thisY, thisX + thisW, thisY, lastX + lastW, lastY, 0, 1);
	};

	// <------ HoldEnd 模块 ------>
	
	EngineDataArchetype holdEndArchetype = noteArchetype;
	holdEndArchetype.name = "Hanipure Hold End";
	holdEndArchetype.preprocess = Execute({NoteFunction.trackTouchId.set(0)});
	holdEndArchetype.shouldSpawn = RuntimeUpdate.get(0) >= EntityDataArray.get(32 * EntityData.get(2)) - appearTimeLength;
	holdEndArchetype.spawnOrder = 1000 + EntityDataArray.get(32 * EntityData.get(2));
	holdEndArchetype.updateSequential = Execute({
		If(
			RuntimeUpdate.get(0) >= EntityData.get(0) - appearTimeLength,
			Draw(Sprite_NormalHold, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1), 
			Execute({})
		), DrawHoldBody(), If(
			LevelOption.get(Option_Autoplay),
			Execute({
				If(
					RuntimeUpdate.get(0) >= EntityDataArray.get(32 * EntityData.get(2)) && NoteFunction.playLoopedId.get() == 0,
					Execute({
						NoteFunction.playLoopedId.set(PlayLooped(Effect_Hold))
					}), Execute({})
				),
				If(
					RuntimeUpdate.get(0) >= EntityData.get(0),
					Execute({
						EntityInput.set(0, 1),
						EntityInput.set(1, 0),
						StopLooped(NoteFunction.playLoopedId.get()),
						Play(Effect_Perfect, minSFXDistance),
						EntityInput.set(2, Bucket_HoldEnd),
						EntityInput.set(3, 0),
						EntityDespawn.set(0, 1)
					}), Execute({})
				)
			}), Execute({})
		)
	});
	holdEndArchetype.updateParallel = Execute({
		If(
			LevelOption.get(Option_Autoplay) || RuntimeUpdate.get(0) < EntityDataArray.get(EntityData.get(2) * 32) - judgment.good,
			Execute({}),
			Execute({
				If(
					RuntimeUpdate.get(0) > EntityDataArray.get(EntityData.get(2) * 32) + judgment.good && NoteFunction.trackTouchId.get() == 0,
					Execute({
						EntityInput.set(0, 0),
						EntityInput.set(1, 0),
						EntityDespawn.set(0, 1)
					}), Execute({})
				),
				If(
					NoteFunction.trackTouchId.get() == 0,
					Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size && NoteFunction.trackTouchId.get() == 0,
							Execute({
								If(
									lines[EntityDataArray.get(EntityData.get(2) * 32 + 1)].inClickBox(touches[NoteFunction.touchCounter.get()]) && // 以上一个 hold 开始的 touch
									touches[NoteFunction.touchCounter.get()].st >= EntityDataArray.get(EntityData.get(3) * 32) - judgment.good && // 在判定时间之后
									!isUsed(touches[NoteFunction.touchCounter.get()]) &&
									touches[NoteFunction.touchCounter.get()].st <= EntityDataArray.get(EntityData.get(2) * 32) + judgment.good, // 在判定时间之前
									Execute({
										NoteFunction.trackTouchId.set(touches[NoteFunction.touchCounter.get()].id),
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					}), Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size,
							Execute({
								If(
									touches[NoteFunction.touchCounter.get()].id == NoteFunction.trackTouchId.get() &&
									touches[NoteFunction.touchCounter.get()].ended == 1,
									Execute({
										If(
											RuntimeUpdate.get(0) >= EntityData.get(0) - judgment.good &&
											RuntimeUpdate.get(0) <= EntityData.get(0) + judgment.good && // 以正确时间结束
											lines[EntityData.get(1)].inClickBox(touches[NoteFunction.touchCounter.get()]), // 以正确位置结束
											Execute({
												EntityInput.set(1, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
												judgeNote(),
												EntityInput.set(2, Bucket_HoldEnd),
												EntityInput.set(3, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
												EntityDespawn.set(0, 1)
											}), Execute({
												EntityInput.set(0, 0),
												EntityInput.set(1, 0),
												EntityDespawn.set(0, 1)
											})
										)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					})
				)
			})
		)
	});
	holdEndArchetype.touch = Execute({});
	holdEndArchetype.data.push_back({"last", 2});
	holdEndArchetype.data.push_back({"start", 3});

	// <------ HoldFlickEnd 模块 ------>
	
	EngineDataArchetype holdFlickEndArchetype = noteArchetype;
	holdFlickEndArchetype.name = "Hanipure Hold Flick End";
	holdFlickEndArchetype.preprocess = Execute({NoteFunction.trackTouchId.set(0)});
	holdFlickEndArchetype.shouldSpawn = RuntimeUpdate.get(0) >= EntityDataArray.get(32 * EntityData.get(2)) - appearTimeLength;
	holdFlickEndArchetype.spawnOrder = 1000 + EntityDataArray.get(32 * EntityData.get(2));
	holdFlickEndArchetype.updateSequential = Execute({
		If(
			RuntimeUpdate.get(0) >= EntityData.get(0) - appearTimeLength,
			Draw(Sprite_NormalFlick, NoteFunction.l, NoteFunction.b, NoteFunction.l, NoteFunction.t, NoteFunction.r, NoteFunction.t, NoteFunction.r, NoteFunction.b, 1000 - EntityData.get(0), 1), Execute({})
		), DrawHoldBody(), If(
			LevelOption.get(Option_Autoplay),
			Execute({
				If(
					RuntimeUpdate.get(0) >= EntityDataArray.get(32 * EntityData.get(2)) && NoteFunction.playLoopedId.get() == 0,
					Execute({
						NoteFunction.playLoopedId.set(PlayLooped(Effect_Hold))
					}), Execute({})
				),
				If(
					RuntimeUpdate.get(0) >= EntityData.get(0),
					Execute({
						EntityInput.set(0, 1),
						EntityInput.set(1, 0),
						StopLooped(NoteFunction.playLoopedId.get()),
						Play(Effect_Flick, minSFXDistance),
						EntityInput.set(2, Bucket_HoldFlickEnd),
						EntityInput.set(3, 0),
						EntityDespawn.set(0, 1)
					}), Execute({})
				)
			}), Execute({})
		)
	});
	holdFlickEndArchetype.updateParallel = Execute({
		If(
			LevelOption.get(Option_Autoplay) || RuntimeUpdate.get(0) < EntityDataArray.get(EntityData.get(2) * 32) - judgment.good,
			Execute({}),
			Execute({
				If(
					RuntimeUpdate.get(0) > EntityDataArray.get(EntityData.get(2) * 32) + judgment.good && NoteFunction.trackTouchId.get() == 0,
					Execute({
						EntityInput.set(0, 0),
						EntityInput.set(1, 0),
						EntityDespawn.set(0, 1)
					}), Execute({})
				),
				If(
					NoteFunction.trackTouchId.get() == 0,
					Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size && NoteFunction.trackTouchId.get() == 0,
							Execute({
								If(
									lines[EntityDataArray.get(EntityData.get(2) * 32 + 1)].inClickBox(touches[NoteFunction.touchCounter.get()]) && // 以上一个 hold 开始的 touch
									touches[NoteFunction.touchCounter.get()].st >= EntityDataArray.get(EntityData.get(3) * 32) - judgment.good && // 在判定时间之后
									!isUsed(touches[NoteFunction.touchCounter.get()]) &&
									touches[NoteFunction.touchCounter.get()].st <= EntityDataArray.get(EntityData.get(2) * 32) + judgment.good, // 在判定时间之前
									Execute({
										NoteFunction.trackTouchId.set(touches[NoteFunction.touchCounter.get()].id)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					}), Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size,
							Execute({
								If(
									touches[NoteFunction.touchCounter.get()].id == NoteFunction.trackTouchId.get() &&
									touches[NoteFunction.touchCounter.get()].ended == 1,
									Execute({
										If(
											RuntimeUpdate.get(0) >= EntityData.get(0) - judgment.good &&
											RuntimeUpdate.get(0) <= EntityData.get(0) + judgment.good && // 以正确时间结束
											lines[EntityData.get(1)].inClickBoxLast(touches[NoteFunction.touchCounter.get()]) && // 以正确位置结束
											movedLast(touches[NoteFunction.touchCounter.get()]), // 有位移
											Execute({
												EntityInput.set(0, 1),
												EntityInput.set(1, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
												Play(Effect_Flick, minSFXDistance),
												EntityInput.set(2, Bucket_HoldFlickEnd),
												EntityInput.set(3, touches[NoteFunction.touchCounter.get()].t - EntityData.get(0)),
												EntityDespawn.set(0, 1)
											}), Execute({
												EntityInput.set(0, 0),
												EntityInput.set(1, 0),
												EntityDespawn.set(0, 1)
											})
										)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					})
				)
			})
		)
	});
	holdFlickEndArchetype.touch = Execute({});
	holdFlickEndArchetype.data.push_back({"last", 2});
	holdFlickEndArchetype.data.push_back({"start", 3});

	// <------ HoldLine 模块 ------>
	
	EngineDataArchetype holdLineArchetype = noteArchetype;
	holdLineArchetype.name = "Hanipure Hold Line";
	holdLineArchetype.preprocess = Execute({NoteFunction.trackTouchId.set(0)});
	holdLineArchetype.shouldSpawn = RuntimeUpdate.get(0) >= EntityDataArray.get(32 * EntityData.get(2)) - appearTimeLength;
	holdLineArchetype.spawnOrder = 1000 + EntityDataArray.get(32 * EntityData.get(2));
	holdLineArchetype.updateSequential = Execute({
		If(
			RuntimeUpdate.get(0) >= EntityData.get(0) - appearTimeLength && RuntimeUpdate.get(0) <= EntityData.get(0),
			Draw(Sprite_HoldLine, NoteFunction.x - NoteFunction.w * 0.7, NoteFunction.b, NoteFunction.x - NoteFunction.w * 0.7, NoteFunction.t, NoteFunction.x + NoteFunction.w * 0.7, NoteFunction.t, NoteFunction.x + NoteFunction.w * 0.7, NoteFunction.b, 1000 - EntityData.get(0), 1), Execute({})
		), DrawHoldBody(), If(
			LevelOption.get(Option_Autoplay),
			Execute({
				If(
					RuntimeUpdate.get(0) >= EntityDataArray.get(32 * EntityData.get(2)) && NoteFunction.playLoopedId.get() == 0,
					Execute({
						NoteFunction.playLoopedId.set(PlayLooped(Effect_Hold))
					}), Execute({})
				),
				If(
					RuntimeUpdate.get(0) >= EntityData.get(0),
					Execute({
						EntityInput.set(0, 1),
						EntityInput.set(1, 0),
						StopLooped(NoteFunction.playLoopedId.get()),
						Play(Effect_Perfect, minSFXDistance),
						EntityInput.set(2, Bucket_HoldLine),
						EntityInput.set(3, 0),
						EntityDespawn.set(0, 1)
					}), Execute({})
				)
			}), Execute({})
		)
	});
	holdLineArchetype.updateParallel = Execute({
		If(
			LevelOption.get(Option_Autoplay) || RuntimeUpdate.get(0) < EntityDataArray.get(EntityData.get(2) * 32) - judgment.good,
			Execute({}),
			Execute({
				If(
					RuntimeUpdate.get(0) > EntityDataArray.get(EntityData.get(2) * 32) + judgment.good && NoteFunction.trackTouchId.get() == 0,
					Execute({
						EntityInput.set(0, 0),
						EntityInput.set(1, 0),
						EntityDespawn.set(0, 1)
					}), Execute({})
				),
				If(
					NoteFunction.trackTouchId.get() == 0,
					Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size && NoteFunction.trackTouchId.get() == 0,
							Execute({
								If(
									lines[EntityDataArray.get(EntityData.get(2) * 32 + 1)].inClickBox(touches[NoteFunction.touchCounter.get()]) && // 以上一个 hold 开始的 touch
									touches[NoteFunction.touchCounter.get()].st >= EntityDataArray.get(EntityData.get(3) * 32) - judgment.good && // 在判定时间之后
									!isUsed(touches[NoteFunction.touchCounter.get()]) &&
									touches[NoteFunction.touchCounter.get()].st <= EntityDataArray.get(EntityData.get(2) * 32) + judgment.good, // 在判定时间之前
									Execute({
										NoteFunction.trackTouchId.set(touches[NoteFunction.touchCounter.get()].id)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1)
							})
						)
					}), Execute({
						NoteFunction.touchCounter.set(0),
						While(
							NoteFunction.touchCounter.get() < touches.size,
							Execute({
								If(
									touches[NoteFunction.touchCounter.get()].id == NoteFunction.trackTouchId.get(),
									Execute({
										If(
											RuntimeUpdate.get(0) >= EntityData.get(0) &&
											lines[EntityData.get(1)].inClickBox(touches[NoteFunction.touchCounter.get()]), // 以正确位置结束
											Execute({
												EntityInput.set(0, 1),
												EntityInput.set(1, 0),
												Play(Effect_Perfect, minSFXDistance),
												EntityInput.set(2, Bucket_HoldLine),
												EntityInput.set(3, 0),
												EntityDespawn.set(0, 1)
											}), Execute({
												If(
													touches[NoteFunction.touchCounter.get()].ended == 1,
													Execute({
														EntityInput.set(0, 0),
														EntityInput.set(1, 0),
														// Debuglog(touches[NoteFunction.touchCounter.get()].id),
														
														EntityDespawn.set(0, 1)
													}), Execute({})
												)
											})
										)
									}), Execute({})
								),
								NoteFunction.touchCounter.add(1),
							})
						)
					})
				)
			})
		)
	});
	holdLineArchetype.touch = Execute({});
	holdLineArchetype.data.push_back({"last", 2});
	holdLineArchetype.data.push_back({"start", 3});
//	holdLineArchetype.updateParallel.order = 100;

	// <------ 同步线模块 ------>
	
	EngineDataArchetype syncLine = noteArchetype;
	syncLine.name = "Hanipure Sync Line";
	syncLine.data = {{"beat", 0}, {"minLane", 1}, {"maxLane", 2}};
	syncLine.hasInput = false;
	auto DrawSyncLine = [](){
		var beat = RuntimeUpdate.get(0) - EntityData.get(0) + appearTimeLength;
		var minLine = EntityData.get(1);
		var maxLine = EntityData.get(2);
		var leftX = lines[minLine].x(beat);
		var rightX = lines[maxLine].x(beat);
		var y = lines[minLine].y(beat);
		var w = lines[minLine].width(beat);
		return Draw(Sprite_SyncLine, leftX, y - w, leftX, y + w, rightX, y + w, rightX, y - w, 0, 1);
	};
	syncLine.updateSequential = Execute({
		DrawSyncLine()
	});
	syncLine.updateParallel = Execute({
		If(
			RuntimeUpdate.get(0) > EntityData.get(0),
			EntityDespawn.set(0, 1),
			Execute({})
		)
	});

	// <------ 引擎配置 ------>
	
	engineConfiguration = EngineConfiguration(
		{
			EngineConfigurationToggleOption("Autoplay", 0, true, Scope),
			EngineConfigurationToggleOption("Strict Judge", 0, true, Scope),
			EngineConfigurationToggleOption("Mirror", 0, false, Scope),
			EngineConfigurationSliderOption("Notes Speed", 5, 1, 50, 0.1, false, Scope),
			EngineConfigurationSliderOption("Notes Size", 1, 0.5, 2.0, 0.1, false, Scope, "#PERCENTAGE"),
			EngineConfigurationToggleOption("Sync Line", 1, false, Scope),
			EngineConfigurationToggleOption("Lock Aspect Radio", 1, false, Scope),
			EngineConfigurationToggleOption("Stage", 1, false, Scope)
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
			{"Hanipure Hold Line", Sprite_HoldLine},
			{"Hanipure Judge Note", Sprite_JudgeNote},
			{"Hanipure Hold Body", Sprite_HoldBody},
			{"Hanipure Sync Line", Sprite_SyncLine}
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
			EngineDataBucket({
				EngineDataBucketSprite(Sprite_NormalNote, 0, 0, 2, 1.5, 270)
			}), EngineDataBucket({
				EngineDataBucketSprite(Sprite_NormalFlick, 0, 0, 2, 1.5, 270)
			}), EngineDataBucket({
				EngineDataBucketSprite(Sprite_HoldBody, 0.25, 0, 1.75, 1.6, 0),
				EngineDataBucketSprite(Sprite_NormalHold, -0.5, 0, 2, 1.5, 270)
			}), EngineDataBucket({
				EngineDataBucketSprite(Sprite_HoldBody, -0.5, 0, 1.75, 1.6, 0),
				EngineDataBucketSprite(Sprite_NormalHold, 0.5, 0, 2, 1.5, 270),
			}), EngineDataBucket({
				EngineDataBucketSprite(Sprite_HoldBody, -0.5, 0, 1.75, 1.6, 0),
				EngineDataBucketSprite(Sprite_NormalFlick, 0.5, 0, 2, 1.5, 270)
			}), EngineDataBucket({
				EngineDataBucketSprite(Sprite_HoldBody, -0.125, 0, 1.75, 1.6, 0),
				EngineDataBucketSprite(Sprite_HoldLine, 0.75, 0, 1.4, 0.3, 270)
			})
		}, {
			// Archetypes
			initialization,
			inputManager,
			stageArchetype,
			noteArchetype,
			flickArchetype,
			holdStartArchetype,
			holdEndArchetype,
			holdFlickEndArchetype,
			holdLineArchetype,
			syncLine
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
