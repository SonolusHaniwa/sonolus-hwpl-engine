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


var drawFrozenHand(var lane, var percent, bool in = true, bool out = true) {
	var angle = IF (percent < 25) {
		handStartAngle,
	} ELSE {
		IF (percent < 75) {
			(percent - 25) / 50 * (handEndAngle - handStartAngle) + handStartAngle
		} ELSE {
			handEndAngle
		} FI
	} FI;
	var goalX = lines[lane].x(appearTimeLength);
	var goalY = lines[lane].y(appearTimeLength);
	var centerX = goalX + handCircleR * Sin(handEndAngle / 180 * acos(-1));
	var centerY = goalY - handCircleR * Cos(handEndAngle / 180 * acos(-1));
	var x = centerX - handCircleR * Sin(angle / 180 * acos(-1));
	var y = centerY + handCircleR * Cos(angle / 180 * acos(-1));
	var a = IF (percent < 25) {
		(!in) ? 1 : 1.0 * percent / 25
	} ELSE {
		IF (percent < 75) {
			1
		} ELSE {
			(!out) ? 1 : 1.0 - 1.0 * (percent - 75) / 25
		} FI
	} FI;
	return Paint(Icons.hand, x, y, handSize, angle, 2000, a);
}

var drawUpperHand(var lane, var percent, var angle, bool in = false, bool out = true) {
	var goalX = lines[lane].x(appearTimeLength);
	var goalY = lines[lane].y(appearTimeLength);
	var x = lines[lane].x(appearTimeLength);
	var y = lines[lane].y(appearTimeLength) + IF (percent < 25) {
		0
	} ELSE {
		IF (percent < 75) {
			(percent - 25) / 50 * handUpperLength
		} ELSE {
			handUpperLength
		} FI
	} FI;
	var a = IF (percent < 25) {
		(!in) ? 1 : 1.0 * percent / 25
	} ELSE {
		IF (percent < 75) {
			1
		} ELSE {
			(!out) ? 1 : 1.0 - 1.0 * (percent - 75) / 25
		} FI
	} FI;
	return Paint(Icons.hand, x, y, handSize, angle, 2000, a);
}

var drawDrop(var id, var lane, var timesNow) {
	var x = lines[lane].x(timesNow);
	var y = lines[lane].y(timesNow);
	var w = lines[lane].width(timesNow);
	var l = x - w, r = x + w;
	var t = y + w, b = y - w;
	return Draw(id, l, b, l, t, r, t, r, b, 1000, 1);
}

var drawStatic(var id, var lane) {
	var x = lines[lane].x(appearTimeLength);
	var y = lines[lane].y(appearTimeLength);
	var w = lines[lane].width(appearTimeLength);
	var l = x - w, r = x + w;
	var t = y + w, b = y - w;
	return Draw(id, l, b, l, t, r, t, r, b, 1000, 1);
}

var drawHoldBody(var lastBeat, var lastLine, var thisBeat, var thisLine, var timeNow) {
    var lastTime = timeNow - lastBeat + appearTimeLength;
    var thisTime = timeNow - thisBeat + appearTimeLength;
    var realLastX = newLerp(lines[lastLine].lowPosition, lines[lastLine].highPosition, lines[lastLine].EaseVal(Min(1, 1 - lastTime / appearTimeLength)));
    var realThisX = newLerp(lines[thisLine].lowPosition, lines[thisLine].highPosition, lines[thisLine].EaseVal(Min(1, 1 - thisTime / appearTimeLength)));
    var lastX = If(
        lastTime > appearTimeLength,
        newLerp(lines[lastLine].lowPosition, lines[thisLine].lowPosition, (lastTime - appearTimeLength) / (thisBeat - lastBeat)),
        realLastX
    );
    var lastY = lines[lastLine].y(Min(appearTimeLength, lastTime));
    var lastW = lines[lastLine].width(Min(appearTimeLength, lastTime)) * 0.8;
    var thisX = If(
        thisTime < 0,
        newLerp(lines[thisLine].highPosition, lines[lastLine].highPosition, (0 - thisTime) / (thisBeat - lastBeat)),
        realThisX
    );
    var thisY = lines[thisLine].y(Min(appearTimeLength, Max(0, thisTime)));
    var thisW = lines[thisLine].width(Min(appearTimeLength, Max(0, thisTime))) * 0.8;
    return Draw(Sprites.HoldBody, lastX - lastW, lastY, thisX - thisW, thisY, thisX + thisW, thisY, lastX + lastW, lastY, 0, 1);
};