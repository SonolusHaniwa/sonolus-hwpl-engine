using namespace std;
using namespace playData;

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

class NormalNote: public Archetype {
    public:

    string name = "Hanipure Normal Note";
    bool hasInput = true;

    vector<pair<string, int> > data = {{"beat", 0}, {"lane", 1}};
    var beat = EntityData.get(0);
    var lane = EntityData.get(1);
    var x = lines[lane].x(times.now - beat + appearTimeLength);
    var y = lines[lane].y(times.now - beat + appearTimeLength);
    var w = lines[lane].width(times.now - beat + appearTimeLength);
    var l = x - w, r = x + w;
    var t = y + w, b = y - w;

    var preprocess = {
        IF (LevelOption.get(Options.mirror) == 1) {
            EntityData.set(1, 7 - EntityData.get(1))
        } FI,
    };

    var spawnOrder = 1000 + beat;

    var shouldSpawn = times.now >= beat - appearTimeLength;

    var initialize = {
        IF (RandomInteger(0, 4) == 0) {
            isHighlighted.set(1)
        } ELSE {
            isHighlighted.set(0)
        } FI,
    };

    var updateSequential = {
        IF (isHighlighted.get() == 1) {
            Draw(Sprites.HighlightedNote, l, b, l, t, r, t, r, b, 1000 - beat, 1)
        } ELSE {
            Draw(Sprites.NormalNote, l, b, l, t, r, t, r, b, 1000 - beat, 1)
        } FI,
        IF (LevelOption.get(Options.autoplay) == 1 && times.now >= beat) {
            EntityInput.set(0, 1),
            EntityInput.set(1, 0),
            EntityInput.set(2, Buckets.NormalNote),
            EntityInput.set(3, 0),
            Play(Clips.Perfect, minSFXDistance),
            EntityDespawn.set(0, 1)
        } FI
    };

    var touch = {
        IF (times.now > beat + judgment.good) {
            Return(0)
        } FI,
        IF (LevelOption.get(Options.autoplay) || times.now < beat - judgment.good) {
            Return(0)
        } FI,
        FOR (i, 0, touches.size, 1) {
            IF (touches[i].started && !isUsed(touches[i]) && lines[lane].inClickBox(touches[i])) {
                markAsUsed(touches[i]),
                EntityInput.set(1, touches[i].t - beat),
                judgeNote(),
                EntityInput.set(2, Buckets.NormalNote),
                EntityInput.set(3, touches[i].t - beat),
                EntityDespawn.set(0, 1)
            } FI,
        } DONE,
    };

    var updateParallel = {
        IF (times.now > beat + judgment.good) {
            EntityInput.set(0, 0),
            EntityDespawn.set(0, 1)
        } FI
    };
};