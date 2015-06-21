#!/bin/bash
# create multiresolution windows icon, and pixmaps
# variables
ICON_SRC=../pixmaps/bitmark1024.png
ICON_DST=../../src/qt/res/icons/bitmark.ico
ICON_SRC_TEST=../../src/qt/res/icons/bitmark_testnet.png
ICON_DST_TEST=../../src/qt/res/icons/bitmark_testnet.ico
# create base for all mainnet icons
convert ${ICON_SRC} -resize 16x16 ../pixmaps/bitmark16.png
convert ${ICON_SRC} -resize 32x32 ../pixmaps/bitmark32.png
convert ${ICON_SRC} -resize 48x48 bitmark-48.png
convert ${ICON_SRC} -resize 64x64 ../pixmaps/bitmark64.png
convert ${ICON_SRC} -resize 128x128 ../pixmaps/bitmark128.png
convert ${ICON_SRC} -resize 256x256 ../pixmaps/bitmark256.png
convert ${ICON_SRC} -resize 512x512 ../pixmaps/bitmark512.png
cp ../pixmaps/bitmark256.png ../../src/qt/res/icons/bitmark.png
# create mainnet windows icon with all variant sizes
convert ../pixmaps/bitmark16.png ../pixmaps/bitmark32.png bitmark-48.png ../pixmaps/bitmark256.png ${ICON_DST}
cp ../pixmaps/bitmark16.png ../../src/qt/res/icons/toolbar.png
# create base for all testnet icons 
convert ${ICON_SRC} -colorspace Gray ../pixmaps/Tbitmark1024.png
convert ../pixmaps/Tbitmark1024.png -resize 256x256 ${ICON_SRC_TEST}
convert ${ICON_SRC_TEST} -resize 16x16 bitmark-test-16.png
convert ${ICON_SRC_TEST} -resize 32x32 bitmark-test-32.png
convert ${ICON_SRC_TEST} -resize 48x48 bitmark-test-48.png
cp ${ICON_SRC_TEST} bitmark-test-256.png
# create testnet windows icon with all varient sizes
convert bitmark-test-16.png bitmark-test-32.png bitmark-test-48.png bitmark-test-256.png ${ICON_DST_TEST}
cp bitmark-test-16.png ../../src/qt/res/icons/toolbar_testnet.png
# create pixmap ico and xpm files
convert ../pixmaps/bitmark16.png ../pixmaps/favicon.ico
cp ${ICON_DST} ../pixmaps/bitmark.ico
convert ../pixmaps/bitmark16.png ../pixmaps/bitmark16.xpm
convert ../pixmaps/bitmark32.png ../pixmaps/bitmark32.xpm
convert ../pixmaps/bitmark64.png ../pixmaps/bitmark64.xpm
convert ../pixmaps/bitmark128.png ../pixmaps/bitmark128.xpm
convert ../pixmaps/bitmark256.png ../pixmaps/bitmark256.xpm
# create apple icns file, use libicns 8.2.1 not the default
mv ../pixmaps/bitmark64.png ../pixmaps/tempbitmark64.png
png2icns ../../src/qt/res/icons/bitmark.icns ../pixmaps/bitmark*.png
mv ../pixmaps/tempbitmark64.png ../pixmaps/bitmark64.png
# clean up
rm bitmark-48.png
rm bitmark-test-*.png
rm ../pixmaps/Tbitmark1024.png
