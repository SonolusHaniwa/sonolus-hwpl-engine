class holdStartTutorial {
    public:

    var drop = {
        drawDrop(Sprites.NormalHold, 3, timesNow),
        drawHoldBody(appearTimeLength, 3, appearTimeLength * 2, 3, timesNow)
    };

    var froze = {
        drawStatic(Sprites.NormalHold, 3),
        drawHoldBody(0, 3, appearTimeLength, 3, 0),
        instruction.set(Texts.holdStart),
        drawFrozenHand(3, ((timesNow - appearTimeLength) % frozenTimeLength) / frozenTimeLength * 100, 1, 0)
    };

    var click = {
        instruction.set(-1),
        tutorialNavigation.add(1),
        tutorialStartTime.set(times.now)
    };

    var play = {
        IF (timesNow < appearTimeLength) {
            drop
        } ELSE {
            IF (timesNow < appearTimeLength + frozenTimes * frozenTimeLength) {
                froze
            } ELSE {
                click
            } FI,
        } FI,
    };
}holdStartTutorial;