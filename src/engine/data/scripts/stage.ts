import { SkinSprite } from 'sonolus-core'
import {
    Add,
    And,
    bool,
    Divide,
    Draw,
    EntityInfo,
    EntityMemory,
    Equal,
    Floor,
    GreaterOr,
    HasSkinSprite,
    If,
    Lerp,
    Less,
    LessOr,
    Multiply,
    Not,
    Or,
    Round,
    Script,
    State,
    Subtract,
    TouchStarted,
    TouchX,
} from 'sonolus.js'
import { options } from '../../configuration/options'
import {
    bandoriJudgmentLineHeight,
    BandoriJudgmentLineSprite,
    bandoriJudgmentLineWidth,
    bandoriStageBottom,
    BandoriStageSprite,
    bandoriStageTop,
    bandoriStageWidth,
    halfBaseNoteHeight,
    laneBottom,
    laneTop,
    laneWidth,
    Layer,
    screenLeft,
    screenRight,
    screenTop,
} from './common/constants'
import { playLaneEffect, playSlotEffect } from './common/effect'
import { playStageSFX } from './common/sfx'
import {
    getLaneBottomCenter,
    getLaneBottomLeft,
    getLaneBottomRight,
    getLaneTopLeft,
    getLaneTopRight,
} from './common/stage'
import { checkTouchYInHitbox, isTouchOccupied } from './common/touch'
import { rectByEdge, rectBySize } from './common/utils'

export function stage(): Script {
    const spawnOrder = -999

    const shouldSpawn = Equal(EntityInfo.of(0).state, State.Despawned)

    const touch = Or(options.isAutoplay, [
        And(
            TouchStarted,
            checkTouchYInHitbox(),
            Not(isTouchOccupied),
            GreaterOr(TouchX, getLaneBottomLeft(-3)),
            LessOr(TouchX, getLaneBottomRight(3)),
            onEmptyTap()
        ),
        isTouchOccupied.set(false),
    ])

    const updateParallel = [drawStageCover(), drawStage()]

    return {
        spawnOrder,
        shouldSpawn,
        touch: {
            code: touch,
            order: 1,
        },
        updateParallel,
    }

    function drawStageCover() {
        const stageCoverBottom = Lerp(laneTop, laneBottom, options.stageCover)

        return And(
            bool(options.stageCover),
            Draw(
                SkinSprite.StageCover,
                ...rectByEdge(screenLeft, screenRight, stageCoverBottom, screenTop),
                Layer.Cover,
                1
            )
        )
    }

    function drawStage() {
        const halfJudgmentLineHeight = Divide(halfBaseNoteHeight, 2)

        const borderWidth = 0.25

        const halfSlotSize = Multiply(halfJudgmentLineHeight, 0.85)

        return [
            If(
                HasSkinSprite(BandoriJudgmentLineSprite),
                Draw(
                    BandoriJudgmentLineSprite,
                    ...rectBySize(
                        0,
                        laneBottom,
                        Multiply(bandoriJudgmentLineWidth, 0.5),
                        Multiply(bandoriJudgmentLineHeight, 0.5)
                    ),
                    Layer.JudgmentLine,
                    1
                ),
                [
                    Draw(
                        SkinSprite.JudgmentLine,
                        ...rectByEdge(
                            screenLeft,
                            screenRight,
                            Subtract(laneBottom, halfJudgmentLineHeight),
                            Add(laneBottom, halfJudgmentLineHeight)
                        ),
                        Layer.JudgmentLine,
                        1
                    ),
                    [...Array(7).keys()].map((i) =>
                        Draw(
                            SkinSprite.NoteSlot,
                            ...rectBySize(
                                getLaneBottomCenter(i - 3),
                                laneBottom,
                                halfSlotSize,
                                halfSlotSize
                            ),
                            Layer.Slot,
                            0.5
                        )
                    ),
                ]
            ),

            If(
                HasSkinSprite(BandoriStageSprite),
                Draw(
                    BandoriStageSprite,
                    ...rectByEdge(
                        Multiply(bandoriStageWidth, -0.5),
                        Multiply(bandoriStageWidth, 0.5),
                        bandoriStageBottom,
                        bandoriStageTop
                    ),
                    Layer.Stage,
                    1
                ),
                [
                    Draw(
                        SkinSprite.StageLeftBorder,
                        getLaneBottomLeft(-3 - borderWidth),
                        laneBottom,
                        getLaneTopLeft(-3 - borderWidth),
                        laneTop,
                        getLaneTopLeft(-3),
                        laneTop,
                        getLaneBottomLeft(-3),
                        laneBottom,
                        Layer.Stage,
                        1
                    ),
                    Draw(
                        SkinSprite.StageRightBorder,
                        getLaneBottomRight(3),
                        laneBottom,
                        getLaneTopRight(3),
                        laneTop,
                        getLaneTopRight(3 + borderWidth),
                        laneTop,
                        getLaneBottomRight(3 + borderWidth),
                        laneBottom,
                        Layer.Stage,
                        1
                    ),

                    [...Array(7).keys()].map((i) =>
                        Draw(
                            i % 2 ? SkinSprite.Lane : SkinSprite.LaneAlternative,
                            getLaneBottomLeft(i - 3),
                            laneBottom,
                            getLaneTopLeft(i - 3),
                            laneTop,
                            getLaneTopRight(i - 3),
                            laneTop,
                            getLaneBottomRight(i - 3),
                            laneBottom,
                            Layer.Stage,
                            1
                        )
                    ),
                ]
            ),
        ]
    }

    function onEmptyTap() {
        const lane = EntityMemory.to<number>(0)

        return [
            lane.set(Floor(Divide(TouchX, laneWidth))),
            If(
                Less(lane, 3),
                playStageSFX(),
                And()
            ),
            playLaneEffect(lane),
            playSlotEffect(lane),
        ]
    }
}
