const canvas = document.getElementsByTagName('canvas')[0];
const ctx = canvas.getContext('2d', { alpha: false });
// const dpr = window.devicePixelRatio;
// const rect = canvas.getBoundingClientRect();
// canvas.width = rect.width * dpr;
// canvas.height = rect.height * dpr;

const cellVelocity = 2;
const capacity = 4;
const acceleration = 0.1;

let points = [];

function addPoints(n) {
    for (let i = 0; i < n; i++) {
        const point = new Point(Math.random() * canvas.width, Math.random() * canvas.height, Math.floor(Math.random()*10+10));
        points.push(point);
    }
}

function resolveCollision(a, b) {
    const dx = b.x - a.x;
    const dy = b.y - a.y;
    const d = Math.sqrt(dx*dx + dy*dy);
    if (d > a.radius + b.radius) return;
    const lerp = (a.radius + b.radius - d)/2;
    a.x -= dx/d * lerp;
    a.y -= dy/d * lerp;
    b.x += dx/d * lerp;
    b.y += dy/d * lerp;
}

let quadtree;

function step() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    quadtree = new Quadtree(new Rect(canvas.width/2, canvas.width/2, canvas.width/2, canvas.width/2), capacity, 5, null);
    for (const point of points) {
        quadtree.insert(point);
    }
    for (const a of points) {
        const pointsResult = quadtree.query(a.getBoundingRect(), null);
        // console.log(pointsResult);
        // console.log(a.getBoundingRect());
        // console.log(pointsResult);
        for (const b of pointsResult) {
            if (a === b) continue;
            for (let i = 0; i < 4; i++) {
                resolveCollision(a, b);
            }
            // point.update(this.rootBoundary);
        }
        a.update();
    }
    quadtree.draw(ctx);
    requestAnimationFrame(step);
}

requestAnimationFrame(step);

// ctx.scale(dpr, dpr);
// canvas.style.width = `${rect.width}px`;
// canvas.style.height = `${rect.height}px`;