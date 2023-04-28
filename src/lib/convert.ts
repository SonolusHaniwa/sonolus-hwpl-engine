import { LevelData, LevelDataEntity } from 'sonolus-core'
import { Archetype } from 'sonolus.js/dist'

export type ChartObject =
    | SingleObject
    | DirectionalObject
    | SlideObject
    | LongObject
    | BPMObject
    | SystemObject

type ObjectBase = {
    beat: number
}

export type NoteBase = ObjectBase & {
    lane: number
    flick?: true
    skill?: true
    charge?: true
}

export type SingleObject = NoteBase & {
    type: 'Single'
}

type DirectionalObject = NoteBase & {
    type: 'Directional'
    direction: 'Left' | 'Right'
    width: number
}

export type SlideObject = {
    type: 'Slide'
    connections: Connection[]
}

type LongObject = {
    type: 'Long'
    connections: Connection[]
}

type Connection = NoteBase & {
    hidden?: true
}

type BPMObject = ObjectBase & {
    type: 'BPM'
    bpm: number
}

type SystemObject = ObjectBase & {
    type: 'System'
    data: string
}

export function fromHanipure(chart: Array<Array<number>>, archetypes: {
    initializationIndex: number
    stageIndex: number
    tapNoteIndex: number
    flickNoteIndex: number
    directionalFlickNoteIndex: number
    slideStartNoteIndex: number
    slideTickNoteIndex: number
    slideEndNoteIndex: number
    slideFlickNoteIndex: number
    straightSliderIndex: number
    curvedSliderIndex: number
}): LevelData {
    let convertChart: ChartObject[] = new Array;
    convertChart.push({"type":"BPM","bpm":150,"beat":0});
    for (let i = 0; i < chart.length; i++) {
        let x: Array<number> = chart[i];
        if (x[0] == 1) {
            let xx: SingleObject = {
                beat: x[1] * 2,
                lane: x[2] - 1,
                type: "Single"
            }; convertChart.push(xx);
        } else if (x[0] == 2) {
            let xx: SingleObject = {
                beat: x[1] * 2,
                lane: x[2] - 1,
                type: "Single",
                flick: true
            }; convertChart.push(xx);
        } else if (x[0] == 11) {
            let xx: SlideObject = {
                type: "Slide",
                connections: [
                    {
                        beat: x[1] * 2,
                        lane: x[2] - 1
                    }
                ]
            };
            for (let j = 0; j < chart.length; j++) {
                if (chart[j][0] == 12 && chart[j][3] == x[3]) {
                    xx.connections.push({
                        beat: chart[j][1] * 2,
                        lane: chart[j][2] - 1
                    });
                } if (chart[j][0] == 13 && chart[j][3] == x[3]) {
                    xx.connections.push({
                        beat: chart[j][1] * 2,
                        lane: chart[j][2] - 1,
                        flick: true
                    });
                }
            } convertChart.push(xx);
        } else if (x[0] == 21) {
            let xx: SlideObject = {
                type: "Slide",
                connections: [
                    {
                        beat: x[1] * 2,
                        lane: x[2] - 1
                    }
                ]
            };
            for (let j = 0; j < chart.length; j++) {
                if (chart[j][0] == 22 && chart[j][3] == x[3]) {
                    xx.connections.push({
                        beat: chart[j][1] * 2,
                        lane: chart[j][2] - 1
                    });
                } if (chart[j][0] == 23 && chart[j][3] == x[3]) {
                    xx.connections.push({
                        beat: chart[j][1] * 2,
                        lane: chart[j][2] - 1,
                    });
                } if (chart[j][0] == 24 && chart[j][3] == x[3]) {
                    xx.connections.push({
                        beat: chart[j][1] * 2,
                        lane: chart[j][2] - 1,
                        flick: true
                    });
                }
            } convertChart.push(xx);
        }
    }
    console.log(convertChart)
    return _fromBestdori(convertChart, archetypes)
}

