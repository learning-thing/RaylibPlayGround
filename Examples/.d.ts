// get individual args
declare function Args(pos: number): string;
// get the amount of args
declare function ArgcCount(): number;
// print a message
declare function Print(message: string): void;
// Beginning of the drawLoop
declare function BeginDrawing(): void;
// End of the DrawLoop
declare function EndDrawing(): void;
// Start 3d rendering mode
declare function BeginMode3D(camera: { position: {x: number, y: number, z: number}, target: {x: number, y: number, z: number}, up: {x: number, y: number, z: number}, fovy: number }): void;
// End 3d rendering mode
declare function EndMode3D(): void;
// Clear the background (normally at the beginning of the drawLoop)
declare function ClearBackground(color: object): void;
// Draw the current frames per second
declare function DrawFPS(x: number, y: number): void;
//Set the desired framerate
declare function SetTargetFPS(fps: number):void;
// Pause the application for a given time
declare function Sleep(duration: number): void;
// Get the sine
declare function Sin(x: number): number;
// Get the cosine
declare function Cos(x: number): number;
// Launch in headless mode (no rendering)
declare function Headless(): void;
// Get the frame time
declare function GetFrameTime(): number;
// Get the window width
declare function GetScreenWidth(): number;
// Get the window height
declare function GetScreenHeight(): number;
// Resize the window
declare function ResizeWindow(width: number, height: number): void;
// Maximize the window
declare function MaximizeWindow(): void;
// Set the title of the application window
declare function SetWindowTitle(title: string): void;
// Allow resizing
declare function AllowWindowResize(): void;

// Closes the window
declare function CloseWindow(): void;
// Get the char pressed (check [0] of the returned string)
declare function GetCharPressed(): string;

// Check if a key is held down
declare function IsKeyDown(key: string): boolean;
// Check if a key just got pressed
declare function IsKeyPressed(key: string): boolean;
// Get the mouse X position
declare function GetMouseX(): number;
// Get the mouse Y position
declare function GetMouseY(): number;
// Get Mouse movement between current and last frame (x-axis)
declare function GetMouseDeltaX(): number;
// Get Mouse movement between current and last frame (y-axis)
declare function GetMouseDeltaY(): number;

// Check if a mouse button is down
declare function IsMouseButtonDown(button: number): number;
// Check if a mouse button just got pressed
declare function IsMouseButtonPressed(button: number): number;
// Check how much the mousewheel was moved
declare function GetMouseWheelMove(): number;

// Draw a Line
declare function DrawLine(startX: number, starty: number, endX: number, endY: number, color: {r: number, g: number, b: number, a: number}): void;
// Draw a Circle
declare function DrawCircle(x: number, y: number, radius: number, color: {r: number, g: number, b: number, a: number}): void;
// Draw a Rectangle
declare function DrawRectangle(x: number, y: number, width: number, height: number, color: {r: number, g: number, b: number, a: number}): void;
// Draw a Rectangle (Lines only)
declare function DrawRectangleLines(): void;
// Draw a text
declare function DrawText(text: string, xPos: number, yPos: number, fontSize: number, tint: {r: number, g: number, b: number, a: number}): void;
// Draw an image
declare function DrawImage(filePath: string, posX: number, posY: number, width: number, height: number): void;
// Draw a 3D grid
declare function DrawGrid(slices: number, spacing: number): void;
// Draw a 3D Cube
declare function DrawCube(position: object, width: number, height: number, length: number): void;
// Draw a 3D Cube (from vectors)
declare function DrawCubeV(): void;
// Draw a 3D Cube (from vectors), just wires (broken)
declare function DrawCubeWires(): void;
// Load a 3D model
declare function LoadModel(path: string): number;
// Draw a mesh
declare function DrawMesh(meshID: number): void;
// Draw a model
declare function DrawModel(meshID: number): void;
// Load a shader from the files system, get back a shader id
declare function LoadShader(vertexPath: string, fragmentPath: string): number;
// Load a shaders Uniform location
declare function GetUniformLocation(shaderID: number, uniform: string): number;
// Set the value of a shader uniform
declare function SetUniform(shaderID: number, uniformLocation: number, value: any): void;
// Start using a shader
declare function BeginShader(shaderID: number): void;
// Stop using any active shaders
declare function EndShader(): void;
// Get the last modified time of a file
declare function GetFileModTime(path: string): number;
// Set the font to use
declare function SetFont(path: string): void;
// Open a file to read (returns file id)
declare function OpenFile(path: string): number;
declare function CloseFile(fileID: number): void;
// Read a file from a file
declare function GetLine(fileID: number, maxCharacters: number): number;
// Check if end of file has been reached
declare function AtEOF(fileID: number): boolean;
// Rewind a file (reset seek) (actually reopen)
declare function Rewind(fileID: number): void;
// Write string to a file
declare function Save(filePath: string, content: string);

// Host a server
declare function Host(port: number): void;
// Check if app is hosting
declare function IsHosting(): boolean;
// Connect to a server
declare function Host(address: string): void;
// Send a message (server sends to all clients, clients send to the server)
declare function SendMessage(message: string): void;

// Open a serial port
declare function OpenSerial(path: string, baudrate: number): void;
// Write a message to the opened serial port
declare function WriteSerial(message: string): void;
declare function WriteSerial(message: number): void;
// Check if serial is already opened
declare function IsSerialOpen(): boolean;
// Try to reopen serial if originally failed to do so
declare function SerialRetry(): void;
declare function ReadSerial(): string;
// Get the width of text
declare function MeasureTextW(text: string, fontSize): number;
// Get the height of text
declare function MeasureTextH(text: string, fontSize): number;