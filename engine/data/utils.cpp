using namespace std;
using namespace playData;

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
		return newLerp(highWidth / lineNumber, 1.0 / lineNumber, 1 - (y(t) + stage.h / 2) / stage.h) * stage.w * LevelOption.get(Options.noteSize) * 0.6;
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

auto inClickBox = [](Touch touch){
	return clickBoxl <= touch.x && touch.x <= clickBoxr && clickBoxb <= touch.y && touch.y <= clickBoxt;
};