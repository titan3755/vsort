#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <sdl.h>
#undef main  // Undefine main for SDL2 to work with main function, because SDL2 defines main as SDL_main
#include <SDL_ttf.h>
#include <SDL_image.h>

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;
auto ARRAY_SIZE = 100;
auto SORTING_ALGORITHM = 3;  // 1 for bubble sort, 2 for selection sort, 3 for insertion sort, 4 for quicksort, 5 for counting sort, 6 for merge sort;

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
		else if (SORTING_ALGORITHM == 4) {
			sprintf_s(title, "Quick Sort Visualizer - FPS: %d, Frame Time: %d ms", frameCount, (currentTime - lastFrameTime) / frameCount);
		}
		else if (SORTING_ALGORITHM == 5) {
			sprintf_s(title, "Counting Sort Visualizer - FPS: %d, Frame Time: %d ms", frameCount, (currentTime - lastFrameTime) / frameCount);
		}
		else if (SORTING_ALGORITHM == 6) {
			sprintf_s(title, "Merge Sort Visualizer - FPS: %d, Frame Time: %d ms", frameCount, (currentTime - lastFrameTime) / frameCount);
		}
        SDL_SetWindowTitle(window, title);
        lastFrameTime = currentTime;
        frameCount = 0;
    }
}

// read the "ARRAY_SIZE" value from the "settings.txt" file and return the value as integer
int readArraySizeFromFile() {
	FILE* file;
	errno_t err = fopen_s(&file, "settings.txt", "r");
	if (err != 0) {
		printf("Error opening file!\n");
		return -1;
	}

	int arraySize;
	if (fscanf_s(file, "%d", &arraySize) != 1) {
		printf("Error reading array size!\n");
		fclose(file);
		return -1;
	}

	fclose(file);
	return arraySize;
}


void event_handler(SDL_Event* event, SDL_Renderer* renderer, SDL_Window* window) {
	while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			exit(0);
		}
	}
}

void draw_bars(SDL_Renderer *renderer, int* array) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        SDL_Rect rect = { i * BAR_WIDTH, WINDOW_HEIGHT - array[i], BAR_WIDTH, array[i] };
		SDL_RenderFillRect(renderer, &rect);
    }
}

