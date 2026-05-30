var line;
var name;

function onStart() {
   print("Starting");
   SetTargetFPS(60);
   line = "";
}

var history;

function onReady() {
    SetFont("fonts/Inconsolata-LGC-Bold.otf");
    Connect("127.0.0.1:6767");
    name = "";
    history = []
}

function onFrame() {
    BeginDrawing();
    ClearBackground();
    DrawFPS(10, 10);

    var char = GetCharPressed();
    if (char !== undefined) {
        line+=char;
    }

    DrawRectangle(10, 45, GetScreenWidth()-20, 60, {r: 200, g: 120, b: 255, a: 255});
    DrawText(line, 20, 50, 30, {r: 200, g: 200, b: 255, a: 255} );

    for (var i = 0; i < 10; i++) {
        if (history[history.length-i] !== undefined)
            if (history[history.length-i][6] === name[6]) {
                DrawText(history[history.length - i], 20, 150 + i * 50, 40, {r: 100, g: 100, b: 255, a: 255});
            }
            else {
                DrawText(history[history.length - i], 20, 150 + i * 50, 40, {r: 255, g: 255, b: 255, a: 255});
            }
    }

    if (IsKeyPressed("enter")) {
        SendMessage(name+": "+line);
        line = "";
    }
    EndDrawing();
}

function onMessage(message) {
    print("Received: "+message);
    history.push(message);
    if (name.length < 2) {
        name = message.split(":")[1];
        print("My name is: "+name);
    }
}