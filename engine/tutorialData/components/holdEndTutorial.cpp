class holdEndTutorial {
    public:

    var drop = {
        drawDrop(Sprites.NormalHold, 3, timesNow),
        drawHoldBody(0, 3, appearTimeLength, 3, timesNow),
        drawUpperHand(3, 0, handEndAngle, 0, 0)
    };

    var froze = {
        drawStatic(Sprites.NormalHold, 3),
        instruction.set(Texts.holdEnd),
        drawFrozenHand(3, 100 - ((timesNow - appearTimeLength) % frozenTimeLength) / frozenTimeLength * 100, 1, 0)
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
}holdEndTutorial;