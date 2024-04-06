class Rect {
    constructor(x, y, width, height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    contains(point) {
        return point.x >= this.x - this.width/2 &&
               point.x <= this.x + this.width/2 &&
               point.y >= this.y - this.height/2 &&
               point.y <= this.y + this.height/2;
    }

    intersects(other) {
        return this.x - this.width/2 < other.x + other.width/2 &&
               this.x + this.width/2 > other.x - other.width/2 &&
               this.y - this.height/2 < other.y + other.height/2 &&
               this.y + this.height/2 > other.y - other.height/2;
    }

    draw() {
        ctx.strokeStyle = '#000000';
        ctx.lineWidth = 1;
        ctx.strokeRect(this.x - this.width/2, this.y - this.height/2, this.width, this.height);
    }
}