void bubble_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int* read_count, int* write_count, int* comp_count) {
    int swapped;
    int temp;
    // Bubble sort algorithm
    // event loop thread
    for (int i = 0; i < ARRAY_SIZE - 1; i++) {
        swapped = 0;
        for (int j = 0; j < ARRAY_SIZE - i - 1; j++) {
			event_handler(event, renderer, window);
			setWindowTitleAccordingToFramerateAndTimeElapsed(window);
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
			(*read_count) += 2;
			(*comp_count)++;
            if (array[j] > array[j + 1]) {
                // Swap the elements
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
				(*read_count) += 2;
				(*write_count) += 3;
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

void selection_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int* read_count, int* write_count, int* comp_count) {
	int minIndex;
	int temp;
	// Selection sort algorithm
	for (int i = 0; i < ARRAY_SIZE - 1; i++) {
		minIndex = i;
		for (int j = i + 1; j < ARRAY_SIZE; j++) {
			event_handler(event, renderer, window);
			setWindowTitleAccordingToFramerateAndTimeElapsed(window);
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
			(*read_count) += 2;
			(*comp_count)++;
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
		(*write_count) += 3;
		(*read_count) += 2;
	}
}

void insertion_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int* read_count, int* write_count, int* comp_count) {
	int key;
	int j;
	// Insertion sort algorithm
	for (int i = 1; i < ARRAY_SIZE; i++) {
		key = array[i];
		j = i - 1;
		(*read_count)++;
		while (j >= 0 && array[j] > key) {
			event_handler(event, renderer, window);
			setWindowTitleAccordingToFramerateAndTimeElapsed(window);
			array[j + 1] = array[j];
			j = j - 1;
			(*read_count)++;
			(*write_count)++;
			(*comp_count)++;

			// highlight the two bars being compared
			SDL_Rect rect1 = { (j + 1) * BAR_WIDTH, WINDOW_HEIGHT - array[j + 1], BAR_WIDTH, array[j + 1] };
			SDL_Rect rect2 = { j * BAR_WIDTH, WINDOW_HEIGHT - array[j], BAR_WIDTH, array[j] };

			// increase read count
			(*read_count) += 2;

			// Color bars being compared in red
			SDL_SetRenderDrawColor(renderer, compClr.r, compClr.g, compClr.b, 255);
			SDL_RenderFillRect(renderer, &rect1);
			SDL_RenderFillRect(renderer, &rect2);

			// Update window surface to show changes
			SDL_RenderPresent(renderer);

			// Redraw the entire array (clear previous coloring)
			SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
			draw_bars(renderer, array);

			// Update window surface
			SDL_RenderPresent(renderer);
		}
		array[j + 1] = key;
		(*write_count)++;
	}
}

int partition(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int low, int high, int* read_count, int* write_count, int* comp_count) {
	int pivot = array[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		event_handler(event, renderer, window);
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);

		SDL_Rect rect1 = { j * BAR_WIDTH, WINDOW_HEIGHT - array[j], BAR_WIDTH, array[j] };
		SDL_Rect rect2 = { high * BAR_WIDTH, WINDOW_HEIGHT - array[high], BAR_WIDTH, array[high] };

		SDL_SetRenderDrawColor(renderer, compClr.r, compClr.g, compClr.b, 255);
		SDL_RenderFillRect(renderer, &rect1);
		SDL_RenderFillRect(renderer, &rect2);

		SDL_RenderPresent(renderer);

		(*read_count) += 2;
		(*comp_count)++;
		if (array[j] < pivot) {
			i++;
			std::swap(array[i], array[j]);
			(*write_count) += 3;
		}

		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);

		SDL_RenderPresent(renderer);
	}
	std::swap(array[i + 1], array[high]);
	(*write_count) += 3;

	return (i + 1);
}

void quick_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int low, int high, int* read_count, int* write_count, int* comp_count) {
	if (low < high) {
		int pi = partition(renderer, window, event, array, low, high, read_count, write_count, comp_count);

		quick_sort_visualized(renderer, window, event, array, low, pi - 1, read_count, write_count, comp_count);
		quick_sort_visualized(renderer, window, event, array, pi + 1, high, read_count, write_count, comp_count);
	}
}

void counting_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int* read_count, int* write_count, int* comp_count) {
	int max = array[0];
	int min = array[0];
	for (int i = 1; i < ARRAY_SIZE; i++) {
		if (array[i] > max) {
			max = array[i];
		}
		if (array[i] < min) {
			min = array[i];
		}
		(*read_count) += 2;
	}

	int range = max - min + 1;
	int* count = new int[range];
	int* output = new int[ARRAY_SIZE];

	for (int i = 0; i < range; i++) {
		count[i] = 0;
	}
	for (int i = 0; i < ARRAY_SIZE; i++) {
		count[array[i] - min]++;
		(*read_count)++;
	}

	for (int i = 1; i < range; i++) {
		count[i] += count[i - 1];
	}

	for (int i = ARRAY_SIZE - 1; i >= 0; i--) {
		event_handler(event, renderer, window);
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);

		output[count[array[i] - min] - 1] = array[i];
		count[array[i] - min]--;
		(*read_count)++;
		(*write_count)++;

		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, output);

		SDL_RenderPresent(renderer);
	}

	for (int i = 0; i < ARRAY_SIZE; i++) {
		event_handler(event, renderer, window);
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);
		array[i] = output[i];
		(*write_count)++;

		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);

		SDL_RenderPresent(renderer);
	}

	delete[] count;
	delete[] output;
}

