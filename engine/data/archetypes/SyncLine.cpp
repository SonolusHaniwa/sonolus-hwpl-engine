using namespace std;
using namespace playData;

class SyncLine: public Archetype {
    public:

    string name = "Hanipure Sync Line";
    
    vector<pair<string, int> > data = {{"beat", 0}, {"minLane", 1}, {"maxLane", 2}};
    function<var()> DrawSyncLine = [](){
		var beat = RuntimeUpdate.get(0) - EntityData.get(0) + appearTimeLength;
		var minLine = EntityData.get(1);
		var maxLine = EntityData.get(2);
		var leftX = lines[minLine].x(beat);
		var rightX = lines[maxLine].x(beat);
		var y = lines[minLine].y(beat);
		var w = lines[minLine].width(beat);
		return If(
			LevelOption.get(Options.syncLine) == 1,
			Draw(Sprites.SyncLine, leftX, y - w, leftX, y + w, rightX, y + w, rightX, y - w, 0, 1),
			Execute({})
		);
	};

    var shouldSpawn = times.now >= EntityData.get(0) - appearTimeLength;

    var spawnOrder = 1000 + EntityData.get(0);

    var preprocess = {
        IF (LevelOption.get(Options.mirror) == 1) {
            playLoopedId.set(EntityData.get(1)),
            EntityData.set(1, 7 - EntityData.get(2)),
            EntityData.set(2, 7 - playLoopedId.get()),
            playLoopedId.set(0)
        } FI,
    };

    var updateSequential = {
        DrawSyncLine()
    };

    var updateParallel = {
        IF (times.now > EntityData.get(0)) {
            EntityDespawn.set(0, 1)
        } FI,
    };
};