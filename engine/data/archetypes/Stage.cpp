using namespace std;
using namespace playData;

class Stage: public Archetype {
    public:

    string name = "Hanipure Stage";

    var drawStage() {
        var stagel = stage.w / -2, stager = stage.w / 2, staget = stage.h / 2, stageb = stage.h / -2;
        var linel = screen.l, liner = screen.r, lineb = stageb - 20.0 / 1080.0, linet = stageb + 20.0 / 1080.0;
        var drawFunc = Execute({
            If(
                LevelOption.get(Options.stage),
                Draw(Sprites.Stage, stagel, stageb, stagel, staget, stager, staget, stager, stageb, 1, 1),
                Execute({})
            ), Draw(Sprites.JudgeLine, linel, lineb, linel, linet, liner, linet, liner, lineb, 2, 1)
        });
        for (int i = 1; i <= 6; i++) {
            var notel = lines[i].lowPosition - noteSize / 2;
            var notet = stageb + noteSize / 2;
            var noter = lines[i].lowPosition + noteSize / 2;
            var noteb = stageb - noteSize / 2;
            drawFunc.args.push_back(Draw(Sprites.JudgeNote, notel, noteb, notel, notet, noter, notet, noter, noteb, 3, 1));
        } return drawFunc;
    }

    var onEmptyTop() {
        return Play(Clips.Stage, minSFXDistance);
    }

    var spawnOrder = 2;

    var shouldSpawn = 1;

    var updateSequential = drawStage();

    int touchOrder = 1000;
    var touch = {
        IF (LevelOption.get(Options.autoplay) == 1) {
            Return(0)
        } FI,
        FOR (i, 0, touches.size, 1) {
            IF (touches[i].started && !isUsed(touches[i]) && inClickBox(touches[i])) {
                onEmptyTop(),
                markAsUsed(touches[i])
            } FI,
        } DONE,
    };
};