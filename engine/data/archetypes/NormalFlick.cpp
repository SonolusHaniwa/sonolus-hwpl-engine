using namespace std;
using namespace playData;

auto movedLast = [](Touch touch) {
    return Abs(touch.dx) >= 0.05 && Abs(touch.dy) >= 0.05;
};

class NormalFlick: public Archetype {
    public:

    string name = "Hanipure Normal Flick";
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

    var spawnOrder = 1000 + EntityData.get(0);

    var shouldSpawn = RuntimeUpdate.get(0) >= EntityData.get(0) - appearTimeLength;

    var initialize = {
        IF (RandomInteger(0, 4) == 0) {
            isHighlighted.set(1)
        } ELSE {
            isHighlighted.set(0)
        } FI,
    };

    var updateSequential = {
        IF (isHighlighted.get() == 1) {
            Draw(Sprites.HighlightedFlick, l, b, l, t, r, t, r, b, 1000 - beat, 1)
        } ELSE {
            Draw(Sprites.NormalFlick, l, b, l, t, r, t, r, b, 1000 - beat, 1)
        } FI,
        IF (LevelOption.get(Options.autoplay) == 1 && times.now >= beat) {
            EntityInput.set(0, 1),
            EntityInput.set(1, 0),
            Play(Clips.Flick, minSFXDistance),
            EntityInput.set(2, Buckets.NormalFlick),
            EntityInput.set(3, 0),
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
            IF (touches[i].st >= beat - judgment.good && !isUsed(touches[i]) && lines[lane].inClickBoxST(touches[i]) && movedLast(touches[i])) {
                markAsUsed(touches[i]),
				EntityInput.set(1, touches[i].t - beat),
				EntityInput.set(0, 1),
				Play(Clips.Flick, minSFXDistance),
				EntityInput.set(2, Buckets.NormalFlick),
				EntityInput.set(3, touches[i].t - beat),
				EntityDespawn.set(0, 1)
            } FI,
        } DONE,
    };
};