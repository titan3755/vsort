#include <sdl.h>
#undef main  // Undefine main for SDL2 to work with main function
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;
constexpr auto ARRAY_SIZE = 100;
#define BAR_WIDTH (WINDOW_WIDTH / ARRAY_SIZE)

struct Color {
    int r;
    int g;
    int b;
};

Color barClr = {
    0, 255, 0
};

Color compClr = {
    255, 0, 0
};

Color windowClr = {
    0, 0, 0
};

void draw_bars(SDL_Surface* surface, int* array) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        SDL_Rect rect = { i * BAR_WIDTH, WINDOW_HEIGHT - array[i], BAR_WIDTH, array[i] };
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, barClr.r, barClr.g, barClr.b));  // Blue color
    }
}

void bubble_sort_visualized(SDL_Surface* surface, SDL_Window* window, int* array) {
    int swapped;
    int temp;
	SDL_Event event;
    // Bubble sort algorithm
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
			}
		}
        swapped = 0;
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++) {
            // Highlight the two bars being compared
            SDL_Rect rect1 = { j * BAR_WIDTH, WINDOW_HEIGHT - array[j], BAR_WIDTH, array[j] };
            SDL_Rect rect2 = { (j + 1) * BAR_WIDTH, WINDOW_HEIGHT - array[j + 1], BAR_WIDTH, array[j + 1] };

            // Color bars being compared in red
            SDL_FillRect(surface, &rect1, SDL_MapRGB(surface->format, compClr.r, compClr.g, compClr.b)); // Red color for compared bars
            SDL_FillRect(surface, &rect2, SDL_MapRGB(surface->format, compClr.r, compClr.g, compClr.b));

            // Update window surface to show changes
            SDL_UpdateWindowSurface(window);  // Correctly passing the window here

            // Compare and swap if needed
            if (array[j] > array[j + 1]) {
                // Swap the elements
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                swapped = 1;
            }

            // Redraw the entire array (clear previous coloring)
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, windowClr.r, windowClr.g, windowClr.b)); // Black background
            draw_bars(surface, array);

            // Update window surface
            SDL_UpdateWindowSurface(window);  // Correctly passing the window here
        }
        // If no two elements were swapped in the inner loop, the array is sorted
        if (swapped == 0)
            break;
    }
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Bubble Sort Visualizer",  // Window title
        SDL_WINDOWPOS_UNDEFINED,   // Window position (X)
        SDL_WINDOWPOS_UNDEFINED,   // Window position (Y)
        WINDOW_WIDTH,              // Window width
        WINDOW_HEIGHT,             // Window height
        SDL_WINDOW_SHOWN           // Flags
    );

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Get the window surface
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if (surface == NULL) {
        printf("Unable to get window surface! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize random seed
    srand(time(NULL));

    // Generate an array of random heights
    int array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % (WINDOW_HEIGHT - 50) + 1;  // Random height between 1 and WINDOW_HEIGHT
    }

    // Draw initial bars
    draw_bars(surface, array);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(1000);  // Delay to let the user see the initial state

    // Perform the bubble sort with visualization
    bubble_sort_visualized(surface, window, array);

    // Wait before quitting
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0);
            }
        }
    }

    // Clean up and close the program
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
