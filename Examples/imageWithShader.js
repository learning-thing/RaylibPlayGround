var camera;

var x;
var y;
var shader;
var iTime = 0;
var blockCol = {r: 255, g: 255, b: 255, a: 255};
var WHITE = {r: 255, g: 255, b: 255, a: 255};
var PINK = {r: 255, g: 10, b: 255, a: 255};
var BLACK = {r: 0, g: 0, b: 0, a: 255};
var delta;
var myUniform;

function onStart() {
    print("Starting");
    iTime = 0;
    distance = 50;
    _distance = 0;
    rotation = 0;
    _rotation = 0;
    delta = .016;
    yaw = 0;
    SetTargetFPS(100);
}


function onReady() {
    //SetTargetFPS(100);
    Print("Ready");
    shader = LoadShader("Shader/pointsVS.glsl", "Shader/pointsFS.glsl");
    myUniform = GetUniformLocation(shader, "iTime");
    camera = {
        position: {x: 10, y: 20, z: 30},
        target: {x: 0, y: 0, z: 0},
        up: {x: 0, y: 1, z: 0},
        fovy: 60,
    }
    SetWindowTitle("Shader fun");
    x = 10;
    y = 10;
}

Print("Hot reload");
//myUniform = GetUniformLocation(shader, "iTime");

var velocity = {x: 0, y: 0};
var moveSpeed = 200;

function onFrame() {
    delta = GetFrameTime();
    iTime += delta;

    if (y < GetScreenHeight()-100) {
        velocity.y+=.1;
    } else {
        y = GetScreenHeight()-100;
        velocity.y = 0;
    }

    x+=velocity.x*delta;
    y+=velocity.y*delta;
    SetUniform(shader, myUniform, iTime);

    if (IsKeyPressed("up"))    { velocity.y = -100; }
    if (IsKeyDown("down"))  { y+=delta*moveSpeed; }
    if (IsKeyDown("right")) { x+=delta*moveSpeed; }
    if (IsKeyDown("left"))  { x-=delta*moveSpeed; }

    //print(iTime);
    BeginDrawing();
        ClearBackground({r: 50, g: 50, b: 50, a: 255});

        BeginShader(shader);
            DrawImage("zaynelpixel.png", x, y, 100, 100);
        EndShader();
    DrawFPS(GetScreenWidth()-100, 10);
    EndDrawing();
}
