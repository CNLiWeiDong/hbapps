#!/ bin / sh
SOURCE_DIR="$( cd "$( dirname "$0" )" && pwd )"
for file in `find ../plugins/ -name '*.hpp'`
do
    clang-format -i $file -style=Mozilla
done