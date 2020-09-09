# hbapps
hbapps include monitor, grid_trad, monitor_mail, log_server apps.


# code format
1、clang-format -style=llvm -dump-config > .clang-format
2、命令手动
find common/ -name *.h | xargs -I F clang-format -i F
find common/ -name *.hpp | xargs -I F clang-format -i F
find common/ -name *.cpp | xargs -I F clang-format -i F
3、git-clang-format
