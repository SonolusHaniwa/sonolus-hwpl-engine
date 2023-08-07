using namespace std;

class Buckets {
    public:

    int NormalNote = 0;
	int NormalFlick = 1;
	int NormalHold = 2;
	int HoldEnd = 3;
	int HoldFlickEnd = 4;
	int HoldLine = 5;
}Buckets;

auto bucket = defineBuckets<class Buckets>({
    {
        .sprites = {
            {
                .id = Sprites.NormalNote,
                .x = 0,
                .y = 0,
                .w = 2.0,
                .h = 1.5,
                .rotation = 270,
            }
        },
        .unit = UnitText.Millisecond
    }, {
        .sprites = {
            {
                .id = Sprites.NormalFlick,
                .x = 0,
                .y = 0,
                .w = 2.0,
                .h = 1.5,
                .rotation = 270,
            }
        },
        .unit = UnitText.Millisecond
    }, {
        .sprites = {
            {
                .id = Sprites.HoldBody,
                .x = 0.25,
                .y = 0,
                .w = 1.75,
                .h = 1.6,
                .rotation = 0,
            }, {
                .id = Sprites.NormalHold,
                .x = -0.5,
                .y = 0,
                .w = 2.0,
                .h = 1.5,
                .rotation = 270,
            }
        },
        .unit = UnitText.Millisecond
    }, {
        .sprites = {
            {
                .id = Sprites.HoldBody,
                .x = -0.5,
                .y = 0,
                .w = 1.75,
                .h = 1.6,
                .rotation = 0,
            }, {
                .id = Sprites.NormalHold,
                .x = 0.5,
                .y = 0,
                .w = 2.0,
                .h = 1.5,
                .rotation = 270,
            }
        },
        .unit = UnitText.Millisecond
    }, {
        .sprites = {
            {
                .id = Sprites.HoldBody,
                .x = -0.5,
                .y = 0,
                .w = 1.75,
                .h = 1.6,
                .rotation = 0,
            }, {
                .id = Sprites.NormalFlick,
                .x = 0.5,
                .y = 0,
                .w = 2.0,
                .h = 1.5,
                .rotation = 270,
            }
        },
        .unit = UnitText.Millisecond
    }, {
        .sprites = {
            {
                .id = Sprites.HoldBody,
                .x = -0.125,
                .y = 0,
                .w = 1.75,
                .h = 1.6,
                .rotation = 0,
            }, {
                .id = Sprites.HoldLine,
                .x = 0.75,
                .y = 0,
                .w = 1.4,
                .h = 0.3,
                .rotation = 270,
            }
        },
        .unit = UnitText.Millisecond
    }
});