var camera;

var blockPos;
var shader;
var iTime;
var blockCol = {r: 255, g: 255, b: 255, a: 255};
var WHITE = {r: 255, g: 255, b: 255, a: 255};
var PINK = {r: 255, g: 10, b: 255, a: 255};
var BLACK = {r: 0, g: 0, b: 0, a: 255};

var distance;
var _distance;
var rotation;
var _rotation;
var delta;
var yaw;

var mclaren;

function onStart() {
    Print("Starting");
    iTime = 0;
    distance = 100;
    _distance = 0;
    rotation = 0;
    _rotation = 0;
    delta = .016;
    yaw = 0;
    SetTargetFPS(100);
}

var myUniform;

function onReady() {
    //SetTargetFPS(100);
    //print("Ready");
    //shader = LoadShader("Shader/baseVS.glsl", "Shader/baseFS.glsl");
    //myUniform = GetUniformLocation(shader, "iTime");
    camera = {
        position: {x: 10, y: 20, z: 30},
        target: {x: 0, y: 0, z: 0},
        up: {x: 0, y: 1, z: 0},
        fovy: 70,
    }
    blockPos = {x: 6, y: 3, z: 8}
    SetWindowTitle("Shader fun");
    mclaren = LoadModel("models/toyota_gr_gt.glb");
    OpenSerial("/dev/ttyUSB0", 9600);
    AllowWindowResize();
}

Print("Hot reload ");
//SetTargetFPS(100);

var a = 0;
function onFrame() {
    delta = GetFrameTime();
    iTime += delta;

    //SetUniform(shader, myUniform, iTime);
    if (IsKeyDown("up")) {
        distance-=delta*100;
    }
    if (IsKeyDown("down")) {
        distance+=delta*100;
    }
    if (IsKeyPressed("right")) {
        rotation+=delta*18;
        a-=10;
        WriteSerial(a);
    }
    if (IsKeyPressed("left")) {
        rotation-=delta*18;
        a+=10;
        WriteSerial(a);
	}

    BeginDrawing();
    //ClearBackground({r: 50, g: 50, b: 50, a: 255});
        distance -= GetMouseWheelMove()*3;
        _distance += (distance-_distance)*delta*10;
        _rotation += (rotation-_rotation)*delta*10;
        camera.position.x = Sin(_rotation)*_distance;
        camera.position.z = Cos(_rotation)*_distance;
        BeginMode3D(camera);

        //BeginShader(shader);
        if (IsMouseButtonDown(0)) {
            rotation -= GetMouseDeltaX()*.006;
            camera.position.y += GetMouseDeltaY()*.2;
        }
        //DrawGrid(100, 5);
        var xDist = 6;//6
        var zDist = 3;//3
        var offset = -20;
        var amplitude = 2;
        for (var y = 0; y < 20; y++) {
            for (var i = 0; i < 10; i++) {
                blockPos.y = Sin(iTime+y*.5+i*.5) * amplitude;
                blockPos.x = offset+3+xDist*i;
                blockPos.z = -30+zDist*y;
                DrawCube(blockPos, 3, 3, 3, (y % 2) ? WHITE : BLACK);
            }
            for (var i = 0; i < 10; i++) {
                blockPos.y = Sin(iTime+y*.5+i*.5-.25) * amplitude;
                blockPos.x = offset+xDist*i;
                DrawCube(blockPos, 3, 3, 3, (y % 2) ? BLACK : WHITE);
            }
        }
        DrawModel(mclaren, {x: 7, y: 5, z: 0}, 30);
        //EndShader();
        EndMode3D();
        DrawFPS(10, 10);
        //SetTargetFPS(0);
	EndDrawing();
}
