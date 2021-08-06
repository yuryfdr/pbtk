#!/bin/sh
export LANG=C
# CD to the current script path
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
PBDEV_DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )/../pocketbook-sdk/"

if [ "$1" = "" -o "$1" = "360" ]; then

    if [ -f pb360/pbtk_test.app ]; then
        echo 'Remove previous build'
        rm -f pb360/pbtk_test.app
    fi

    mkdir -p pb360
    cd pb360
    cmake \
        -D CMAKE_TOOLCHAIN_FILE=../tools/toolchain-arm-pocketbook.cmake \
        -D TARGET_TYPE=ARM \
        -D DEVICE_NAME=pb360 \
        -D MAX_IMAGE_SCALE_MUL=2 \
        -D CMAKE_BUILD_TYPE=Release \
        -D ENABLE_CHM=1 \
        -D ENABLE_ANTIWORD=1 \
        -D GUI=CRGUI_PB \
        -D CMAKE_CXX_FLAGS_RELEASE:STRING="-fomit-frame-pointer -O1" \
        ..
    make

    cd ..
    if [ -f pb360/pbtk_test.app ]; then
        echo 'Strip binary'
        "$PBDEV_DIR/PBSDK/bin/arm-linux-strip" "pb360/pbtk_test.app"
    else
        echo 'Failed compiling binary!'
        exit
    fi
    echo 'Done'

fi

if [ "$1" = "" -o "$1" = "pro2" ]; then

    if [ ! -f "$PBDEV_DIR/FRSCSDK/arm-none-linux-gnueabi/sysroot/usr/lib/libinkview.1.1a.so" ]; then
        echo
        echo 'Invalid SDK structure!'
        echo
        echo 'libinkview.so is in FRSCSDK/arm-none-linux-gnueabi/sysroot/usr/lib/'
        echo '  For pro2 you have to rename it to libinkview.1.1a.so'
        echo '  (a symlink is created by make.sh when needed)'
        echo
        exit
    fi

    curDir="`pwd`"
    cd "$PBDEV_DIR/FRSCSDK/arm-none-linux-gnueabi/sysroot/usr/lib"
    rm -f libinkview.so
    ln -s libinkview.1.1a.so libinkview.so
    cd "$curDir"

    if [ -f pbpro2/pbtk_test.app ]; then
        echo 'Remove previous build'
        rm -f pbpro2/pbtk_test.app
    fi

    mkdir -p pbpro2
    cd pbpro2
    cmake \
        -D CMAKE_TOOLCHAIN_FILE=../tools/toolchain-arm-gnu-eabi-pocketbook.cmake \
        -D TARGET_TYPE=ARM \
        -D DEVICE_NAME=pb360 \
        -D MAX_IMAGE_SCALE_MUL=2 \
        -D CMAKE_BUILD_TYPE=Release \
        -D ENABLE_CHM=1 \
        -D ENABLE_ANTIWORD=1 \
        -D GUI=CRGUI_PB \
        -D ENABLE_PB_DB_STATE=1 \
        -D BACKGROUND_CACHE_FILE_CREATION=1 \
        -D POCKETBOOK_PRO=1 \
        -D POCKETBOOK_PRO_PRO2=1 \
        ..
    make

    cd ..
    if [ -f pbpro2/pbtk_test.app ]; then
        echo 'Strip binary'
        "$PBDEV_DIR/FRSCSDK/bin/arm-none-linux-gnueabi-strip" "pbpro4/pbtk_test.app"
    else
        echo 'Failed compiling binary!'
        exit
    fi
    echo 'Done'

fi

if [ "$1" = "" -o "$1" = "pro4" ]; then

    if [ ! -f "$PBDEV_DIR/FRSCSDK/arm-none-linux-gnueabi/sysroot/usr/lib/libinkview.1.1a.so" ]; then
        echo
        echo 'Invalid SDK structure!'
        echo
        echo 'libinkview.so is in FRSCSDK/arm-none-linux-gnueabi/sysroot/usr/lib/'
        echo '  For pro4 you have to add libinkview.pb626.fw4.4.so from a FW4 device to the lib dir'
        echo '  (a symlink is created by make.sh when needed)'
        echo
        exit
    fi

    curDir="`pwd`"
    cd "$PBDEV_DIR/FRSCSDK/arm-none-linux-gnueabi/sysroot/usr/lib"
    rm -f libinkview.so
    ln -s libinkview.pb626.fw4.4.so libinkview.so
    cd "$curDir"

    if [ -f pbpro4/pbtk_test.app ]; then
        echo 'Remove previous build'
        rm -f pbpro4/pbtk_test.app
    fi

    mkdir -p pbpro4
    cd pbpro4
    cmake \
        -D CMAKE_TOOLCHAIN_FILE=../tools/toolchain-arm-gnu-eabi-pocketbook.cmake \
        -D TARGET_TYPE=ARM \
        -D DEVICE_NAME=pb360 \
        -D MAX_IMAGE_SCALE_MUL=2 \
        -D CMAKE_BUILD_TYPE=Release \
        -D ENABLE_CHM=1 \
        -D ENABLE_ANTIWORD=1 \
        -D GUI=CRGUI_PB \
        -D ENABLE_PB_DB_STATE=1 \
        -D BACKGROUND_CACHE_FILE_CREATION=1 \
        -D POCKETBOOK_PRO=1 \
        -D CR3_JPEG=1 \
        ..
    make

    cd ..
    if [ -f pbpro4/pbtk_test.app ]; then
        echo 'Strip binary'
        "$PBDEV_DIR/FRSCSDK/bin/arm-none-linux-gnueabi-strip" "pbpro4/pbtk_test.app"
    else
        echo 'Failed compiling binary!'
        exit
    fi
    echo 'Done'

fi

if [ "$1" = "" -o "$1" = "pro5" ]; then

    if [ -f pbpro5/pbtk_test.app ]; then
        echo 'Remove previous build'
        rm -f pbpro5/pbtk_test.app
    fi

    mkdir -p pbpro5
    cd pbpro5
    cmake \
        -D CMAKE_TOOLCHAIN_FILE=../tools/toolchain-arm-obreey-linux-gnueabi-pocketbook.cmake \
        -D TARGET_TYPE=ARM \
        -D DEVICE_NAME=pb360 \
        -D MAX_IMAGE_SCALE_MUL=2 \
        -D CMAKE_BUILD_TYPE=Release \
        -D ENABLE_CHM=1 \
        -D ENABLE_ANTIWORD=1 \
        -D GUI=CRGUI_PB \
        -D ENABLE_PB_DB_STATE=1 \
        -D BACKGROUND_CACHE_FILE_CREATION=1 \
        -D POCKETBOOK_PRO=1 \
        -D POCKETBOOK_PRO_FW5=1 \
        ..
    make

    cd ..
    if [ -f pbpro5/pbtk_test.app ]; then
        echo 'Strip binary'
        "$PBDEV_DIR/SDK_481/bin/arm-obreey-linux-gnueabi-strip" "pbpro5/pbtk_test.app"
    else
        echo 'Failed compiling binary!'
        exit
    fi
    echo "Done"

fi
