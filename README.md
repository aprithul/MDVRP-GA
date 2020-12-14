# MDVRP-GA
An Multi Depot Vehicle Routing Problem solver using Genetic Algorithm.  GA code is implemented in C++ and then compiled with Emscripten to web assembly.
Contents:
  1. C-mdvrp -> Contains benchmark problems by Cordeau
  2. Emscripten -> Local folder for the compiler. Get it from the Emscripten website.
  3. ga -> C++ genetic algorithm code to solve MDVRP
  4. Web Application -> html/js web application. Uses plotly for graphing.
 

# How to host the web application
Copy contents of the 'Web Application' folder into your root directory. The application should be at : {your domain name}/mdvrp.html
Depending on your host, you may need to rename 'mdvrp.html' to 'index.html'


# Instructions

1) Step 1.
Install Emscripten by following the steps from the Emscripte official website:
https://emscripten.org/docs/getting_started/downloads.html


2) Step2.

