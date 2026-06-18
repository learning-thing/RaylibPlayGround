# Raylib Playground!

# Cloning
```sh
git clone https://github.com/learning-thing/RaylibPlayGround/ --recursive
cd RaylibPlayGround
```

# Building
```sh
# build mujs
cd vendor/mujs
make release
cd ../..

# build RaylibPlayGround
cmake -G "Ninja" -B build -DCMAKE_BUILD_TYPE=Release
cd build
ninja -j 4
```

# Running
Simply run `./RaylibPlayGround` or `./RaylibPlayGround [arg]`, to use [arg] as the script to run.

# Multiplayer
- Unfortunately I struggled to compile the windows version and even though I hate windows I wanted to keep it crossplatform
even though I had some basics already working (the chat app for example),
SteamGameNetworkingSockets also introduced alot of runtime library dependencies and made compiling a littly more annoying,
so I will delay multiplayer until I can actually maintain it. 


# Reading Serial
- Requires boost and is currently only available on linux

# Libraries used
- Raylib
- R3D
- MuJS
- SteamGameNetworkingSockets
- (X11)
- boost::asio