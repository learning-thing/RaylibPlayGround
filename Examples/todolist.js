var tasks;
var textFile;
var taskCount;
var iTime;
var padSpace;
var a;
var timer;

function onStart() {
    print("Starting");
    SetTargetFPS(30);
}

function reloadTextFile() {
    print("Starting to reload text file");
    Rewind(textFile);

    tasks = []
    taskCount = 0;
    var longest = 0;
    while (!AtEOF(textFile)) {
        taskCount++;
        var newTask = GetLine(textFile, 256)
        if (newTask.length < 1) break;
        print(""+taskCount+": "+newTask);
        //print(longest);
        if (newTask.length > longest) {
            longest = newTask.length;

        }
        tasks.push(newTask);
    }
    padSpace = GetScreenHeight()/tasks.length;
    if (longest * padSpace > (GetScreenWidth()*1.5)) {
        //print("too long: " + longest);
        padSpace = (GetScreenWidth()*1.5)/longest;
    }
    print("Reloaded text file");
}

function onReady() {
    tasks = [];
    taskCount = 0;
    textFile = OpenFile("test.txt");
    iTime = 0
    taskCount = 0;
    a = 0;
    timer = 2;
    reloadTextFile();

    SetFont("fonts/Inconsolata-LGC-Bold.otf");
}

var fileModTime = GetFileModTime("test.txt");
print("Hot reload!!")

//SetTargetFPS(20);
function onFrame() {
    BeginDrawing();
    ClearBackground();
    //DrawFPS(10, 10);

    if (GetFileModTime("test.txt") != fileModTime) {
        print("Have to reload: " + fileModTime)
        fileModTime = GetFileModTime("test.txt");
        reloadTextFile();
    }

    iTime += GetFrameTime();
    timer -= GetFrameTime();
    if (timer <= 0) {
        timer = .1; a++;
    }

    var offset = 2;//20+sin(iTime*5)*10;
    for (var i = 0; i < tasks.length; ++i) {
        if ((a % (tasks.length)) == i) DrawText(""+(i+1)+": "+tasks[i], 10+offset, padSpace*i, padSpace, {r: 250, g: 250, b: 250, a: 255});
        else                             DrawText(""+(i+1)+": "+tasks[i], 10,        padSpace*i, padSpace, {r: 250, g: 250, b: 250, a: 100});
    }

    EndDrawing();
}
