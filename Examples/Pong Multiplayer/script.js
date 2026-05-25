function onStart() {
   print("Starting");

}
var velocity = {
    x: 100,
    y: 100
}
var ball = {
    x: 100,
    y: 100
}

var player1 = {
    x: 10,
    y: 10,
    score: 0
}

var player2 = {
    x: 10,
    y: 10,
    score: 0
}

var playerHeight = 100;

function onReady() {
   SetTargetFPS(60);
   SetWindowTitle("Pong server");
   Host(6767);
}

var levelMult = 1;
var ballRadius = 10;
var playerWidth = 10;
var padding = 10;

function onFrame() {
    BeginDrawing();
    ClearBackground();
    DrawFPS(10, 10);
    var delta = GetFrameTime();

    ball.x += velocity.x  * levelMult * delta;
    ball.y += velocity.y  * levelMult * delta;


    if (ball.x >= GetScreenWidth()-ballRadius-playerWidth-padding) {
        if (player2.y < ball.y && player2.y+playerHeight > ball.y) {
            ball.x = GetScreenWidth()-ballRadius-playerWidth-padding;
            level++;
            velocity.x *= -1;
        } else {
            level = 1;
            velocity.x = 100;
            velocity.y = 100;
            ball.x = 100;
            ball.y = 100;
        }
    }
    if (ball.y >= GetScreenHeight()-ballRadius)   { ball.y = GetScreenHeight()-ballRadius; velocity.y *= -1; }
    if (ball.x <= playerWidth+padding+ballRadius) {
        if (player1.y < ball.y && player1.y+playerHeight > ball.y) {
            ball.x = playerWidth+padding+ballRadius;
            velocity.x *= -1; level++;
        } else {
            //if player1 messes up
            level = 1;
            velocity.x = 100;
            velocity.y = 100;
            ball.x = 100;
            ball.y = 100;
        }
    }
    if (ball.y <= ballRadius) { ball.y = ballRadius; velocity.y *= -1; }

    SendMessage(""+ball.x+";"+ball.y);
    EndDrawing();
}

function onMessage(message) {
    SendMessage(message);
    //print(message);
}
