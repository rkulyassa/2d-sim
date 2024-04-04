const canvas = document.getElementsByTagName('canvas')[0];
const ctx = canvas.getContext('2d', { alpha: false });
// const dpr = window.devicePixelRatio;
// const rect = canvas.getBoundingClientRect();
// canvas.width = rect.width * dpr;
// canvas.height = rect.height * dpr;

const cellVelocity = 2;
const capacity = 4;
const acceleration = 0.1;
const maxDepth = 5;

let points = [];

function addPoints(n) {
    for (let i = 0; i < n; i++) {
        const point = new Point(Math.random() * canvas.width, Math.random() * canvas.height, Math.floor(Math.random()*20+50));
        points.push(point);
    }
}

function resolveCollision(a, b) {
    const dx = b.x - a.x;
    const dy = b.y - a.y;
    const d = Math.sqrt(dx*dx + dy*dy);
    // console.log(d, a.radius, b.radius);
    if (d >= a.radius + b.radius) return;
    const overlap = a.radius + b.radius - d;

    // console.log(a.radius, b.radius, d, lerp);
    // const M = a.getBoundingRect().area + b.getBoundingRect().area;
    a.x -= dx/d * overlap/2;// * b.getBoundingRect().area/M;
    a.y -= dy/d * overlap/2;// * b.getBoundingRect().area/M;
    b.x += dx/d * overlap/2;// * a.getBoundingRect().area/M;
    b.y += dy/d * overlap/2;// * a.getBoundingRect().area/M;


}

let quadtree;

function render() {
    requestAnimationFrame(render);

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    quadtree = new Quadtree(new Rect(canvas.width/2, canvas.height/2, canvas.width/2, canvas.height/2), capacity, maxDepth);
    for (const point of points) {
        quadtree.insert(point);
    }
    for (const a of points) {
        // for (let i = 0; i < 80; i++) {
            const pointsResult = quadtree.query(a.getBoundingRect());
            // console.log(pointsResult);
            for (const b of pointsResult) {
                if (a === b) continue;
                resolveCollision(a, b);
            }
        // }
    }
    for (const a of points) {
        a.update();
    }
    quadtree.draw(ctx);
    for (const point of points) {
        point.draw();
        point.getBoundingRect().draw();
    }
    // setTimeout(() => {
    //     requestAnimationFrame(step);
    // }, 200);
}

render();

// ctx.scale(dpr, dpr);
// canvas.style.width = `${rect.width}px`;
// canvas.style.height = `${rect.height}px`;