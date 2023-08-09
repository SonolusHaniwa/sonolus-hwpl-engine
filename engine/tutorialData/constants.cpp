using namespace std;

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

	const var w = screen.w * stageMaxPercentage;
	const var h = screen.h * stageMaxPercentage;
}stage;

const var t = screen.t;
const var b = t - stage.h;
const var speed = stage.h / defaultAppearTime;
const var appearTimeLength = stage.h / speed;
const double noteSize = 128.0 / 1080.0;
var clickBoxl = stage.w / (-2.0) - stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxr = stage.w / 2.0 + stage.w / lineNumber * (maxSize - 1.0) / 2.0;
var clickBoxt = stage.h / (-2.0) + 512.0 / 1080.0;
var clickBoxb = stage.h / (-2.0) - 512.0 / 1080.0;