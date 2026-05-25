
function onStart() {
   print("Starting");
   //SetTargetFPS(60);
   Headless();
   name = "";
}

function onReady() {
    Host(6767);
}
function onFrame() {

}

function onMessage(message) {
    print(message);
    print("Sending back to all clients..");
    SendMessage(message);
}