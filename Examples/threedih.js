var camera = {
    position: {x: 10, y: 20, z: 30},
    target: {x: 0, y: 0, z: 0},
    up: {x: 0, y: 1, z: 0},
    fovy: 60,
}

var blockPos = {x: 6, y: 3, z: 8};

var iTime;
var blockCol = {r: 255, g: 255, b: 255, a: 255};
var WHITE = {r: 255, g: 255, b: 255, a: 255};
var BLACK = {r: 0, g: 0, b: 0, a: 255};

var distance;
var _distance;
var rotation;
var _rotation;
var delta;

function onStart() {
    print("Starting");
    iTime = 0;
    distance = 50;
    _distance = 0;
    rotation = 0;
    _rotation = 0;
    delta = .016;
    SetTargetFPS(100);

}

function onReady() {
    //SetTargetFPS(100);
    print("Ready");
}


print("Hot reload");


function onFrame() {
    delta = GetFrameTime();
    iTime += delta;
    if (IsKeyDown("up")) {
        distance-=delta*100;
    }
    if (IsKeyDown("down")) {
        distance+=delta*100;
    }
    if (IsKeyDown("right")) {
        rotation+=delta;
    }
    if (IsKeyDown("left")) {
        rotation-=delta;
    }
    _distance += (distance-_distance)*delta*2;
    _rotation += (rotation-_rotation)*delta*2;
    camera.position.x = sin(_rotation)*_distance;
    camera.position.z = cos(_rotation)*_distance;
    BeginDrawing();
    ClearBackground({r: 50, g: 50, b: 50, a: 255});
    DrawFPS(10, 10);
        BeginMode3D(camera);

        if (IsKeyDown("space")) {
            rotation-=GetMouseDeltaX()*.01;
            camera.position.y += GetMouseDeltaY()*.1;
        }


        //DrawGrid(100, 5);
        for (var y = 0; y < 20; y++) {
            for (var i = 0; i < 10; i++) {
                blockPos.y = sin(iTime+y*.5+i*.5)*2;
                blockPos.x = -20+3+6*i;
                blockPos.z = -30+3*y;
                DrawCube(blockPos, 3, 3, 3, (y % 2) ? WHITE : BLACK);
            }
            for (var i = 0; i < 10; i++) {
                blockPos.y = sin(iTime+y*.5+i*.5-.25)*2;
                blockPos.x = -20+6*i;
                DrawCube(blockPos, 3, 3, 3, (y % 2) ? BLACK : WHITE);
            }
        }
        EndMode3D();
    EndDrawing();
}
