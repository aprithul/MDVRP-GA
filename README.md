# MDVRP-GA
A Multi Depot Vehicle Routing Problem solver using Genetic Algorithm.  GA code is implemented in C++ and then compiled with Emscripten to web assembly.
Contents:
  1. C-mdvrp -> Contains benchmark problems by Cordeau
  2. Emscripten -> Local folder for the compiler. Get it from the Emscripten website.
  3. ga -> C++ genetic algorithm code to solve MDVRP
  4. Web Application -> html/js web application. Uses plotly for graphing.
 

# How to host the web application
Copy contents of the 'Web Application' folder into your host's root directory. The application should now be acceesible at: 
{your domain name}/mdvrp.html
Depending on your host, you may need to rename 'mdvrp.html' to 'index.html'


# Instructions on compiling
Note: All paths are relative to the root of the project.

1) Step 1.
Get Emscripten by following the instructions from the official documentation:
https://emscripten.org/docs/getting_started/downloads.html


2) Step 2.
In the same terminal session, change directory to 'ga/src/'
Then run:
emcc --bind -o ../../Web\ Application/Resources/ga.js gacplus.cpp Evaluate.cpp GA.cpp Individual.cpp Population.cpp Utils.cpp

This will compile the C++ code into WebAssembly and place it (along with the herlper javascript file) inside 'Web Application/Resources'

3)Step 3.
That's all you need to do. You can run a local server by:
emrun ../../Web\ Application/index.html --port 8000


# Modifying the code
The Emscripten documentation should be enough to understand how to connect C++ and JS. 
The C++ GA does have a lot of ifdef statements. This was done so that I could compile the code with g++ alongside Emscripten. The code assumes you are on windows and that WIN32 is defined.


