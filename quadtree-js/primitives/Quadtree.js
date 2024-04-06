class Quadtree {
    /**
     * @param {Rect} boundary Root bounding area
     * @param {number} maxObjects Max amount of objects within a branch before splitting
     * @param {number} maxDepth Max depth of branches
     * @param {Quadtree} parent Parent branch pointer; null for the root branch
     */
    constructor(boundary, maxObjects, maxDepth, parent) {
        this.boundary = boundary;
        this.maxObjects = maxObjects;
        this.maxDepth = maxDepth;
        /** @type {number} Current depth; 1 for the root branch */
        this.depth = parent ? parent.depth + 1 : 1;
        /** @type {Array<Object>} Leaves */
        this.objects = [];
        /** @type {boolean} */
        this.divided = false;
        /** @type {Array<Quadtree>} Pointers to branches; NW, NE, SW, SE */
        this.branches = [null, null, null, null];
    }

    /**
     * @param {Object} object 
     * @description Insert object recursively
     * If maxDepth reached, ignore maxObjects and push object anyways
     * Otherwise, descend until an undivided branch is found
     * Finally, push the object and split() if maxObjects reached
     */
    insert(object) {
        if (!this.boundary.contains(object)) return;

        if (this.depth == this.maxDepth) {
            this.objects.push(object);
        } else {
            if (this.divided) {
                for (const branch of this.branches) {
                    branch.insert(object);
                }
            } else {
                this.objects.push(object);
                if (this.objects.length > this.maxObjects) {
                    this.split();
                }
            }
        }
    }

    /**
     * @description Split current branch into four new branches
     * Return early if maxDepth already reached
     * Redistribute objects into the new branches
     */
    split() {
        if (this.depth > this.maxDepth) return;
        const x = this.boundary.x;
        const y = this.boundary.y;
        const w = this.boundary.width;
        const h = this.boundary.height;
        this.branches[0] = new Quadtree(new Rect(x - w/4, y - h/4, w/2, h/2), this.maxObjects, this.maxDepth, this);
        this.branches[1] = new Quadtree(new Rect(x + w/4, y - h/4, w/2, h/2), this.maxObjects, this.maxDepth, this);
        this.branches[2] = new Quadtree(new Rect(x - w/4, y + h/4, w/2, h/2), this.maxObjects, this.maxDepth, this);
        this.branches[3] = new Quadtree(new Rect(x + w/4, y + h/4, w/2, h/2), this.maxObjects, this.maxDepth, this);

        for (const object of this.objects) {
            for (const branch of this.branches) {
                if (branch.boundary.contains(object)) {
                    branch.insert(object);
                }
            }
        }

        this.objects = [];
        this.divided = true;
    }

    /**
     * @param {Rect} range Area to search
     * @param {Array<Object>} found Objects found thus far
     * @returns {Array<Object>} All objects found within the branches intersecting with the given range
     * @description Find objects recursively given a search range
     */
    query(range, found) {
        if (!found) found = [];

        if (!this.boundary.intersects(range)) return found;

        for (const object of this.objects) {
            if (range.intersects(object.getBoundingRect())) {
                found.push(object);
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
        this.boundary.draw();
        if (this.divided) {
            for (const branch of this.branches) {
                branch.draw();
            }
        }
    }
}