void merge(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int left, int mid, int right, int* read_count, int* write_count, int* comp_count) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	int* L = new int[n1];
	int* R = new int[n2];

	for (int i = 0; i < n1; i++) {
		L[i] = array[left + i];
		(*read_count)++;
	}
	for (int j = 0; j < n2; j++) {
		R[j] = array[mid + 1 + j];
		(*read_count)++;
	}

	int i = 0;
	int j = 0;
	int k = left;

	while (i < n1 && j < n2) {
		event_handler(event, renderer, window);
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);

		SDL_Rect rect1 = { k * BAR_WIDTH, WINDOW_HEIGHT - array[k], BAR_WIDTH, array[k] };
		SDL_SetRenderDrawColor(renderer, compClr.r, compClr.g, compClr.b, 255);
		SDL_RenderFillRect(renderer, &rect1);
		SDL_RenderPresent(renderer);

		(*comp_count)++;
		if (L[i] <= R[j]) {
			array[k] = L[i];
			i++;
		}
		else {
			array[k] = R[j];
			j++;
		}
		(*write_count)++;
		k++;

		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);
		SDL_RenderPresent(renderer);
	}

	while (i < n1) {
		event_handler(event, renderer, window);
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);

		array[k] = L[i];
		(*write_count)++;
		i++;
		k++;

		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);
		SDL_RenderPresent(renderer);
	}

	while (j < n2) {
		event_handler(event, renderer, window);
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);

		array[k] = R[j];
		(*write_count)++;
		j++;
		k++;

		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);
		SDL_RenderPresent(renderer);
	}

	delete[] L;
	delete[] R;
}

void merge_sort_visualized(SDL_Renderer* renderer, SDL_Window* window, SDL_Event* event, int* array, int left, int right, int* read_count, int* write_count, int* comp_count) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		merge_sort_visualized(renderer, window, event, array, left, mid, read_count, write_count, comp_count);
		merge_sort_visualized(renderer, window, event, array, mid + 1, right, read_count, write_count, comp_count);

		merge(renderer, window, event, array, left, mid, right, read_count, write_count, comp_count);
	}
}

