import { Add, Code, Multiply, Subtract } from 'sonolus.js'

export function getDistanceSquared(
    x1: Code<number>,
    y1: Code<number>,
    x2: Code<number>,
    y2: Code<number>
) {
    return Add(
        Multiply(Subtract(x1, x2), Subtract(x1, x2)),
        Multiply(Subtract(y1, y2), Subtract(y1, y2))
    )
}

export type Direction = 'up' | 'down' | 'left' | 'right'
export type Rect = [
    Code<number>,
    Code<number>,
    Code<number>,
    Code<number>,
    Code<number>,
    Code<number>,
    Code<number>,
    Code<number>
]

export function rotate(r: Rect, direction: Direction): Rect {
    switch (direction) {
        case 'up':
            return r
        case 'down':
            return [r[4], r[5], r[6], r[7], r[0], r[1], r[2], r[3]]
        case 'left':
            return [r[6], r[7], r[0], r[1], r[2], r[3], r[4], r[5]]
        case 'right':
            return [r[2], r[3], r[4], r[5], r[6], r[7], r[0], r[1]]
    }
}

export function rectByEdge(
    left: Code<number>,
    right: Code<number>,
    bottom: Code<number>,
    top: Code<number>,
    direction?: Direction
): Rect {
    if (direction) {
        return rotate(rectByEdge(left, right, bottom, top), direction)
    } else {
        return [left, bottom, left, top, right, top, right, bottom]
    }
}

export function rectBySize(
    x: Code<number>,
    y: Code<number>,
    w: Code<number>,
    h: Code<number>,
    direction: Direction = 'up'
): Rect {
    return rectByEdge(Subtract(x, w), Add(x, w), Subtract(y, h), Add(y, h), direction)
}
