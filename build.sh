# NOTE: Modified from https://gist.github.com/KuanYuChang/f768cd48e1f2e261109a1e6bfa5a07b5

# If using macOS, please first install Command-Line Tools, Homebrew, and cmake via the following commands:
# $ xcode-select --install
# $ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# $ brew install cmake

#git submodule add https://github.com/Mojang/leveldb.git leveldb
git submodule update --init --recursive
mkdir local
mkdir -p leveldb/build && cd leveldb/build
cmake -DCMAKE_INSTALL_PREFIX=../../local -DCMAKE_BUILD_TYPE=Release ../../leveldb
make install -j4
cd ../..
cmake .
make
./minecraft-editor
