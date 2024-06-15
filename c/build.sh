# need install mingw on windows

# create temp windows resource file
touch temp.rc
echo "id ICON \"../image/arco.ico\"" > temp.rc
# build temp rc file
windres -i temp.rc -o temprc.o
# build exe
gcc main.c arcojson.c temprc.o -o ../arco_player.exe

# clean temp rc file
rm temp.rc temprc.o
