# Raylib Playground!

# Cloning
```sh
git clone https://github.com/learning-thing/RaylibPlayGround/ --recursive
```

# Building:
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

# Libraries used:
- Raylib
- MuJS
