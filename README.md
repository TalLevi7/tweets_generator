# NLP Markov Chain Tweet Generator

## Introduction

This project is an implementation of a Markov Chain-based tweet generator using Natural Language Processing (NLP). It takes a corpus of existing tweets and generates new tweets probabilistically based on word frequency patterns.

## Project Overview

The program consists of three main stages:

1. **Input Stage**: The program reads a text file (corpus) containing a dataset of tweets.
2. **Learning Stage**: The program processes the corpus to build a Markov Chain data structure, computing word frequencies and their probabilities.
3. **Output Stage**: The program generates new tweets based on the Markov Chain model.

## Markov Chains

A Markov Chain is a model describing a sequence of events where the probability of each event depends only on the state attained in the previous event. In this project:

- Each word in a tweet represents a state in the Markov Chain.
- Transition probabilities are determined based on the frequency of word sequences in the corpus.
- The process continues until a termination condition is met (e.g., reaching an ending punctuation).

## Implementation Details

### Data Structures

The project uses the following key data structures:

- **MarkovChain**: Holds the entire dataset structured as a linked list of unique words.
- **MarkovNode**: Represents each word in the dataset and keeps track of possible following words.
- **MarkovNodeFrequency**: Stores the frequency of occurrence of words following a specific word.

### Random Number Generation

The program uses pseudo-random number generation to select words based on their probabilities. Key functions:

- `srand(seed)`: Initializes the random seed.
- `rand()`: Generates pseudo-random numbers.

### File Handling

The program reads input from a corpus file (e.g., `tweets_justdoit.txt`) and parses it to extract sentences and words for processing.

## Files in the Project

- `markov_chain.h`: Contains struct definitions.
- `markov_chain.c`: Implements the Markov Chain logic.
- `linked_list.h`, `linked_list.c`: Implements a linked list.
- `generator_tweets.c`: Contains the main function and additional tweet generation logic.
- `tweets_justdoit.txt`: Example dataset for testing.

## Compilation and Execution

To compile the project, use:

```sh
gcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator
```

To run the program:

`./tweets_generator <SEED> <NUM_TWEETS> <FILE_PATH> <MAX_WORDS>`

Where:
- `<SEED>` is the random seed value.
- `<NUM_TWEETS>` is the number of tweets to generate.
- `<MAX_WORDS>` - optional, if not given, will read the whole file

for example:
```sh
./tweets_generator 42 5 justdoit_tweets.txt
```
