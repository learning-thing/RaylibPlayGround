function onStart() {
    Print("Starting");
    SetTargetFPS(60);
}

function onReady() {}
function onFrame() {
    BeginDrawing();
    ClearBackground({r: 0, g: 0, b: 0, a: 255});

    EndDrawing();
}
