#include "markov_chain.h"


/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Get a string str.
 * @param str
 * @return 1 if the string ends with '.', else returns 0
 */

int is_last_word(const char* str)
{
    // in case the str is NULL or length = 0
    if ((str == NULL) || (strlen(str) == 0) ) {
        return 0;
    }
    // Find the length of the string
    int len = strlen(str);

    // Check if the last character is '.'
    return (str[len - 1] == '.') ? 1 : 0;
}

//----------------------------------------------------------------------------------------------------------------------

// Function to create and initialize a MarkovChain, if unsuccessful return NULL
MarkovChain *initialize_MarkovChain() {
    // Allocate memory for the MarkovChain struct
    MarkovChain *chain = malloc(sizeof(MarkovChain));
    if (chain == NULL) {
        printf (ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }

    // Allocate memory for the LinkedList inside the MarkovChain
    chain->database = malloc(sizeof(LinkedList));
    if (chain->database == NULL) {
        printf (ALLOCATION_ERROR_MASSAGE);
        free(chain); // Free the MarkovChain struct before returning
        return NULL;
    }

    // Initialize the LinkedList's fields
    chain->database->first = NULL;
    chain->database->last = NULL;
    chain->database->size = 0;

    return chain;
}

//----------------------------------------------------------------------------------------------------------------------
/**
* Check if data_ptr is in database. If so, return the Node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the data to look for
 * @return Pointer to the Node wrapping given data, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr) {
    // Check for NULL inputs
    if (markov_chain == NULL || markov_chain->database == NULL || data_ptr == NULL) {
        return NULL;
    }
    // Start at the first node of the linked list
    Node *current = markov_chain->database->first;

    // Traverse the entire linked list
    while (current != NULL) {
        // Check if the current node's data matches the input data
        if (strcmp(current->data->data, data_ptr) == 0) {
            return current;
        }
        // Move to the next node
        current = current->next;
    }

    // If no matching node is found, return NULL
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the data to look for
 * @return Node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    // First, check if the node already exists in the database
    Node *existing_node = get_node_from_database(markov_chain, data_ptr);
    if (existing_node != NULL) {
        return existing_node;
    }

    // If not existing, create a new MarkovNode
    MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
    if (new_markov_node == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Deep copy the string to ensure proper memory management
    new_markov_node->data = malloc(strlen(data_ptr) + 1);
    if (new_markov_node->data == NULL) {
        free(new_markov_node);
        return NULL;  // Memory allocation failed
    }
    strcpy(new_markov_node->data, data_ptr);

    // Initialize frequency list
    new_markov_node->frequency_list = NULL;
    new_markov_node->frequency_list_size = 0;

    // Add to the database
    if (add(markov_chain->database, new_markov_node) != 0) {
        // If add fails, free allocated memory
        free(new_markov_node->data);
        free(new_markov_node);
        return NULL;
    }

    return get_node_from_database(markov_chain, data_ptr);
}

//Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {
//    // First, check if the node already exists in the database
//    Node *existing_node = get_node_from_database(markov_chain, data_ptr);
//    if (existing_node != NULL) {
//        // printf("Existing node found for %s\n", data_ptr);
//        return existing_node;
//    }
//
//    // Create a new MarkovNode
//    MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
//    if (new_markov_node == NULL) {
//        printf("Failed to allocate MarkovNode for %s\n", data_ptr);
//        return NULL;
//    }
//
//    // Deep copy the string
//    new_markov_node->data = malloc(strlen(data_ptr) + 1);
//    if (new_markov_node->data == NULL) {
//        printf("Failed to allocate data for %s\n", data_ptr);
//        free(new_markov_node);
//        return NULL;
//    }
//    strcpy(new_markov_node->data, data_ptr);
//
//    // Initialize frequency list
//    new_markov_node->frequency_list = NULL;
//    new_markov_node->frequency_list_size = 0;
//
//    // Add to database using existing add function
//    if (add(markov_chain->database, new_markov_node) != 0) {
//        printf("Failed to add %s to database\n", data_ptr);
//        free(new_markov_node->data);
//        free(new_markov_node);
//        return NULL;
//    }
//
//    return get_node_from_database(markov_chain, data_ptr);
//}


//----------------------------------------------------------------------------------------------------------------------

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: 0 if the process was successful, 1 if in
 * case of allocation error.
 */

int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node) {
    // Check for NULL inputs
    if (first_node == NULL || second_node == NULL) {
        return 1;
    }

    // check if last word (ends with '.'). if so, frequency list will remain NULL and end function
    if (is_last_word(first_node->data) == 1)
    {
        // first_node->frequency_list == NULL;
        return 0; // returning 0 because its 'success' - the process does with it is supposed to do -> ending word's frequency list = NULL
    }

    // Check if frequency list exists, if not, create it
    if (first_node->frequency_list == NULL) {
        first_node->frequency_list = malloc(sizeof(MarkovNodeFrequency*));
        if (first_node->frequency_list == NULL) {
            return 1; // malloc failed
        }
        first_node->frequency_list_size = 0;
    }

    // Check if the second_node word is already in the frequency list
    for (int i = 0; i < first_node->frequency_list_size; i++) {
        if (strcmp(first_node->frequency_list[i].markov_node->data, second_node->data) == 0) {
            // If found, increase frequency
            first_node->frequency_list[i].frequency++;
            return 0; // success
        }
    }

    // If not found, add new word to frequency list
    MarkovNodeFrequency *temp = realloc(first_node->frequency_list,
                                         (first_node->frequency_list_size + 1) * sizeof(MarkovNodeFrequency));
    if (temp == NULL) {
        return 1; // realloc failed
    }
    first_node->frequency_list = temp;


    // Create new frequency entry
    first_node->frequency_list[first_node->frequency_list_size].markov_node = second_node;
    first_node->frequency_list[first_node->frequency_list_size].frequency = 1;
    first_node->frequency_list_size++;

    return 0; // success
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
//Free markov_chain and all of it's content from memory

void free_database(MarkovChain **ptr_chain) {
    if (ptr_chain == NULL || *ptr_chain == NULL) {
        return;
    }

    // Free each node in the database
    Node *current = (*ptr_chain)->database->first;
    while (current != NULL) {
        MarkovNode *markov_node = current->data;

        // Free the word
        free(markov_node->data);
        markov_node->data = NULL;

        // Free frequency list
        free(markov_node->frequency_list);
        markov_node->frequency_list = NULL;

        // Free the markov node
        free(markov_node);
        markov_node = NULL;

        // Move to next
        Node *temp = current;
        current = current->next;
        free(temp);
        temp = NULL;

    }
    // Free the database and chain
    free((*ptr_chain)->database);
    free(*ptr_chain);
    *ptr_chain = NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Get one random MarkovNode from the given markov_chain's database.
 * @param markov_chain
 * @return the random MarkovNode
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain) {
    // incase the markov chain is null
    if (markov_chain == NULL || markov_chain->database == NULL ||
        markov_chain->database->size == 0) {
        return NULL;
    }

    Node *current = markov_chain->database->first;

    // generates a random index using the given function
    int random_index = get_random_number(markov_chain->database->size);
    // Traverse to that node

    for (int i = 0; i < random_index; i++) {
        current = current->next;
    }

    while (is_last_word(current->data->data) != 0) // if != 0, means it is a last word (ends with '.'), so keep generating
    {
        current = markov_chain->database->first;
        // generates a new random index using the given function
        int random_index = get_random_number(markov_chain->database->size);
        // Traverse to that node
        for (int i = 0; i < random_index; i++) {
            current = current->next;
        }
    }
    return current->data;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Choose randomly the next MarkovNode, depend on it's occurrence frequency.
 * @param cur_markov_node current MarkovNode
 * @return the next random MarkovNode
 */
MarkovNode* get_next_random_node(MarkovNode *cur_markov_node) {
    // incase frequency list is null
    if (cur_markov_node == NULL || cur_markov_node->frequency_list_size == 0) {
        return NULL;
    }

    // Calculate total frequency of all the next words in this node
    int total_frequency = 0;
    for (int j = 0; j < cur_markov_node->frequency_list_size; j++) {
        total_frequency += cur_markov_node->frequency_list[j].frequency;
    }

    // Generate random number 'i', representing the index of the next word in the frequency list
    int i = get_random_number(total_frequency);

    // Choose node based on cumulative frequency
    int cumulative = 0;
    for (int j = 0; j < cur_markov_node->frequency_list_size; j++) {
        cumulative += cur_markov_node->frequency_list[j].frequency;
        if (i < cumulative) { // checking if i is in the cumulative numbers range
            return cur_markov_node->frequency_list[j].markov_node;
        }
    }

    // incase anything else goes wrong (should never happen), return first value
    return cur_markov_node->frequency_list[0].markov_node;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence must have at least 2 words in it.
 * @param first_node markov_node to start with
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(MarkovNode *first_node, int max_length) {
    if (first_node == NULL || max_length < 2) {
        return;
    }

    // Print first word
    printf("%s", first_node->data);
    MarkovNode *current = first_node;
    int word_count = 1;

    // Generate subsequent words
    while (word_count < max_length && current->frequency_list_size > 0) {
        // Get next random node
        MarkovNode *next_node = get_next_random_node(current);

        // If no next node (word ends with '.'), break
        if (next_node == NULL) {
            break;
        }

        // Print next word
        printf(" %s", next_node->data);

        current = next_node;
        word_count++;
    }
}

