# ELIZA-tree

C implementation of the ELIZA psychotherapist chatbot using a lernable decision tree trained on synthetic data to classify user input and select responses.

ELIZA-tree extracts 21 linguistic features from user input (like presence of emotion words, family references and question marks) and uses a decision tree to choose among 16 avaiable response categories
It features shit like feature extraction, pronoun reflection, memory etc (like ELIZA but enchanced)

## Building

Just run `make`

You need GCC and GNU make

## Usage

First train the tree, you can do it by:

```bash
make train
```

this genereates synthetic data, trains a tree and saves to `eliza.tree`

The `eliza_tree` binary supports these flags:
- `-o, --output <file>`: output file (default: `eliza.tree`)
- `-n, --samples <num>`: number of training samples (default: 10000)
- `-d, --max_depth <depth>`: max tree depth (default: 7)
- `-m, --min_samples <num>`: min samples per leaf (default: 10)

After training you can chat:

```bash
make chat
```

That loads `eliza.tree` and starts an interactive session, type `quit` or `exit` to kill the chat

