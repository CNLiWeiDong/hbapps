# OSX Mojave instructions

These instructions assume:
* You are using OSX Mojave; prior versions won't work
* You installed brew
* You installed xcode's command-line tools and they're up to date

There may be more brew packages needed.

# Install build environment

Install clang 8 and other needed tools:
```
brew install llvm@8 cmake git boost autoconf@2.13  mysql-connector-c mysql++ curl curl-openssl zlib cryptopp
```

# Build hbapps

Note: ignore warnings which look like this:

```
ld: warning: direct access in function ... to global weak symbol ... from file ... means the weak symbol cannot be overridden at runtime. This was likely caused by different translation units being compiled with different visibility settings.
```

## Release build

```
cd ~
git clone --recursive https://127.0.0.1.git
cd hbapps
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Debug build

```
cd ~
git clone --recursive https://127.0.0.1.git
cd hbapps
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
