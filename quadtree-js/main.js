// Physics
const gravity = 0.1;
const cellVelocity = 2;
const circleSizes = [2, 4, 6];

// Quadtree
const maxObjects = 8;
const maxDepth = 5;

// Rendering/computation
const collisionIterations = 4;
const substeps = 8;
const circleBorderSize = 2;
const canvasCenterPointSize = 5;
const canvas = document.getElementsByTagName('canvas')[0];
const ctx = canvas.getContext('2d');
let fpsUpdateOffset = 5; // frames at which fps updates, for readability
let initialFrameTime;
let fps;

// Input
let worldType;
let quadtreeEnabled;
let collisionsEnabled;
let circleBordersEnabled;
let canvasCenterEnabled;

// Misc
let quadtree;
let circles = [];

function reset() {
    circles = [];
}

/**
 * @param {Circle} a 
 * @param {Circle} b 
 * @description
 * Separate circles along a normalized difference vector
 * Scale by the amount of overlap to the intersection midpoint and the "mass" of both circles
 */
function resolveCollision(a, b) {
    const dx = b.x - a.x;
    const dy = b.y - a.y;
    const d = Math.sqrt(dx*dx + dy*dy);

    if (d >= a.radius + b.radius) return;
    const overlap = (a.radius + b.radius - d)/2;

    const m = a.squareSize + b.squareSize;
    a.x -= dx/d * overlap * b.squareSize/m;
    a.y -= dy/d * overlap * b.squareSize/m;
    b.x += dx/d * overlap * a.squareSize/m;
    b.y += dy/d * overlap * a.squareSize/m;
}

/**
 * @param {number} n
 */
function addCircles(n) {
    for (let i = 0; i < n; i++) {
        const circle = new Circle(Math.random() * canvas.width, Math.random() * canvas.height, circleSizes[Math.floor(Math.random() * circleSizes.length)]);//Math.floor(Math.random()*20+50));
        circles.push(circle);
    }
}

function tick() {
    // update circles motion
    for (const circle of circles) {
        circle.update();
    }

    // create quadtree and insert all circles
    if (quadtreeEnabled) {
        quadtree = new Quadtree(new Rect(canvas.width/2, canvas.height/2, canvas.width, canvas.height), maxObjects, maxDepth);
        for (const circle of circles) {
            quadtree.insert(circle);
        }
    }

    // separate colliding circles. uses quadtree query for O(n*log(n)) time complexity, else n^2 algorithm
    if (collisionsEnabled) {
        let collisionChecksCount = 0;
        for (let i = 0; i < collisionIterations; i++) {
            for (const a of circles) {
                let compareArray = quadtreeEnabled ? quadtree.query(a.getBoundingRect()) : circles;
                for (const b of compareArray) {
                    if (a === b) continue;
                    resolveCollision(a, b);
                    collisionChecksCount += 1;
                }
            }
        }
        document.getElementById('collisionChecksCount').innerText = `Collision checks count: ${collisionChecksCount}`;
    }
}

function render() {
    // init fps counter
    if (!initialFrameTime) {
        initialFrameTime = performance.now();
        fps = 0;
    }

    // clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = '#000000';
    ctx.lineWidth = 1;
    ctx.strokeRect(0, 0, canvas.width, canvas.height);

    // update inputs
    worldType = parseInt(document.getElementById('worldSelect').value);
    quadtreeEnabled = document.getElementById('quadtreeEnabled').checked;
    collisionsEnabled = document.getElementById('collisionsEnabled').checked;
    circleBordersEnabled = document.getElementById('circleBordersEnabled').checked;
    canvasCenterEnabled = document.getElementById('canvasCenterEnabled').checked;

    // process physics
    for (let i = 0; i < substeps; i++) {
        tick();
    }

    // draw to canvas
    if (quadtreeEnabled) {
        quadtree.draw();
    }
    for (const circle of circles) {
        circle.draw();
    }
    if (canvasCenterEnabled) {
        ctx.fillStyle = '#000000';
        ctx.fillRect(canvas.width/2 - canvasCenterPointSize/2, canvas.height/2 - canvasCenterPointSize/2, canvasCenterPointSize, canvasCenterPointSize);
    }
    document.getElementById('circleCount').innerText = `Circle count: ${circles.length}`;

    // calculate fps
    const delta = (performance.now() - initialFrameTime)/1000;
    initialFrameTime = performance.now();
    fps = Math.floor(1/delta);
    if (fpsUpdateOffset > 0) {
       fpsUpdateOffset -= 1;
    } else {
       document.getElementById('fps').innerText = `FPS: ${fps}`;
       fpsUpdateOffset = 10;
    }

    requestAnimationFrame(render);
}

render();