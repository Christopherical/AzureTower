# AzureTower
Prototype Roguelike Dungeon crawler.
# One-time setup for Debug
cmake -B Build -D CMAKE_BUILD_TYPE=Debug

cmake --build Build --config Debug -j
# One-time setup for Release
cmake -B Build -D CMAKE_BUILD_TYPE=Release

cmake --build Build --config Release -j