int main() {

	// read the "ARRAY_SIZE" value from the "settings.txt" file
	ARRAY_SIZE = readArraySizeFromFile();
	if (ARRAY_SIZE == -1 || ARRAY_SIZE > WINDOW_WIDTH || WINDOW_WIDTH % ARRAY_SIZE != 0) {
		ARRAY_SIZE = 100;
	}

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
	}
	else {
		printf("SDL initialized successfully!\n");
	}

	// event
	SDL_Event e;

	// Initialize SDL_ttf
	if (TTF_Init() < 0) {
		printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
		SDL_Quit();
		return -1;
	}
	else {
		printf("SDL_ttf initialized successfully!\n");
	}

	// Initialize SDL_image
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
		TTF_Quit();
		SDL_Quit();
		return -1;
	}
	else {
		printf("SDL_image initialized successfully!\n");
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
	else if (SORTING_ALGORITHM == 4) {
		char window_title[] = "Quick Sort Visualizer";
		printf("Quick Sort Visualizer\n");
	}
	else if (SORTING_ALGORITHM == 5) {
		char window_title[] = "Counting Sort Visualizer";
		printf("Counting Sort Visualizer\n");
	}
	else if (SORTING_ALGORITHM == 6) {
		char window_title[] = "Merge Sort Visualizer";
		printf("Merge Sort Visualizer\n");
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
		IMG_Quit();
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
		IMG_Quit();
		SDL_Quit();
		return -1;
	}
	else {
		printf("Renderer created successfully!\n");
	}

    // Initialize random seed
    srand(static_cast<unsigned int>(time(NULL)));

	while (true) {

		// screen flush
		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);

		// display some text detailing user inputs for selection of the sorting algorithm
		TTF_Font* font = TTF_OpenFont("cm.ttf", 20);
		if (!font) {
			printf("TTF_OpenFont: %s\n", TTF_GetError());
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Color textColor = { 0, 182, 252 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, "1 for Bubble Sort, 2 for Selection Sort, 3 for Insertion Sort", textColor);
		if (!textSurface) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!textTexture) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

		// second line of text
		SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "4 for Quick Sort, 5 for Counting Sort, 6 for Merge Sort", textColor);
		if (!textSurface2) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
		if (!textTexture2) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRect2 = { 10, 30, textSurface2->w, textSurface2->h };
		SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);

		// display some more text regarding the settings file and array size values
		textColor = { 255, 0, 0 };
		char text[100];
		sprintf_s(text, "ARRAY_SIZE: %d , WINDOW_WIDTH: %d", ARRAY_SIZE, WINDOW_WIDTH);
		SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, text, textColor);
		if (!textSurface3) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
		if (!textTexture3) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRect3 = { 10, 70, textSurface3->w, textSurface3->h };
		SDL_RenderCopy(renderer, textTexture3, NULL, &textRect3);

		// display text regarding settings file
		SDL_Surface* textSurface4 = TTF_RenderText_Solid(font, "Settings file: settings.txt, AVAILABLE SETTINGS: ARRAY_SIZE", textColor);
		if (!textSurface4) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyTexture(textTexture3);
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTexture4 = SDL_CreateTextureFromSurface(renderer, textSurface4);
		if (!textTexture4) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurface4);
			SDL_DestroyTexture(textTexture3);
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRect4 = { 10, 90, textSurface4->w, textSurface4->h };
		SDL_RenderCopy(renderer, textTexture4, NULL, &textRect4);

		// display text regarding the program
		SDL_Surface* textSurface5 = TTF_RenderText_Solid(font, "ARRAY_SIZE<=WINDOW_WIDTH&&WINDOW_WIDTH%ARRAY_SIZE==0", textColor);
		if (!textSurface5) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyTexture(textTexture4);
			SDL_FreeSurface(textSurface4);
			SDL_DestroyTexture(textTexture3);
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTexture5 = SDL_CreateTextureFromSurface(renderer, textSurface5);
		if (!textTexture5) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurface5);
			SDL_DestroyTexture(textTexture4);
			SDL_FreeSurface(textSurface4);
			SDL_DestroyTexture(textTexture3);
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRect5 = { 10, 110, textSurface5->w, textSurface5->h };
		SDL_RenderCopy(renderer, textTexture5, NULL, &textRect5);

		// render an image below the text
		SDL_Surface* imageSurface = IMG_Load("sdl.png");
		if (!imageSurface) {
			printf("IMG_Load: %s\n", IMG_GetError());
			SDL_DestroyTexture(textTexture5);
			SDL_FreeSurface(textSurface5);
			SDL_DestroyTexture(textTexture4);
			SDL_FreeSurface(textSurface4);
			SDL_DestroyTexture(textTexture3);
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
		if (!imageTexture) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(imageSurface);
			SDL_DestroyTexture(textTexture5);
			SDL_FreeSurface(textSurface5);
			SDL_DestroyTexture(textTexture4);
			SDL_FreeSurface(textSurface4);
			SDL_DestroyTexture(textTexture3);
			SDL_FreeSurface(textSurface3);
			SDL_DestroyTexture(textTexture2);
			SDL_FreeSurface(textSurface2);
			SDL_DestroyTexture(textTexture);
			SDL_FreeSurface(textSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect imageRect = { 80, 150, imageSurface->w, imageSurface->h };
		SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);

		// present the renderer
		SDL_RenderPresent(renderer);

		// sorting algorithm selection
		bool event_loop_active = true;
		while (event_loop_active) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					SDL_FreeSurface(textSurface);
					SDL_DestroyTexture(textTexture);
					SDL_FreeSurface(textSurface2);
					SDL_DestroyTexture(textTexture2);
					SDL_FreeSurface(textSurface3);
					SDL_DestroyTexture(textTexture3);
					SDL_FreeSurface(textSurface4);
					SDL_DestroyTexture(textTexture4);
					SDL_FreeSurface(textSurface5);
					SDL_DestroyTexture(textTexture5);
					SDL_FreeSurface(imageSurface);
					SDL_DestroyTexture(imageTexture);
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					TTF_CloseFont(font);
					TTF_Quit();
					IMG_Quit();
					SDL_Quit();
					return 0;
				}
				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_1:
						SORTING_ALGORITHM = 1;
						event_loop_active = false;
						break;
					case SDLK_2:
						SORTING_ALGORITHM = 2;
						event_loop_active = false;
						break;
					case SDLK_3:
						SORTING_ALGORITHM = 3;
						event_loop_active = false;
						break;
					case SDLK_4:
						SORTING_ALGORITHM = 4;
						event_loop_active = false;
						break;
					case SDLK_5:
						SORTING_ALGORITHM = 5;
						event_loop_active = false;
						break;
					case SDLK_6:
						SORTING_ALGORITHM = 6;
						event_loop_active = false;
						break;
					}
					break;
				}
			}
		}

		char window_title_inter[] = "Sorting Visualizer";
		if (SORTING_ALGORITHM == 1) {
			char window_title_inter[] = "Bubble Sort Visualizer";
			printf("Bubble Sort Visualizer\n");
		}
		else if (SORTING_ALGORITHM == 2) {
			char window_title_inter[] = "Selection Sort Visualizer";
			printf("Selection Sort Visualizer\n");
		}
		else if (SORTING_ALGORITHM == 3) {
			char window_title_inter[] = "Insertion Sort Visualizer";
			printf("Insertion Sort Visualizer\n");
		}
		else if (SORTING_ALGORITHM == 4) {
			char window_title_inter[] = "Quick Sort Visualizer";
			printf("Quick Sort Visualizer\n");
		}
		else if (SORTING_ALGORITHM == 5) {
			char window_title_inter[] = "Counting Sort Visualizer";
			printf("Counting Sort Visualizer\n");
		}
		else if (SORTING_ALGORITHM == 6) {
			char window_title_inter[] = "Merge Sort Visualizer";
			printf("Merge Sort Visualizer\n");
		}
		SDL_SetWindowTitle(window, window_title_inter);

		// after the user has selected the sorting algorithm, clear the screen and destroy font, surface, and texture
		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		SDL_FreeSurface(textSurface2);
		SDL_DestroyTexture(textTexture2);
		SDL_FreeSurface(textSurface3);
		SDL_DestroyTexture(textTexture3);
		SDL_FreeSurface(textSurface4);
		SDL_DestroyTexture(textTexture4);
		SDL_FreeSurface(textSurface5);
		SDL_DestroyTexture(textTexture5);
		SDL_FreeSurface(imageSurface);
		SDL_DestroyTexture(imageTexture);
		TTF_CloseFont(font);

		// Generate an array of random heights
		int* array = new int[ARRAY_SIZE];
		for (int i = 0; i < ARRAY_SIZE; i++) {
			array[i] = rand() % (WINDOW_HEIGHT - 50) + 1;  // Random height between 1 and WINDOW_HEIGHT
		}

		// Draw initial bars
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);
		SDL_RenderPresent(renderer);
		for (int i = 0; i <= 1000; i++) {
			event_handler(&e, renderer, window);
			SDL_Delay(1);
		}

		// define array read, write and comparison counts
		int read_count = 0;
		int write_count = 0;
		int comp_count = 0;

		// get time at start of program
		auto start = std::chrono::high_resolution_clock::now();

		// Perform the bubble sort with visualization
		if (SORTING_ALGORITHM == 1) {
			bubble_sort_visualized(renderer, window, &e, array, &read_count, &write_count, &comp_count);
		}
		else if (SORTING_ALGORITHM == 2) {
			selection_sort_visualized(renderer, window, &e, array, &read_count, &write_count, &comp_count);
		}
		else if (SORTING_ALGORITHM == 3) {
			insertion_sort_visualized(renderer, window, &e, array, &read_count, &write_count, &comp_count);
		}
		else if (SORTING_ALGORITHM == 4) {
			quick_sort_visualized(renderer, window, &e, array, 0, ARRAY_SIZE - 1, &read_count, &write_count, &comp_count);
		}
		else if (SORTING_ALGORITHM == 5) {
			counting_sort_visualized(renderer, window, &e, array, &read_count, &write_count, &comp_count);
		}
		else if (SORTING_ALGORITHM == 6) {
			merge_sort_visualized(renderer, window, &e, array, 0, ARRAY_SIZE - 1, &read_count, &write_count, &comp_count);
		}
		else {
			printf("Invalid sorting algorithm selected!\n");
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		// get time at end of program
		auto end = std::chrono::high_resolution_clock::now();
		SDL_RenderPresent(renderer);
		// draw the final bars with a different color
		SDL_SetRenderDrawColor(renderer, windowClr.r, windowClr.g, windowClr.b, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, barClr.r, barClr.g, barClr.b, 255);
		draw_bars(renderer, array);
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, barFinalClr.r, barFinalClr.g, barFinalClr.b, 255);
		for (int i = 0; i < ARRAY_SIZE; i++) {
			event_handler(&e, renderer, window);
			SDL_Rect rect = { i * BAR_WIDTH, WINDOW_HEIGHT - array[i], BAR_WIDTH, array[i] };
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderPresent(renderer);
			SDL_Delay(BAR_WIDTH);
		}
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
		else if (SORTING_ALGORITHM == 4) {
			sprintf_s(window_title_final, "Quick Sort Visualizer - Time Elapsed: %f seconds", elapsed.count());
		}
		else if (SORTING_ALGORITHM == 5) {
			sprintf_s(window_title_final, "Counting Sort Visualizer - Time Elapsed: %f seconds", elapsed.count());
		}
		else if (SORTING_ALGORITHM == 6) {
			sprintf_s(window_title_final, "Merge Sort Visualizer - Time Elapsed: %f seconds", elapsed.count());
		}
		else {
			sprintf_s(window_title_final, "Sorting Visualizer - Time Elapsed: %f seconds", elapsed.count());
		}
		SDL_SetWindowTitle(window, window_title_final);

		// display some text detailing user inputs to continue or exit the program
		TTF_Font* fontF = TTF_OpenFont("cm.ttf", 20);
		if (!fontF) {
			printf("TTF_OpenFont: %s\n", TTF_GetError());
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Color textColorFinal = { 0, 255, 0 };
		SDL_Surface* textSurfaceF = TTF_RenderText_Solid(fontF, "Enter to continue, Space or Escape to exit", textColorFinal);
		if (!textSurfaceF) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(fontF);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTextureF = SDL_CreateTextureFromSurface(renderer, textSurfaceF);
		if (!textTextureF) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurfaceF);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(fontF);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRectF = { 10, 10, textSurfaceF->w, textSurfaceF->h };
		SDL_RenderCopy(renderer, textTextureF, NULL, &textRectF);

		// display more text detailing the number of reads, writes, and comparisons
		char textFinal[100];
		sprintf_s(textFinal, "Reads: %d, Writes: %d, Comparisons: %d", read_count, write_count, comp_count);
		SDL_Surface* textSurfaceFinal = TTF_RenderText_Solid(fontF, textFinal, textColorFinal);
		if (!textSurfaceFinal) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyTexture(textTextureF);
			SDL_FreeSurface(textSurfaceF);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(fontF);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTextureFinal = SDL_CreateTextureFromSurface(renderer, textSurfaceFinal);
		if (!textTextureFinal) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurfaceFinal);
			SDL_DestroyTexture(textTextureF);
			SDL_FreeSurface(textSurfaceF);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(fontF);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRectFinal = { 10, 30, textSurfaceFinal->w, textSurfaceFinal->h };
		SDL_RenderCopy(renderer, textTextureFinal, NULL, &textRectFinal);

		// display some more text detailing the time elapsed
		char textFinalTime[100];
		sprintf_s(textFinalTime, "Time Elapsed: %f seconds", elapsed.count());
		SDL_Surface* textSurfaceFinalTime = TTF_RenderText_Solid(fontF, textFinalTime, textColorFinal);
		if (!textSurfaceFinalTime) {
			printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
			SDL_DestroyTexture(textTextureFinal);
			SDL_FreeSurface(textSurfaceFinal);
			SDL_DestroyTexture(textTextureF);
			SDL_FreeSurface(textSurfaceF);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(fontF);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Texture* textTextureFinalTime = SDL_CreateTextureFromSurface(renderer, textSurfaceFinalTime);
		if (!textTextureFinalTime) {
			printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
			SDL_FreeSurface(textSurfaceFinalTime);
			SDL_DestroyTexture(textTextureFinal);
			SDL_FreeSurface(textSurfaceFinal);
			SDL_DestroyTexture(textTextureF);
			SDL_FreeSurface(textSurfaceF);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(fontF);
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return -1;
		}
		SDL_Rect textRectFinalTime = { 10, 50, textSurfaceFinalTime->w, textSurfaceFinalTime->h };
		SDL_RenderCopy(renderer, textTextureFinalTime, NULL, &textRectFinalTime);

		// present the renderer
		SDL_RenderPresent(renderer);

		// get user input to continue or exit the program
		bool continue_loop = true;
		bool secondary_loop = true;
		while (secondary_loop) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					SDL_FreeSurface(textSurfaceF);
					SDL_DestroyTexture(textTextureF);
					SDL_FreeSurface(textSurfaceFinal);
					SDL_DestroyTexture(textTextureFinal);
					SDL_FreeSurface(textSurfaceFinalTime);
					SDL_DestroyTexture(textTextureFinalTime);
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					TTF_CloseFont(fontF);
					TTF_Quit();
					IMG_Quit();
					SDL_Quit();
					delete[] array;
					return 0;
				}
				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
						case SDLK_RETURN:
							secondary_loop = false;
							SDL_FreeSurface(textSurfaceF);
							SDL_DestroyTexture(textTextureF);
							SDL_FreeSurface(textSurfaceFinal);
							SDL_DestroyTexture(textTextureFinal);
							SDL_FreeSurface(textSurfaceFinalTime);
							SDL_DestroyTexture(textTextureFinalTime);
							TTF_CloseFont(fontF);
							delete[] array;
							break;
						case SDLK_SPACE:
							continue_loop = false;
							secondary_loop = false;
							SDL_FreeSurface(textSurfaceF);
							SDL_DestroyTexture(textTextureF);
							SDL_FreeSurface(textSurfaceFinal);
							SDL_DestroyTexture(textTextureFinal);
							SDL_FreeSurface(textSurfaceFinalTime);
							SDL_DestroyTexture(textTextureFinalTime);
							TTF_CloseFont(fontF);
							delete[] array;
							break;
						case SDLK_ESCAPE:
							secondary_loop = false;
							continue_loop = false;
							SDL_FreeSurface(textSurfaceF);
							SDL_DestroyTexture(textTextureF);
							SDL_FreeSurface(textSurfaceFinal);
							SDL_DestroyTexture(textTextureFinal);
							SDL_FreeSurface(textSurfaceFinalTime);
							SDL_DestroyTexture(textTextureFinalTime);
							SDL_DestroyRenderer(renderer);
							SDL_DestroyWindow(window);
							TTF_CloseFont(fontF);
							TTF_Quit();
							IMG_Quit();
							SDL_Quit();
							delete[] array;
							return 0;
							break;
					}
				}
			}
		}
		if (!continue_loop) {
			break;
		}
	}

    // Clean up and close the program
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
	IMG_Quit();
    SDL_Quit();
    return 0;
}
