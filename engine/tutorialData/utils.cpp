using namespace std;

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
		return newLerp(highWidth / lineNumber, 1.0 / lineNumber, 1 - (y(t) + stage.h / 2) / stage.h) * stage.w * 0.6;
	}
}lines;

var drawStage() {
    var stagel = stage.w / -2, stager = stage.w / 2, staget = stage.h / 2, stageb = stage.h / -2;
    var linel = screen.l, liner = screen.r, lineb = stageb - 20.0 / 1080.0, linet = stageb + 20.0 / 1080.0;
    var drawFunc = Execute({
        Draw(Sprites.Stage, stagel, stageb, stagel, staget, stager, staget, stager, stageb, 1, 1),
        Draw(Sprites.JudgeLine, linel, lineb, linel, linet, liner, linet, liner, lineb, 2, 1)
    });
    for (int i = 1; i <= 6; i++) {
        var notel = lines[i].lowPosition - noteSize / 2;
        var notet = stageb + noteSize / 2;
        var noter = lines[i].lowPosition + noteSize / 2;
        var noteb = stageb - noteSize / 2;
        drawFunc.args.push_back(Draw(Sprites.JudgeNote, notel, noteb, notel, notet, noter, notet, noter, noteb, 3, 1));
    } return drawFunc;
}