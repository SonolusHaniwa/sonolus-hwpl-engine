class flickTutorial {
    public:

    var drop = {
        drawDrop(Sprites.NormalFlick, 3, timesNow)
    };

    var froze = {
        drawStatic(Sprites.NormalFlick, 3),
        instruction.set(Texts.flick),
        IF ((timesNow - appearTimeLength) % (frozenTimeLength * 2.0) < frozenTimeLength) {
            drawFrozenHand(3, ((timesNow - appearTimeLength) % frozenTimeLength) / frozenTimeLength * 100, 1, 0)
        } ELSE {
            drawUpperHand(3, ((timesNow - appearTimeLength) % frozenTimeLength) / frozenTimeLength * 100, handEndAngle)
        } FI,
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
}flickTutorial;