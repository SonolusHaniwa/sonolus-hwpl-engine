import {
    And,
    Divide,
    Draw,
    EntityInfo,
    EntityMemory,
    Equal,
    GreaterOr,
    Lerp,
    Multiply,
    Or,
    SkinSprite,
    SScript,
    State,
    Subtract,
    Time,
} from 'sonolus.js'
import { options } from '../../configuration/options'
import {
    Layer,
    noteBaseBottom,
    noteBaseBottomScale,
    noteBaseTop,
    noteBaseTopScale,
    noteOnScreenDuration,
    stageTop,
} from './common/constants'
import { approach, NoteData } from './common/note'

export function simLine(): SScript {
    const rightIndex = EntityMemory.to<number>(0)
    const leftIndex = Subtract(rightIndex, 1)

    const leftData = NoteData.of(leftIndex)
    const rightData = NoteData.of(rightIndex)

    const time = EntityMemory.to<number>(1)
    const spawnTime = EntityMemory.to<number>(2)

    const left = EntityMemory.to<number>(3)
    const right = EntityMemory.to<number>(4)

    const scale = EntityMemory.to<number>(5)
    const bottom = EntityMemory.to<number>(6)
    const top = EntityMemory.to<number>(7)

    const initialize = [
        time.set(NoteData.of(rightIndex).time),
        spawnTime.set(Subtract(time, noteOnScreenDuration)),

        left.set(leftData.center),
        right.set(rightData.center),
    ]

    const updateParallel = Or(
        And(options.isAutoplay, GreaterOr(Time, time)),
        Equal(EntityInfo.of(leftIndex).state, State.Despawned),
        Equal(EntityInfo.of(rightIndex).state, State.Despawned),
        And(GreaterOr(Time, spawnTime), [
            scale.set(
                approach(Divide(Subtract(Time, time), noteOnScreenDuration))
            ),
            bottom.set(Lerp(stageTop, noteBaseBottom, scale)),
            top.set(Lerp(stageTop, noteBaseTop, scale)),

            Draw(
                SkinSprite.SimultaneousConnectionNeutral,
                Multiply(left, noteBaseBottomScale, scale),
                bottom,
                Multiply(left, noteBaseTopScale, scale),
                top,
                Multiply(right, noteBaseTopScale, scale),
                top,
                Multiply(right, noteBaseBottomScale, scale),
                bottom,
                Layer.SimLine,
                1
            ),
        ])
    )

    return {
        initialize: {
            code: initialize,
        },
        updateParallel: {
            code: updateParallel,
        },
    }
}
