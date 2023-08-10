class holdFlickEndTutorial {
    public:

    var drop = {
        drawDrop(Sprites.NormalFlick, 3, timesNow),
        drawHoldBody(0, 3, appearTimeLength, 3, timesNow),
        drawUpperHand(3, 0, handEndAngle, 0, 0)
    };

    var froze = {
        drawStatic(Sprites.NormalFlick, 3),
        instruction.set(Texts.holdFlickEnd),
        drawUpperHand(3, ((timesNow - appearTimeLength) % frozenTimeLength) / frozenTimeLength * 100, handEndAngle)
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
}holdFlickEndTutorial;