#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_INGREDIENTS 100

typedef struct {
    char* ingredients[MAX_INGREDIENTS];
    int numIngredients;

    char* allergens[10];
    int numAllergens;
} Food;

typedef struct {
    char* allergen;
    char* ingredient;
} AllergenIngredient;

// Sort allergenic ingredients based on the allergen name
int allergenIngredientCmp(const void * elem1, const void * elem2) {
    AllergenIngredient allergenIngredient1 = (*(AllergenIngredient*)elem1);
    AllergenIngredient allergenIngredient2 = (*(AllergenIngredient*)elem2);
    return strcmp(allergenIngredient1.allergen, allergenIngredient2.allergen);
}

char* getIngredientWithAllergen(Food* foods[50], char* allergen, int numFoods, int numAllergens, AllergenIngredient allergenIngredients[numAllergens]) {
    char* potentialIngredients[MAX_INGREDIENTS] = {0};
    int numPotentialIngredients = 0;

    // First, find the first food with the allergen listed
    int foodIndex = 0;
    int foundAllergenFood = 0;
    while (!foundAllergenFood) {
        Food* food = foods[foodIndex++];

        for (int i = 0; i < food->numAllergens; i++) {
            if (strcmp(allergen, food->allergens[i]) == 0) {
                foundAllergenFood = 1;

                // Start with this set of food
                memcpy(potentialIngredients, food->ingredients, food->numIngredients * sizeof(char*));
                numPotentialIngredients = food->numIngredients;

                break;
            }
        }
    }

    // Now we do food ingredient intersections
    int maxCheckLength = numPotentialIngredients;

    // Do an initial pass of potential ingredients to see if any allergens were already found
    for (int i = 0; i < maxCheckLength; i++) {
        char* potentialIngredient = potentialIngredients[i];

        int alreadyFound = 0;
        for (int j = 0; j < numAllergens; j++) {
            char* allergenIngredient = allergenIngredients[j].ingredient;

            if (allergenIngredient != NULL && strcmp(potentialIngredient, allergenIngredient) == 0) {
                alreadyFound = 1;
                break;
            }
        }

        if (alreadyFound) {
            potentialIngredients[i] = NULL;
            numPotentialIngredients--;
        }
    }

    while (foodIndex < numFoods && numPotentialIngredients > 1) {
        Food* foodCheck = foods[foodIndex++];

        // If the food doesn't have the allergen in question, skip it
        int hasAllergen = 0;
        for (int j = 0; j < foodCheck->numAllergens; j++) {
            if (strcmp(allergen, foodCheck->allergens[j]) == 0) {
                hasAllergen = 1;
                break;
            }
        }

        if (!hasAllergen) {
            continue;
        }

        // Not sorted, so would be kind of slow
        for (int i = 0; i < maxCheckLength; i++) {
            // Skip crossed out ingredients
            char* currPotentialIngredient = potentialIngredients[i];
            if (currPotentialIngredient == NULL) {
                continue;
            }

            // If the checked food contains potential ingredient, it stays (intersection)
            int found = 0;
            for (int j = 0; j < foodCheck->numIngredients; j++) {
                if (strcmp(currPotentialIngredient, foodCheck->ingredients[j]) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                potentialIngredients[i] = NULL;
                numPotentialIngredients--;
            }
        }
    }

    // Multiple potentials means we should process others first
    if (numPotentialIngredients > 1) {
        return NULL;
    }

    // Pick out and return the remaining ingredient
    char* ingredient = NULL;
    for (int i = 0; i < maxCheckLength; i++) {
        if (potentialIngredients[i] != NULL) {
            ingredient = potentialIngredients[i];
            break;
        }
    }
    return ingredient;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        Food* foods[50] = {0};
        int numFoods = 0;

        char* allergens[10] = {0};
        int allergenCount = 0;

        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            Food* food = calloc(1, sizeof(Food));

            int readingAllergens = 0;
            while (line != NULL) {
                char* piece = strsep(&line, " ");

                if (strcmp(piece, "(contains") == 0) {
                    readingAllergens = 1;
                    continue;
                }

                if (!readingAllergens) {
                    food->ingredients[food->numIngredients++] = piece;
                } else {
                    piece[strlen(piece) - 1] = '\0';
                    food->allergens[food->numAllergens++] = piece;

                    // Add to allergen list
                    int duplicate = 0;
                    for (int i = 0; i < allergenCount; i++) {
                        if (strcmp(allergens[i], piece) == 0) {
                            duplicate = 1;
                            break;
                        }
                    }

                    if (!duplicate) {
                        allergens[allergenCount++] = piece;
                    }
                }
            }

            foods[numFoods++] = food;
        }

        // In theory, if foods are sorted by ingredient length, then the allergen processing might be faster
        // Cause the initial ingredient set is as small as possible
        // qsort(foods, numFoods, sizeof(Food*), foodCmp);

        AllergenIngredient allergenIngredients[allergenCount];
        memset(allergenIngredients, 0, sizeof(allergenIngredients));
        int matchedIngredients = 0;

        // Keep looping over the whole allergen set until we have an ingredient per allergen
        while (matchedIngredients != allergenCount) {
            for (int i = 0; i < allergenCount; i++) {
                if (allergenIngredients[i].ingredient != NULL) {
                    continue;
                }

                char* ingredient = getIngredientWithAllergen(foods, allergens[i], numFoods, allergenCount, allergenIngredients);

                if (ingredient != NULL) {
                    allergenIngredients[i].allergen = allergens[i];
                    allergenIngredients[i].ingredient = ingredient;
                    matchedIngredients++;
                }
            }
        }

        // We should have a list of every allergen ingredient now
        // Count all the food ingredients that aren't allergens
        int count = 0;
        for (int i = 0; i < numFoods; i++) {
            Food* food = foods[i];

            for (int j = 0; j < food->numIngredients; j++) {
                char* ingredient = food->ingredients[j];
                
                int isAllergen = 0;
                for (int k = 0; k < allergenCount; k++) {
                    char* allergenIngredient = allergenIngredients[k].ingredient;

                    if (strcmp(ingredient, allergenIngredient) == 0) {
                        isAllergen = 1;
                        break;
                    }
                }

                if (!isAllergen) {
                    count++;
                }
            }

            // Don't need food anymore past this point
            free(food);
        }

        printf("Part 1 count: %d\n", count);

        printf("Part 2 (sorted ingredients by allergen): ");
        qsort(allergenIngredients, allergenCount, sizeof(AllergenIngredient), allergenIngredientCmp);
        for (int i = 0; i < allergenCount; i++) {
            printf("%s", allergenIngredients[i].ingredient);

            if (i < allergenCount - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    free(startBuffer);
}
