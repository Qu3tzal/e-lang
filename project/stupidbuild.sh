# Clear.
rm ./../linux/bin/*

# Build.
g++ -Wall -Wfatal-errors -Werror -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline ./../src/* -o ./../linux/bin/e-lang

# Promote.
chmod +x ./../bin/linux/e-lang

# Run
./../linux/bin/e-lang
