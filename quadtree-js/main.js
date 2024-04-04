const canvas = document.getElementsByTagName('canvas')[0];
const ctx = canvas.getContext('2d', { alpha: false });
const dpr = window.devicePixelRatio;
const rect = canvas.getBoundingClientRect();
canvas.width = rect.width * dpr;
canvas.height = rect.height * dpr;

const CAPACITY = 4;
const POINTS = 1000;

const quadtree = new Quadtree(new Rect(canvas.width/2, canvas.width/2, canvas.width/2, canvas.width/2), CAPACITY);

for (let i = 0; i < POINTS; i++) {
    const point = new Point(Math.random()*canvas.width, Math.random()*canvas.height);
    quadtree.insert(point);
}
quadtree.draw(ctx);

function addPoint(x, y) {
    const point = new Point(x, y);
    quadtree.insert(point);
    quadtree.draw(ctx);
}

function addPoints(n) {
    for (let i = 0; i < n; i++) {
        const point = new Point(Math.random()*canvas.width, Math.random()*canvas.height);
        quadtree.insert(point);
        quadtree.draw(ctx);
    }
}

ctx.scale(dpr, dpr);
canvas.style.width = `${rect.width}px`;
canvas.style.height = `${rect.height}px`;