using namespace std;
using namespace playData;

const double maxSize = 2.5;                     // 判定大小
const var interfaceGap = 0.05;                  // 组件间距
const var targetAspectRatio = 1920.0 / 1080.0;  // 目标屏幕宽高比
const var lineNumber = 6;                       // 按键数
const var highWidth = 0.1;                      // 高位宽度与低位宽度比
const var defaultAppearTime = 5.0;              // Note 默认出现时间
const var stageMaxPercentage = 0.7;             // 舞台最大占比
const var minSFXDistance = 0.02;                // 最小音效时间

class stage {
	public:

	const var w = If(
		LevelOption.get(Options.lockAspectRatio),
		If(
			targetAspectRatio < screen.aspectRatio,
			screen.h * stageMaxPercentage * targetAspectRatio,
			screen.w * stageMaxPercentage
	    ), screen.w * stageMaxPercentage
	);
	const var h = If(
		LevelOption.get(Options.lockAspectRatio),
		If(
			targetAspectRatio < screen.aspectRatio,
			screen.h * stageMaxPercentage,
			screen.w * stageMaxPercentage / targetAspectRatio
		), screen.h * stageMaxPercentage
	);
}stage;

const var t = If(
	LevelOption.get(Options.lockAspectRatio) && screen.aspectRatio < targetAspectRatio,
	screen.w / targetAspectRatio * 0.5,
	screen.t
);
const var b = t - stage.h;
const var speed = stage.h / defaultAppearTime / 5.0 * LevelOption.get(Options.noteSpeed);
const var appearTimeLength = stage.h / speed;
const double noteSize = 128.0 / 1080.0;
var clickBoxl = stage.w / (-2.0) - stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxr = stage.w / 2.0 + stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;

class judgment {
	public:

	var perfect = If(
		LevelOption.get(Options.strictJudge) == 1,
		0.035, 0.05
	);
	var great = If(
		LevelOption.get(Options.strictJudge) == 1,
		0.05, 0.10
	);
	var good = If(
		LevelOption.get(Options.strictJudge) == 1,
		0.08, 0.15
	);
}judgment;

class score {
	public:

	var perfect = 1;
	var great = 0.75;
	var good = 0.5;
}score;