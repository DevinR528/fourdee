// Daniel Shiffman
// http://youtube.com/thecodingtrain
// http://codingtra.in
// JavaScript transcription: Chuck England

// Coding Challenge #113: 4D Hypercube
// https://youtu.be/XE3YDVdQSPo

// Matrix Multiplication
// https://youtu.be/tzsgS19RRc8

let current_angleX = 0;
let current_angleY = 0;

let current_cameraX = 0;
let current_cameraY = 0.4;

var start_angleX, start_angleY;
var start_cameraX, start_cameraY;
var start_mouseX, start_mouseY;

function mousePressed() {
    switch (mouseButton)
    {
        case LEFT:
            start_angleX = current_angleX;
            start_angleY = current_angleY;
            break;
        case CENTER:
            start_cameraX = current_cameraX;
            start_cameraY = current_cameraY;
            break;
        default: break;
    }
    start_mouseX = mouseX;
    start_mouseY = mouseY;
    return false;
}

function mouseDragged() {
    switch (mouseButton)
    {
        case LEFT:
            current_angleX = start_angleX + map(mouseX - start_mouseX, 0, width, 0, -TWO_PI);
            current_angleY = start_angleY + map(mouseY - start_mouseY, 0, height, 0, +TWO_PI);
            break;
        case CENTER:
            current_cameraX = start_cameraX + map(mouseX - start_mouseX, 0, width, 0, +TWO_PI);
            current_cameraY = start_cameraY + map(mouseY - start_mouseY, 0, height, 0, +TWO_PI);
            break;
        default: break;
    }
}

function mouseReleased() {
    return false;
}

let points = [];

let divisions = 10;

function setup() {
    let size = min(windowWidth, windowHeight);
    createCanvas(size, size, WEBGL);

    smooth();
    
    let next = 0;
    
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, +1, -1), 1, 1, map(j, 0, divisions, 1, -1));
    
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(-1, 1, map(i, 0, divisions, +1, -1), map(j, 0, divisions, 1, -1));
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, -1, +1), 1, -1, map(j, 0, divisions, 1, -1));
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(+1, 1, map(i, 0, divisions, -1, +1), map(j, 0, divisions, 1, -1));
     
//     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, +1, -1), map(j, 0, divisions, 1, -1), 1, -1);
    
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(-1, map(j, 0, divisions, 1, -1), map(i, 0, divisions, +1, -1), -1);
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, -1, +1), map(j, 0, divisions, 1, -1), -1, -1);
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(+1, map(j, 0, divisions, 1, -1), map(i, 0, divisions, -1, +1), -1);
//     
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, +1, -1), -1, 1, map(j, 0, divisions, -1, 1));
    
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(-1, -1, map(i, 0, divisions, +1, -1), map(j, 0, divisions, -1, 1));
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, -1, +1), -1, -1, map(j, 0, divisions, -1, 1));
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(+1, -1, map(i, 0, divisions, -1, +1), map(j, 0, divisions, -1, 1));
     
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, +1, -1), map(j, 0, divisions, -1, 1), 1, 1);
    
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(-1, map(j, 0, divisions, -1, 1), map(i, 0, divisions, +1, -1), 1);
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(map(i, 0, divisions, -1, +1), map(j, 0, divisions, -1, 1), -1, 1);
     
    for (let j = 0; j < divisions; j++)
        for (let i = 0; i < divisions; i++)
            points[next++] = new P4Vector(+1, map(j, 0, divisions, -1, 1), map(i, 0, divisions, -1, +1), 1);
     
}

var point_history = [];

function draw() {
    background(0);
    scale(1, -1, 1, 1);
    rotateX(current_cameraY);
    rotateY(current_cameraX);
    let projected3d = [];


//    strokeWeight(10);
//    stroke(200, 200, 200);
//    point(0, 0, 0);
//    stroke(200, 0, 0);
//    point(10, 0, 0);
//    stroke(0, 200, 0);
//    point(0, 10, 0);
//    stroke(0, 0, 200);
//    point(0, 0, 10);
    
    var timeslice = [];
    
    for (let i = 0; i < points.length; i++) {
        const v = points[i];

//        const rotationXZ = [
//            [cos(current_angleX), 0, -sin(current_angleX), 0],
//            [0, 1, 0, 0],
//            [sin(current_angleX), 0, cos(current_angleX), 0],
//            [0, 0, 0, 1],
//        ];

        const rotationXW = [
            [cos(current_angleX), 0, 0, -sin(current_angleX)],
            [0, 1, 0, 0],
            [0, 0, 1, 0],
            [sin(current_angleX), 0, 0, cos(current_angleX)],
        ];

        const rotationYW = [
            [1, 0, 0, 0],
            [0, cos(current_angleY), 0, -sin(current_angleY)],
            [0, 0, 1, 0],
            [0, sin(current_angleY), 0, cos(current_angleY)]
        ];

        let rotated = matmul(rotationXW, v);
        rotated = matmul(rotationYW, rotated);

        let distance = 2;
        let w = 1 / (distance - rotated.w);

        const projection = [
            [w, 0, 0, 0],
            [0, w, 0, 0],
            [0, 0, w, 0],
        ];

        let projected = matmul(projection, rotated);
        projected.mult(width / 8);
        projected3d[i] = projected;
        
        switch ((i / (divisions ** 2)) % 8)
        {
            case 0: stroke(100, 100, 100); break;
            case 1: stroke(100, 100, 200); break;
            case 2: stroke(100, 200, 100); break;
            case 3: stroke(100, 200, 200); break;
            case 4: stroke(200, 100, 100); break;
            case 5: stroke(200, 100, 200); break;
            case 6: stroke(200, 200, 100); break;
            case 7: stroke(200, 200, 200); break;
        }
        
        //    strokeWeight(map(projected.z, -5, 5, 2, 5));
        strokeWeight(8);
//        noFill();
        timeslice.push(projected);
        point(projected.x, projected.y, projected.z);
    }
    
    while (point_history.length >= 10)
        point_history.shift();
    
    point_history.push(timeslice);
    
    strokeWeight(1);
    stroke(255);
    for (var i = 0, n = point_history.length - 1; i < n; i++)
    {
        var current_slice = point_history[i];
        var next_slice = point_history[i + 1];
        
        for (var j = 0, m = current_slice.length; j < m; j++)
        if (j % 4 == 0)
        {
            const a = current_slice[j];
            const b = next_slice[j];
            line(a.x, a.y, a.z, b.x, b.y, b.z);
        }
    }
}



















