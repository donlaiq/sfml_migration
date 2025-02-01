# Migration to SFML 3.0.0
Migration of the code from Beginning-C-Game-Programming-Third-Edition to support SFML 3.0.0

A note for my future self. I've been using GNOME Terminator to manage multiple terminals in a single window and editing everything with Vim.

These are the command templates required to build it on Linux:

`$ g++ -c *.cpp -I<full_path_to_the_sfml_library_folder>/include`  
`$ g++ -std=c++20 *.o -o <executable_name> -L<full_path_to_the_sfml_library_folder>/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio`  
`$ export LD_LIBRARY_PATH=<full_path_to_the_sfml_library_folder>/lib && ./<executable_name>`  

A concrete example for the Zombie Arena project might be:

`$ g++ -c *.cpp -I/home/donlaiq/sfml/include`  
`$ g++ -std=c++20 *.o -o Zombies -L/home/donlaiq/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio`  
`$ export LD_LIBRARY_PATH=/home/donlaiq/sfml/lib && ./Zombies`  

All the assets can be found in the original project by following this [link](https://github.com/PacktPublishing/Beginning-C-Game-Programming-Third-Edition).
