#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <sdl.h>
#undef main  // Undefine main for SDL2 to work with main function, because SDL2 defines main as SDL_main
#include <SDL_ttf.h>

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;
constexpr auto ARRAY_SIZE = 100;
constexpr auto SORTING_ALGORITHM = 3;  // 1 for bubble sort, 2 for selection sort, 3 for insertion sort;

#define BAR_WIDTH (WINDOW_WIDTH / ARRAY_SIZE)

struct Color {
    int r;
    int g;
    int b;
};

Color barClr = {
    0, 255, 0
};

Color barFinalClr = {
	0, 0, 255
};

Color compClr = {
    255, 0, 0
};

Color windowClr = {
    0, 0, 0
};

std::mutex mtx;

static void setWindowTitleAccordingToFramerateAndTimeElapsed(SDL_Window* window) {
    static int frameCount = 0;
    static int timerFPS = 0;
    static Uint32 lastFrameTime = 0;
    static char title[100];
    frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - timerFPS > 1000) {
        timerFPS = currentTime;
		if (SORTING_ALGORITHM == 1) {
            sprintf_s(title, "Bubble Sort Visualizer - FPS: %d, Frame Time: %d ms", frameCount, (currentTime - lastFrameTime) / frameCount);
		}
		else if (SORTING_ALGORITHM == 2) {
            sprintf_s(title, "Selection Sort Visualizer - FPS: %d, Frame Time: %d ms", frameCount, (currentTime - lastFrameTime) / frameCount);
		}
		else if (SORTING_ALGORITHM == 3) {
			sprintf_s(title, "Insertion Sort Visualizer - FPS: %d, Frame Time: %d ms", frameCount, (currentTime - lastFrameTime) / frameCount);
		}
        std::lock_guard<std::mutex> lock(mtx);
        SDL_SetWindowTitle(window, title);
        lastFrameTime = currentTime;
        frameCount = 0;
    }
}

void draw_bars(SDL_Renderer *renderer, int* array) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        SDL_Rect rect = { i * BAR_WIDTH, WINDOW_HEIGHT - array[i], BAR_WIDTH, array[i] };
		SDL_RenderFillRect(renderer, &rect);
    }
}

void bubble_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, int* array) {
    int swapped;
    int temp;
	SDL_Event event;
    // Bubble sort algorithm
    // event loop thread
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        swapped = 0;
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
                    SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					TTF_Quit();
					SDL_Quit();
					exit(0);
				}
			}
            // Highlight the two bars being compared
            SDL_Rect rect1 = { j * BAR_WIDTH, WINDOW_HEIGHT - array[j], BAR_WIDTH, array[j] };
            SDL_Rect rect2 = { (j + 1) * BAR_WIDTH, WINDOW_HEIGHT - array[j + 1], BAR_WIDTH, array[j + 1] };

            // Color bars being compared in red
			SDL_SetRenderDrawColor(renderer, compClr.r, compClr.g, compClr.b, 255);
			SDL_RenderFillRect(renderer, &rect1);
			SDL_RenderFillRect(renderer, &rect2);

            // Update window surface to show changes
			SDL_RenderPresent(renderer);

            // Compare and swap if needed
            if (array[j] > array[j + 1]) {
                // Swap the elements
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                swapped = 1;
            }

            // Redraw the entire array (clear previous coloring)
			SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
            draw_bars(renderer, array);

            // Update window surface
			SDL_RenderPresent(renderer);
        }
        // If no two elements were swapped in the inner loop, the array is sorted
        if (swapped == 0)
            break;
    }
}

void selection_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, int* array) {
	int minIndex;
	int temp;
	SDL_Event event;
	// Selection sort algorithm
	for (int i = 0; i < ARRAY_SIZE - 1; i++) {
		minIndex = i;
		for (int j = i + 1; j < ARRAY_SIZE; j++) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					TTF_Quit();
					SDL_Quit();
					exit(0);
				}
			}
			// Highlight the two bars being compared
			SDL_Rect rect1 = { i * BAR_WIDTH, WINDOW_HEIGHT - array[i], BAR_WIDTH, array[i] };
			SDL_Rect rect2 = { j * BAR_WIDTH, WINDOW_HEIGHT - array[j], BAR_WIDTH, array[j] };

			// Color bars being compared in red
			SDL_SetRenderDrawColor(renderer, compClr.r, compClr.g, compClr.b, 255);
			SDL_RenderFillRect(renderer, &rect1);
			SDL_RenderFillRect(renderer, &rect2);

			// Update window surface to show changes
			SDL_RenderPresent(renderer);

			// Compare and swap if needed
			if (array[j] < array[minIndex]) {
				minIndex = j;
			}

			// Redraw the entire array (clear previous coloring)
			SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
			draw_bars(renderer, array);

			// Update window surface
			SDL_RenderPresent(renderer);
		}
		// Swap the elements
		temp = array[i];
		array[i] = array[minIndex];
		array[minIndex] = temp;
	}
}

