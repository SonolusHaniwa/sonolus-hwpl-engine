import { Add, Code, Multiply, Subtract } from 'sonolus.js'
import { halfNoteWidth, laneWidth } from './constants'

export function getLaneTopLeft(lane: Code<number>) {
    return Multiply(0.05, laneWidth, lane)
}

export function getLaneTopRight(lane: Code<number>) {
    return Multiply(0.05, laneWidth, Add(lane, 1))
}

export function getLaneBottomLeft(lane: Code<number>) {
    return Multiply(laneWidth, lane)
}

export function getLaneBottomRight(lane: Code<number>) {
    return Multiply(laneWidth, Add(lane, 1))
}

export function getLaneBottomCenter(lane: Code<number>) {
    return Add(Multiply(laneWidth, lane), halfNoteWidth)
}
