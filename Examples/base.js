var player;
var velocity;
var circleColor = {
    r: 50,
    g: 50,
    b: 255,
    a: 255
};

velocity = { x: 200, y: 200 }

function onStart() {
    Print("Starting");
    SetTargetFPS(60);

    player = { x: 0, y: 0 }
}

function onReady() {}
function onFrame() {
    BeginDrawing();
    ClearBackground({r: 0, g: 0, b: 0, a: 255});
    //DrawFPS(10, 10);
    if (player.x >= GetScreenWidth()) { velocity.x *= -1; }
    if (player.y >= GetScreenHeight()) { velocity.y *= -1; }
    if (player.x <= 0) { velocity.x *= -1; }
    if (player.y <= 0) { velocity.y *= -1; }
    player.x += velocity.x * GetFrameTime();
    player.y += velocity.y * GetFrameTime();
    DrawCircle(player.x, player.y, 20, circleColor);
    EndDrawing();
}
