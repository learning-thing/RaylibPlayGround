
# Window/Application
```js
// Can be used inside onStart() to launch without a window (useful for servers)
Headless();

// Set the window title
SetWindowTitle(title: String);

// Set the Target Frames-Per-Second
SetTargetFPS(fps: Number);

// Returns frametime in seconds
GetFrameTime();

// Get Window width
GetScreenWidth();

// Get Window height
GetScreenHeight();

// Maximize the Window
MaximizeWindow();
```

# Input
```js
// Check if the key or an action is held down 
// (action sytem not complete (only very few predefined actions available))
IsKeyDown(action: String);

// Similar to isKeyDown, but only true if started pressing in this/last frame
IsKeyPressed(actions: String);

// Get the Character the keyboard is trying to type
// warning may retur a string
GetCharPressed();

// Get the X Position of the mouse
GetMouseX();

// Get the Y Position of the mouse
GetMouseY();

// Get the mouse position x delta
GetMouseDeltaX();

// Get the mouse position y delta
GetMouseDeltaY();

// Check if a mouse button is down (0 - left, 1 - right, middle - 2 etc.)
GetMouseButtonDown();

// Check if a mouse button is pressed (0 - left, 1 - right, middle - 2 etc.)
GetMouseButtonDown();

// Get the rotation of the mouseWheel
GetMouseWheelMove();
```

# Drawing
```js
// Colors (rgba, single-byte per channel) can be defined like this:
var myColorWhite = {
	r: 255,
	g: 255,
	b: 255,
	a: 255,
}

//Begin the Drawloop
BeginDrawing();

// Clear the background with a certain color
ClearBackground({r, g, b, a});

// Draw Circle
DrawCicle(x, y, radius, {r, g, b, a});

// Draw a (filled) Rectangle
DrawRectangle(x, y, width, height, {r, g, b, a});

// Draw an outline of a rectange
DrawRectangleLines(x, y, width, height, {r, g, b, a});

// Draw a string/Text
DrawText(text: String, x, y, fontSize, {r, g, b, a} );

// Set the Font to use by File
SetFont(path: String);

//End the Drawloop
EndDrawing();

//##################### Shaders
// Load a shader, returns a shader
LoadShader(vertexFilePath: String, fragmentFilePath: String);

// Start using a shader
BeginShader(shader: Shader);

// Stop active shader
EndShader();

// Get the location of a shader uniform
GetUniformLocation(shader: Shader, uniformName: String);

// Set a uniform of type Number (float)
SetUniformf(value: Number);
//##################### Shaders

```


# Working with Files

```js

// Open a file, returns an id(Number) to be used with file operation functions
// This can be used anytime safely because opened files will return the same id
OpenFile(path: String);

// Can be used to read the next Line from a file
GetLine(file: Number);

// Check if the end of a file has been reached
AtEOF(file: Number);

// Reset/reopen a file, to start it again from the beginning and force a cache reload
Rewind(file: Number);

```

# Networking
(Read docs/Networking for more details)
```js
// Compile with the multiplayer compiler definition defined for multiplayer
// and uncomment GNS in the CMakeLists.txt


// Host a server using the given port eg.: 6767
Host(port: Number);

// Check if we are hosting a server
IsHosting();

// Connect to a server eg.: 127.0.0.1:6767
Connect(address: String);

// Send any String as a message
SendMessage(message: String);

```

# Utility
```js
// Print any String
print(msg: String);
```

# Math
```js
// Get Sine of X
sin(x: Number);

// Get Cosine of X
cos(x: Number);
```