void insertion_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, int* array) {
	int key;
	int j;
	SDL_Event event;
	// Insertion sort algorithm
	for (int i = 1; i < ARRAY_SIZE; i++) {
		key = array[i];
		j = i - 1;
		while (j >= 0 && array[j] > key) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					TTF_Quit();
					SDL_Quit();
					exit(0);
				}
			}
			// Highlight the two bars being compared
			SDL_Rect rect1 = { j * BAR_WIDTH, WINDOW_HEIGHT - array[j], BAR_WIDTH, array[j] };
			SDL_Rect rect2 = { (j + 1) * BAR_WIDTH, WINDOW_HEIGHT - array[j + 1], BAR_WIDTH, array[j + 1] };

			// Color bars being compared in red
			SDL_SetRenderDrawColor(renderer, compClr.r, compClr.g, compClr.b, 255);
			SDL_RenderFillRect(renderer, &rect1);
			SDL_RenderFillRect(renderer, &rect2);

			// Update window surface to show changes
			SDL_RenderPresent(renderer);

			// Compare and swap if needed
			array[j + 1] = array[j];
			j = j - 1;

			// Redraw the entire array (clear previous coloring)
			SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
			draw_bars(renderer, array);

			// Update window surface
			SDL_RenderPresent(renderer);
		}
		array[j + 1] = key;
	}
}

int main() {
	// get time at start of program
	auto start = std::chrono::high_resolution_clock::now();
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
	}
	else {
		printf("SDL initialized successfully!\n");
	}

	// Initialize SDL_ttf
	if (TTF_Init() < 0) {
		printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
		SDL_Quit();
		return -1;
	}
	else {
		printf("SDL_ttf initialized successfully!\n");
	}

    // Create a window
	char window_title[] = "Sorting Visualizer";
	if (SORTING_ALGORITHM == 1) {
		char window_title[] = "Bubble Sort Visualizer";
		printf("Bubble Sort Visualizer\n");
	}
	else if (SORTING_ALGORITHM == 2) {
		char window_title[] = "Selection Sort Visualizer";
		printf("Selection Sort Visualizer\n");
	}
	else if (SORTING_ALGORITHM == 3) {
		char window_title[] = "Insertion Sort Visualizer";
		printf("Insertion Sort Visualizer\n");
	}
    SDL_Window* window = SDL_CreateWindow(
        window_title,              // Window title
        SDL_WINDOWPOS_UNDEFINED,   // Window position (X)
        SDL_WINDOWPOS_UNDEFINED,   // Window position (Y)
        WINDOW_WIDTH,              // Window width
        WINDOW_HEIGHT,             // Window height
        SDL_WINDOW_SHOWN           // Flags
    );

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		TTF_Quit();
        SDL_Quit();
        return -1;
    }
	else {
		printf("Window created successfully!\n");
	}

	// use hardware acceleration
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL || !renderer) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return -1;
	}
	else {
		printf("Renderer created successfully!\n");
	}

	// Set the window title according to the frame rate and time elapsed
	std::atomic <bool> quit_thread(false);
    std::thread t([&quit_thread](SDL_Window* window) {
        while (!quit_thread) {
            setWindowTitleAccordingToFramerateAndTimeElapsed(window);
            std::this_thread::sleep_for(std::chrono::microseconds(1)); // Add a small delay to avoid high CPU usage
        }
		printf("Thread terminated!\n");
    }, window);

    // screen flush
	SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
	SDL_RenderClear(renderer);

    // Initialize random seed
    srand(static_cast<unsigned int>(time(NULL)));

    // Generate an array of random heights
    int array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % (WINDOW_HEIGHT - 50) + 1;  // Random height between 1 and WINDOW_HEIGHT
    }

    // Draw initial bars
	SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
    draw_bars(renderer, array);
	SDL_RenderPresent(renderer);
    SDL_Delay(1000);  // Delay to let the user see the initial state

    // Perform the bubble sort with visualization
	if (SORTING_ALGORITHM == 1) {
		bubble_sort_visualized(renderer, window, array);
	}
	else if (SORTING_ALGORITHM == 2) {
		selection_sort_visualized(renderer, window, array);
	}
	else if (SORTING_ALGORITHM == 3) {
		insertion_sort_visualized(renderer, window, array);
	}
	else {
		printf("Invalid sorting algorithm selected!\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return -1;
	}
	SDL_RenderPresent(renderer);
	// draw the final bars with a different color
	SDL_SetRenderDrawColor(renderer, barFinalClr.r, barFinalClr.g, barFinalClr.b, 255);
	for (int i = 0; i < ARRAY_SIZE; i++) {
		SDL_Rect rect = { i * BAR_WIDTH, WINDOW_HEIGHT - array[i], BAR_WIDTH, array[i] };
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);
        SDL_Delay(BAR_WIDTH);
	}
	// terminate framerate thread
	quit_thread = true;
	// get time at end of program
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	char window_title_final[100];
	if (SORTING_ALGORITHM == 1) {
		sprintf_s(window_title_final, "Bubble Sort Visualizer - Time Elapsed: %f seconds", elapsed.count());
	}
	else if (SORTING_ALGORITHM == 2) {
		sprintf_s(window_title_final, "Selection Sort Visualizer - Time Elapsed: %f seconds", elapsed.count());
	}
	else if (SORTING_ALGORITHM == 3) {
		sprintf_s(window_title_final, "Insertion Sort Visualizer - Time Elapsed: %f seconds", elapsed.count());
	}
	else {
		sprintf_s(window_title_final, "Sorting Visualizer - Time Elapsed: %f seconds", elapsed.count());
	}
	SDL_SetWindowTitle(window, "window_title_final");
    // Wait before quitting
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
				SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
				TTF_Quit();
                SDL_Quit();
                exit(0);
            }
        }
    }

    // Clean up and close the program
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
