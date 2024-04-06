class Circle {
    constructor(x, y, radius) {
        this.x = x;
        this.y = y;
        this.radius = radius;
        this.dx = this.dy = 0;
        this.squareSize = (radius*2)**2;
        this.seeds = [
            [-1,1][Math.round(Math.random())], // dx sign
            [-1,1][Math.round(Math.random())], // dy sign
            Math.random(), // dx
            Math.random()  // dy
        ];
    }

    update() {
        switch (worldType) {
            case 0: // Gravitate to center
                this.dx = canvas.width/2 - this.x;
                this.dy = canvas.height/2 - this.y;
                this.x += this.dx * gravity * 1/substeps;
                this.y += this.dy * gravity * 1/substeps;
                break;
            
            case 1: // Random pong motion
                this.dx = this.seeds[0] * this.seeds[2] * cellVelocity;
                this.dy = this.seeds[1] * this.seeds[3] * cellVelocity;
                this.x += this.dx * 1/substeps;
                this.y += this.dy * 1/substeps;
                this.resolveWallCollision();
                break;
            
            case 2: // No motion
                this.dx = 0;
                this.dy = 0;
                break;
        }
    }

    getBoundingRect() {
        return new Rect(this.x, this.y, this.radius*2, this.radius*2);
    }

    resolveWallCollision() {
        if (this.x - this.radius < 0) {
            this.x = this.radius;
            this.dx *= -1;
            this.seeds[0] *= -1;
        }
        if (this.x + this.radius > canvas.width) {
            this.x = canvas.width - this.radius;
            this.dx *= -1;
            this.seeds[0] *= -1;
        }
        if (this.y - this.radius < 0) {
            this.y = this.radius;
            this.dy *= -1;
            this.seeds[1] *= -1;
        }
        if (this.y + this.radius > canvas.height) {
            this.y = canvas.height - this.radius;
            this.dy *= -1;
            this.seeds[1] *= -1;
        }
    }

    draw() {
        ctx.beginPath();
        ctx.lineWidth = circleBorderSize;
        if (circleBordersEnabled) {
            ctx.arc(this.x, this.y, this.radius - ctx.lineWidth/2, 0, Math.PI * 2);
            ctx.strokeStyle = '#000000';
            ctx.stroke();
        } else {
            ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
        }
        ctx.fillStyle = '#FF0000';
        ctx.fill();
        ctx.closePath();
    }
}