export function _fromBestdori(
    chart: ChartObject[],
    archetypes: {
        initializationIndex: number
        stageIndex: number
        tapNoteIndex: number
        flickNoteIndex: number
        directionalFlickNoteIndex: number
        slideStartNoteIndex: number
        slideTickNoteIndex: number
        slideEndNoteIndex: number
        slideFlickNoteIndex: number
        straightSliderIndex: number
        curvedSliderIndex: number
    }
): LevelData {
    type WrappedNoteEntity = {
        entity: LevelDataEntity
        time: number
        lane: number
        head?: WrappedNoteEntity
    }

    type WrappedSliderEntity = {
        entity: LevelDataEntity
        head: WrappedNoteEntity
        tail: WrappedNoteEntity
    }

    const chartObjects = repair(chart)

    const timings: {
        beat: number
        time: number
        timePerBeat: number
    }[] = []

    let timePerBeat = 0
    let beats = 0
    let time = 0
    chartObjects
        .filter((chartObject): chartObject is BPMObject => chartObject.type === 'BPM')
        .sort((a, b) => a.beat - b.beat)
        .forEach((bpmObject) => {
            time += (bpmObject.beat - beats) * timePerBeat
            beats = bpmObject.beat
            timePerBeat = 60 / bpmObject.bpm
            timings.unshift({
                beat: beats,
                time,
                timePerBeat,
            })
        })

    const wrappedNoteEntities: WrappedNoteEntity[] = [
        {
            entity: {
                archetype: archetypes.initializationIndex,
            },
            time: Number.NEGATIVE_INFINITY,
            lane: 0,
        },
        {
            entity: {
                archetype: archetypes.stageIndex,
            },
            time: Number.NEGATIVE_INFINITY,
            lane: 0,
        },
    ]
    const wrappedSliderEntities: WrappedSliderEntity[] = []

    chartObjects.forEach((chartObject) => {
        switch (chartObject.type) {
            case 'Single': {
                const time = beatToTime(chartObject.beat)
                const lane = chartObject.lane - 3
                wrappedNoteEntities.push({
                    entity: {
                        archetype: chartObject.flick
                            ? archetypes.flickNoteIndex
                            : archetypes.tapNoteIndex,
                        data: {
                            index: 1,
                            values: [time, lane],
                        },
                    },
                    time,
                    lane,
                })
                break
            }
            case 'Directional': {
                const time = beatToTime(chartObject.beat)
                const lane = chartObject.lane - 3
                wrappedNoteEntities.push({
                    entity: {
                        archetype: archetypes.directionalFlickNoteIndex,
                        data: {
                            index: 1,
                            values: [
                                time,
                                lane,
                                chartObject.width - 1,
                                chartObject.direction === 'Left' ? 1 : 0,
                            ],
                        },
                    },
                    time,
                    lane,
                })
                break
            }
            case 'Slide':
            case 'Long': {
                let segments: {
                    time: number
                    lane: number
                }[] = []

                const hasHidden = chartObject.connections.some((connection) => connection.hidden)
                const isLong =
                    chartObject.connections.length === 2 &&
                    chartObject.connections[0].lane === chartObject.connections[1].lane
                        ? 1
                        : 0

                chartObject.connections.forEach((connection, index) => {
                    const time = beatToTime(connection.beat)
                    const lane = connection.lane - 3
                    if (connection.hidden) {
                        segments.push({
                            time,
                            lane,
                        })
                    } else {
                        if (index === 0) {
                            wrappedNoteEntities.push({
                                entity: {
                                    archetype: archetypes.slideStartNoteIndex,
                                    data: {
                                        index: 1,
                                        values: [time, lane],
                                    },
                                },
                                time,
                                lane,
                            })
                        } else {
                            const head = wrappedNoteEntities[wrappedNoteEntities.length - 1]
                            if (index === chartObject.connections.length - 1) {
                                wrappedNoteEntities.push({
                                    entity: {
                                        archetype: connection.flick
                                            ? archetypes.slideFlickNoteIndex
                                            : archetypes.slideEndNoteIndex,
                                        data: {
                                            index: 0,
                                            values: [0, time, lane, 0, 0, isLong],
                                        },
                                    },
                                    time,
                                    lane,
                                    head,
                                })
                            } else {
                                wrappedNoteEntities.push({
                                    entity: {
                                        archetype: archetypes.slideTickNoteIndex,
                                        data: {
                                            index: 0,
                                            values: [0, time, lane],
                                        },
                                    },
                                    time,
                                    lane,
                                    head,
                                })
                            }
                            segments.push({
                                time,
                                lane,
                            })

                            const tail = wrappedNoteEntities[wrappedNoteEntities.length - 1]
                            let prevSegment = {
                                time: head.time,
                                lane: head.lane,
                            }
                            segments.forEach((segment) => {
                                wrappedSliderEntities.push({
                                    entity: {
                                        archetype: hasHidden
                                            ? archetypes.curvedSliderIndex
                                            : archetypes.straightSliderIndex,
                                        data: {
                                            index: 0,
                                            values: [
                                                0,
                                                0,
                                                prevSegment.time,
                                                segment.time,
                                                prevSegment.lane,
                                                segment.lane,
                                            ],
                                        },
                                    },
                                    head,
                                    tail,
                                })
                                prevSegment = segment
                            })
                            segments = []
                        }
                    }
                })
                break
            }
        }
    })

    return {
        bgmOffset: 0,
        entities: [
            ...wrappedNoteEntities
                .sort((a, b) => a.time - b.time || Math.abs(b.lane) - Math.abs(a.lane))
                .map((wrappedNoteEntity) => {
                    if (wrappedNoteEntity.head) {
                        if (!wrappedNoteEntity.entity.data) throw 'Unexpected missing entity data'

                        wrappedNoteEntity.entity.data.values[0] = wrappedNoteEntities.indexOf(
                            wrappedNoteEntity.head
                        )
                    }
                    return wrappedNoteEntity.entity
                }),
            ...wrappedSliderEntities.map((wrappedSliderEntity) => {
                if (!wrappedSliderEntity.entity.data) throw 'Unexpected missing entity data'

                wrappedSliderEntity.entity.data.values[0] = wrappedNoteEntities.indexOf(
                    wrappedSliderEntity.head
                )
                wrappedSliderEntity.entity.data.values[1] = wrappedNoteEntities.indexOf(
                    wrappedSliderEntity.tail
                )
                return wrappedSliderEntity.entity
            }),
        ],
    }

    function beatToTime(beat: number) {
        for (const timing of timings) {
            if (beat >= timing.beat) {
                return timing.time + (beat - timing.beat) * timing.timePerBeat
            }
        }
        return 0
    }
}

function repair(chartObjects: ChartObject[]) {
    chartObjects.forEach((chartObject) => {
        switch (chartObject.type) {
            case 'Long':
            case 'Slide': {
                chartObject.connections.sort((a, b) => a.beat - b.beat)
                const visibleConnections = chartObject.connections.filter(
                    (connection) => !connection.hidden
                )
                switch (visibleConnections.length) {
                    case 0:
                        remove(chartObject)
                        break
                    case 1: {
                        const connection = visibleConnections[0]
                        replace(chartObject, {
                            type: 'Single',
                            lane: connection.lane,
                            beat: connection.beat,
                            flick: connection.flick,
                        })
                        break
                    }
                    default:
                        while (chartObject.connections[0]?.hidden) {
                            chartObject.connections.shift()
                        }
                        while (
                            chartObject.connections[chartObject.connections.length - 1]?.hidden
                        ) {
                            chartObject.connections.pop()
                        }
                        break
                }
                break
            }
        }
    })

    return chartObjects

    function replace(o: ChartObject, n: ChartObject) {
        chartObjects.splice(chartObjects.indexOf(o), 1, n)
    }

    function remove(o: ChartObject) {
        chartObjects.splice(chartObjects.indexOf(o), 1)
    }
}
