using namespace std;

class HoldEnd: public Archetype {
    public:

    string name = "Hanipure Hold End";
    bool hasInput = true;

    vector<pair<string, int> > data = {{"beat", 0}, {"lane", 1}, {"last", 2}, {"start", 3}};
    var beat = EntityData.get(0);
    var lane = EntityData.get(1);
    var last = EntityData.get(2);
    var start = EntityData.get(3);
    var x = lines[lane].x(times.now - beat + appearTimeLength);
    var y = lines[lane].y(times.now - beat + appearTimeLength);
    var w = lines[lane].width(times.now - beat + appearTimeLength);
    var l = x - w, r = x + w;
    var t = y + w, b = y - w;

    var initialize = {
        IF (LevelOption.get(Options.mirror == 1)) {
            EntityData.set(1, 7 - lane)
        } FI,
        trackTouchId.set(0)
    };

    var shouldSpawn = times.now >= EntityDataArray.get(32 * last) - appearTimeLength;

    var spawnOrder = 1000 + EntityDataArray.get(32 * last);

    var updateSequential = {
        IF (times.now >= beat - appearTimeLength) {
            Draw(Sprites.NormalHold, l, b, l, t, r, t, r, b, 1000 - beat, 1)
        } FI,
        DrawHoldBody(),
        IF (LevelOption.get(Options.autoplay)) {
            IF (times.now >= EntityDataArray.get(32 * last) && playLoopedId.get() == 0) {
                playLoopedId.set(PlayLooped(Clips.Hold))
            } FI,
            IF (times.now >= beat) {
                EntityInput.set(0, 1),
                EntityInput.set(1, 0),
                StopLooped(playLoopedId.get()),
                Play(Clips.Perfect, minSFXDistance),
                EntityInput.set(2, Buckets.HoldEnd),
                EntityInput.set(3, 0),
				spawnParticleEffect(Effects.holdCircular, Effects.holdLinear, lane),
                EntityDespawn.set(0, 1)
            } FI,
        } FI,
    };

    var updateParallel = {
        IF (LevelOption.get(Options.autoplay) || times.now < EntityDataArray.get(last * 32) - judgment.good) {
            Return(0)
        } FI,
        // 音效播放
        IF (times.now > EntityDataArray.get(32 * last) - judgment.good && trackTouchId.get() != 0 && playLoopedId.get() == 0) {
            playLoopedId.set(PlayLooped(Clips.Hold))
        } FI, 
        // 没有 Touch 判定
        IF (times.now > EntityDataArray.get(32 * last) + judgment.good && trackTouchId.get() == 0) {
            EntityInput.set(0, 0),
            EntityDespawn.set(0, 1)
        } FI,
        // 超时
        IF (times.now > beat + judgment.good) {
            StopLooped(playLoopedId.get()),
            EntityInput.set(0, 0),
            EntityDespawn.set(0, 1)
        } FI
    };

    var touch = {
        IF (EntityInfoArray.get(3 * last + 2) != EntityState.Despawned) {
            Return(0)
        } FI,
        IF (LevelOption.get(Options.autoplay) || times.now < EntityDataArray.get(last * 32) - judgment.good) {
            Return(0)
        } FI,
        // Touch 监控
        IF (trackTouchId.get() == 0) {
            FOR (i, 0, touches.size, 1) {
                IF (lines[EntityDataArray.get(32 * last + 1)].inClickBox(touches[i]) && !isUsed(touches[i])) {
                    markAsUsed(touches[i]), 
                    trackTouchId.set(touches[i].id)
                } FI
            } DONE,
        } ELSE {
            FOR (i, 0, touches.size, 1) {
                IF (touches[i].id == trackTouchId.get() && touches[i].ended == 1) {
                    markAsUsed(touches[i]),
                    IF (beat - judgment.good <= times.now && times.now <= beat + judgment.good && lines[lane].inClickBox(touches[i])) {
                        EntityInput.set(1, touches[i].t - beat),
                        StopLooped(playLoopedId.get()),
                        judgeNote(),
                        EntityInput.set(2, Buckets.HoldEnd),
                        EntityInput.set(3, touches[i].t - beat),
						spawnParticleEffect(Effects.holdCircular, Effects.holdLinear, lane),
                        EntityDespawn.set(0, 1)
                    } ELSE {
                        StopLooped(playLoopedId.get()),
                        EntityInput.set(0, 0),
                        EntityInput.set(1, 0),
                        EntityDespawn.set(0, 1)
                    } FI,
                } FI,
            } DONE
        } FI,
    };
};
