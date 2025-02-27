#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

#define DELIMITERS " \n\t\r"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"

#define MAX_WORD_LENGTH 100
#define BUFFER_SIZE 1024 // Buffer size for reading lines from the file


//----------------------------------------------------------------------------------------------------------------------

// prints error message and frees all memory
void end_memory_failed(MarkovChain *Chain)
{
    printf(ALLOCATION_ERROR_MASSAGE);
    free_database(&Chain);
};

//----------------------------------------------------------------------------------------------------------------------

//
/**
 * reads the file and fills the MarkovChain's database.
 * @param *file the path to the opened file
 * @param *markov_chain the markov chain to fill
 * @param words_to_read the max number words to read from the file
 * @return 0 if success, 1 if failed
 */
int fill_database(FILE *file, int words_to_read , MarkovChain *markov_chain) {
    char word[MAX_WORD_LENGTH];
    int words_read = 0;
    MarkovNode *previous_node = NULL;

    // Reset file pointer to beginning
    rewind(file);

    // Continue reading until words_to_read or file end
    while ((words_to_read == -1 || words_read < words_to_read) && fscanf(file, "%99s", word) == 1)
    {
        // Add word to database, creating a new node if it doesn't exist
        Node *current_node = add_to_database(markov_chain, word);
        if (current_node == NULL) {
            return 1;
        }

        // If we have a previous node, add this node to its frequency list
        if (previous_node != NULL) {
            if (add_node_to_frequency_list(previous_node, current_node->data) != 0) {
                // Failed to add to frequency list
                return 1;
            }
        }

        // Update previous node for next iteration
        previous_node = current_node->data;
        words_read++;

        // Clear word buffer for next iteration
        memset(word, 0, MAX_WORD_LENGTH);

    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

//void print_markov_chain(MarkovChain *markov_chain) {
//    if (markov_chain == NULL || markov_chain->database == NULL) {
//        printf("Empty Markov Chain\n");
//        return;
//    }
//
//    Node *current_node = markov_chain->database->first;
//    int node_count = 0;
//
//    printf("\n Markov Chain Database:\n");
//    printf("-------------------\n");
//
//    while (current_node != NULL) {
//
//        printf("- %s\n", current_node->data->data);
//        node_count++;
//
//        // Print frequency list
//        for (int i = 0; i < current_node->data->frequency_list_size; i++) {
//            printf("\t- %s %d\n",
//                   current_node->data->frequency_list[i].markov_node->data,
//                   current_node->data->frequency_list[i].frequency);
//        }
//
//        current_node = current_node->next;
//    }
//
//    printf("\n Total Nodes: %d\n\n", node_count);
//}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    // checking that the right number of arguments were received
    if (argc < 4 || argc > 5) {
        printf(NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }

    // Parse arguments
    int seed = atoi(argv[1]); // seed for the srand() function
    int num_tweets = atoi(argv[2]); // number of new tweets to create
    const char *file_path = argv[3]; // path to tweets file
    int words_to_read = (argc == 5) ? atoi(argv[4]) : -1; // max words to read from the file. if not given as an argument, read the whole file


    // Set the random seed
    srand(seed);

    // Open the tweets file
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf(FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }

    // Initialize the Markov Chain
    MarkovChain *Chain = initialize_MarkovChain();
    if (Chain == NULL) {
        fclose(file);
        end_memory_failed(Chain);
        return EXIT_FAILURE;
    }

    // Read the corpus file and fill the Markov Chain
    if (fill_database(file, words_to_read, Chain) != 0)
    {
        end_memory_failed(Chain);
        return EXIT_FAILURE;
    }
    fclose(file);

    // print_markov_chain(Chain); if we want to see the markov chain created

    // Generate and print the tweets
    for (int i = 1; i <= num_tweets; i++) {
        printf("Tweet %d: ", i);
        generate_tweet(get_first_random_node(Chain),20);
        printf("\n");

    }
    // Clean up
    free_database(&Chain);
    return EXIT_SUCCESS;
}