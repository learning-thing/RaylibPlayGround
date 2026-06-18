
function onStart() {
    Print("Starting");
}

var light;
function onReady() {
    SetTargetFPS(60);
    OpenSerial("/dev/ttyACM0");
    SetTargetFPS(100);
    AllowWindowResize();
    light = true;
}

var vel = {x: 100, y: 100};
var speed = 500;


function onFrame() {
    var delta = GetFrameTime();
    var msg = ReadSerial();
    if (IsSerialOpen() && msg) {
        Print(msg);
        switch (Number(msg)) {
            case 68:
                vel.x-=speed*delta;
                break;
            case 67:
                vel.x+=speed*delta;
                break;
            case 69:
                light = !light;
                Print(light);
                break;
            case 70:
                vel.y-=speed*delta;
                break;
            case 21:
                vel.y+=speed*delta;
                break;
            default:
                break
        }
    }

    BeginDrawing();
    ClearBackground({r: 0, g: 0, b: 0, a: 255});
    if (light===true) {
        DrawCircle(vel.x, vel.y, 20, {r: 255, g: 255, b: 255, a: 255});
    }
    EndDrawing();
}
