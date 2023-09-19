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
	var EaseVal(var x) {
		x = 1 - x;
		return Remap(Power({1.06, -45}), 1.06, 1.00, -0.06, Power({1.06, 45 * (x - 1)}));
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
	var inStrictClickBox(Touch touch) {
		var clickBoxl = lowPosition - stage.w / lineNumber * 0.3;
		var clickBoxr = lowPosition + stage.w / lineNumber * 0.3;
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

auto DrawHoldBody = [](){
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
    return Draw(Sprites.HoldBody, lastX - lastW, lastY, thisX - thisW, thisY, thisX + thisW, thisY, lastX + lastW, lastY, 0, 1);
};

auto movedLast = [](Touch touch) {
    return Abs(touch.dx) >= 0.01 && Abs(touch.dy) >= 0.01;
};

auto judgeNote = [](){
    return Switch(
        JudgeSimple(times.now, EntityData.get(0), judgment.perfect, judgment.great, judgment.good), {
            {1, Execute({
                EntityInput.set(0, 1),
                Play(Clips.Perfect, minSFXDistance)
            })}, {2, Execute({
                EntityInput.set(0, 2),
                Play(Clips.Great, minSFXDistance)
            })}, {3, Execute({
                EntityInput.set(0, 3),
                Play(Clips.Good, minSFXDistance)
            })}, {0, Execute({
                EntityInput.set(0, 0)
            })}
        }
    );
};

var spawnLaneEffect(var lane) {
	var t = lines[lane].y(0), b = lines[lane].y(appearTimeLength);
	var tx = lines[lane].x(0), bx = lines[lane].x(appearTimeLength);
	var tw = lines[lane].width(0) * 0.8, bw = lines[lane].width(appearTimeLength) * 0.8;
	return SpawnParticleEffect(Effects.stage, bx - bw, b, tx - tw, t, tx + tw, t, bx + bw, b, 0.5, 0);
}

var spawnParticleEffect(var circular, var linear, var lane) {
	var x = lines[lane].x(appearTimeLength);
	var y = lines[lane].y(appearTimeLength);
	var w = lines[lane].width(appearTimeLength);
	var l = x - w, r = x + w, l2 = x - w * 2, r2 = x + w * 2;
	var b = y, t = y + 0.5, b2 = y - 0.25, t2 = y + 0.25;
	return Execute({
		SpawnParticleEffect(linear, l, b, l, t, r, t, r, b, 0.5, 0),
		SpawnParticleEffect(circular, l2, b2, l2, t2, r2, t2, r2, b2, 0.5, 0),
		spawnLaneEffect(lane)
	});
}
