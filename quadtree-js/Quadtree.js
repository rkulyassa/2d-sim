class Point {
    constructor(x, y, radius) {
        this.x = x;
        this.y = y;
        this.radius = radius;
        // this.dx = Math.random() * cellVelocity*2 - cellVelocity;
        // this.dy = Math.sign(this.dx) * (1 - this.dx**2);
        this.dx = 0;
        this.dy = 0;
    }

    getBoundingRect() {
        return new Rect(this.x, this.y, this.radius*2, this.radius*2);
    }

    update() {
        this.dx = canvas.width/2 - this.x;
        this.dy = canvas.height/2 - this.y;
        this.x += this.dx * acceleration;
        this.y += this.dy * acceleration;
        // this.dy += 9.8;
        // this.x += this.dx;
        // this.y += this.dy;

        if (this.x - this.radius < 0) {
            this.x = this.radius;
            this.dx = -this.dx;
        }
        if (this.x + this.radius > canvas.width) {
            this.x = canvas.width - this.radius;
            this.dx = -this.dx;
        }
        if (this.y - this.radius < 0) {
            this.y = this.radius;
            this.dy = -this.dy;
        }
        if (this.y + this.radius > canvas.height) {
            this.y = canvas.height - this.radius;
            this.dy = -this.dy;// * 0.65;
        }
    }

    draw() {
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
        ctx.fillStyle = '#FF0000';
        ctx.fill();
    }
}

class Rect {
    constructor(x, y, w, h) {
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
    }

    contains(point) {
        return point.x >= this.x - this.w &&
               point.x <= this.x + this.w &&
               point.y >= this.y - this.h &&
               point.y <= this.y + this.h;
    }

    intersects(other) {
        return this.x - this.w <= other.x + other.w &&
               this.x + this.w >= other.x - other.w &&
               this.y - this.h <= other.y + other.h &&
               this.y + this.h >= other.y - other.h;

    }
}

class Quadtree {
    constructor(boundary, capacity, maxDepth, parent) {
        this.boundary = boundary;
        this.capacity = capacity;
        this.maxDepth = maxDepth;
        this.depth = parent ? parent.depth + 1 : 1;
        this.points = [];
        this.divided = false;
        this.branches = [null, null, null, null]; // NW, NE, SW, SE
    }

    insert(point) {
        if (!this.boundary.contains(point)) return;

        if (this.depth > this.maxDepth) {
            this.points.push(point);
        } else {
            if (this.points.length < this.capacity && !this.divided) {
                this.points.push(point);
            } else {
                if (!this.divided) {
                    this.subdivide();
                }
                for (const branch of this.branches) {
                    branch.insert(point);
                }
            }
        }
    }

    subdivide() {
        if (this.depth > this.maxDepth) return;
        const x = this.boundary.x;
        const y = this.boundary.y;
        const w = this.boundary.w;
        const h = this.boundary.h;
        this.branches[0] = new Quadtree(new Rect(x - w/2, y - h/2, w/2, h/2), this.capacity, this.maxDepth, this);
        this.branches[1] = new Quadtree(new Rect(x + w/2, y - h/2, w/2, h/2), this.capacity, this.maxDepth, this);
        this.branches[2] = new Quadtree(new Rect(x - w/2, y + h/2, w/2, h/2), this.capacity, this.maxDepth, this);
        this.branches[3] = new Quadtree(new Rect(x + w/2, y + h/2, w/2, h/2), this.capacity, this.maxDepth, this);

        for (const point of this.points) {
            for (const branch of this.branches) {
                if (branch.boundary.contains(point)) {
                    branch.insert(point);
                }
            }
        }

        this.points = [];
        this.divided = true;
    }

    query(range, found) {
        if (!found) found = [];

        if (!this.boundary.intersects(range)) return found;

        for (const point of this.points) {
            if (range.contains(point)) {
                found.push(point);
            }
        }

        if (this.divided) {
            for (const branch of this.branches) {
                branch.query(range, found);
            }
        }

        return found
    }
    
    draw() {
        const x = this.boundary.x;
        const y = this.boundary.y;
        const w = this.boundary.w;
        const h = this.boundary.h;
        ctx.lineWidth = 1;
        ctx.strokeRect(x - w, y - h, w*2, h*2);

        for (const point of this.points) {
            point.draw();
        }

        if (this.divided) {
            for (const branch of this.branches) {
                branch.draw();
            }
        }
    }
}