class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    draw(ctx) {
        ctx.beginPath();
        ctx.arc(this.x, this.y, 5, 0, Math.PI * 2);
        ctx.fillStyle = '#FF0000';
        ctx.fill();
    }
}

class Rect {
    constructor(x, y, width, height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    contains(point) {
        return point.x > this.x - this.width &&
               point.x < this.x + this.width &&
               point.y > this.y - this.height &&
               point.y < this.y + this.height;
    }
}

class Quadtree {
    constructor(boundary, capacity) {
        this.boundary = boundary;
        this.capacity = capacity;
        this.points = [];
        this.divided = false;
        this.branches = [null, null, null, null]; // NW, NE, SW, SE
    }

    insert(point) {
        if (!this.boundary.contains(point)) return;

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

    subdivide() {
        const x = this.boundary.x;
        const y = this.boundary.y;
        const w = this.boundary.width;
        const h = this.boundary.height;
        this.branches[0] = new Quadtree(new Rect(x - w/2, y - h/2, w/2, h/2), this.capacity);
        this.branches[1] = new Quadtree(new Rect(x + w/2, y - h/2, w/2, h/2), this.capacity);
        this.branches[2] = new Quadtree(new Rect(x - w/2, y + h/2, w/2, h/2), this.capacity);
        this.branches[3] = new Quadtree(new Rect(x + w/2, y + h/2, w/2, h/2), this.capacity);

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
    
    draw(ctx) {
        const x = this.boundary.x;
        const y = this.boundary.y;
        const w = this.boundary.width;
        const h = this.boundary.height;
        ctx.strokeRect(x - w, y - h, w*2, h*2);

        for (const point of this.points) {
            point.draw(ctx);
        }

        if (this.divided) {
            for (const branch of this.branches) {
                branch.draw(ctx);
            }
        }
    }
}