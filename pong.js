var player1;
var player2;
var ball;
var velocity;
var reloadCount;
var circleColor = {
    r: 200,
    g: 50,
    b: 255,
    a: 255
};

var autoplay = false;
var rectColor = {
    r: 200,
    g: 255,
    b: 255,
    a: 255
};

var playerWidth = 10;
var playerHeight = 100;
var padding = 10;
var ballRadius = 20;
var level = 1;

function onStart() {
    print("Starting");
    SetTargetFPS(100);
}

print("reloaded: " + reloadCount);
reloadCount++;
velocity = { x: 100, y: 100 }

player1.score = 0;
player2.score = 0;

function onReady() {
    reloadCount = 0;
    SetWindowTitle("PONG in RaylibPlayground!");
    player1 = { x: padding, y: 0, score: 0}
    player2 = { x: GetScreenWidth()-padding-playerWidth, y: 0, score: 0 }
    ball = {
        x: 100,
        y: 100
    }
}

function onFrame() {
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
            player1.score++;
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
            player2.score++;
        }
    }
    if (ball.y <= ballRadius) { ball.y = ballRadius; velocity.y *= -1; }

    var delta = GetFrameTime();

    var levelMult = ( 1 + (level * (.5-level*.001) ));
    ball.x += velocity.x  * levelMult * delta;
    ball.y += velocity.y  * levelMult * delta;

    BeginDrawing();
    ClearBackground();
    DrawFPS(GetScreenWidth() / 2 + 5, 10);
    //SetTargetFPS(30);

    //Draw Middle Line
    DrawRectangle(GetScreenWidth()/2, 0, 1, GetScreenHeight(), {r: 100, g: 100, b: 100, a: 255});

    DrawCircle(ball.x, ball.y, ballRadius, {r: 10, g: 10, b: 250, a: 255-ball.x/20});
    DrawCircle(ball.x, ball.y, ballRadius, {r: 200, g: 10, b: 10, a: ball.x/5});

    if (IsKeyDown("w"))    { player1.y -= 4; }
    if (IsKeyDown("s"))    { player1.y += 4; }
    if (IsKeyDown("up"))   { player2.y -= 4; }
    if (IsKeyDown("down")) { player2.y += 4; }

    if (IsKeyPressed("space")) { autoplay = !autoplay; }

    //Draw Left player1
    DrawRectangle(player1.x, player1.y, playerWidth, playerHeight, {r: 10, g: 10, b: 250, a: 255});

    //Draw Right player2
    DrawRectangle(player2.x, player2.y, playerWidth, playerHeight, {r: 200, g: 10, b: 10, a: 255});

    //auto playe
    if (autoplay) {
        player1.y = ball.y-playerHeight/2;
        player2.y = ball.y-playerHeight/2;
    }

    DrawText(""+player1.score+":"+player2.score+"\nLevel: "+level, GetScreenWidth()/2-25, 30, 40, {r: 20, g: 200, b: 10, a: 255});

    EndDrawing();
}

