#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

typedef struct Node {
    char *word;
    struct Node *left;
    struct Node *right;
}Node;

Node* initTree(char *word) {
    Node* tree = (Node *)malloc(sizeof(Node));
    if (tree == NULL) {
        printf("Memory allocation error for tree with word %s", word);
        return NULL;
    }
    tree->word = word;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

Node* addNode(char *word, Node *tree) {
    if (tree == NULL) {
        tree = initTree(word);
    } else {
        if (strcmp(tree->word, word) < 0) {
            tree->right = addNode(word, tree->right);
        } else {
            tree->left = addNode(word, tree->left);
        }
    }
    return tree;
};

void writeTreeInFile(FILE* file, Node* tree) {
    if (tree != NULL) {
        writeTreeInFile(file, tree->right);
        fprintf(file, "%s\n", tree->word);
        writeTreeInFile(file, tree->left);
    }
}

void clearTree(Node* tree) {
    if (tree != NULL) {
        clearTree(tree->left);
        clearTree(tree->right);
        free(tree);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        return 0;
    }

    const char *input_filepath = argv[1];
    const char *output_filepath = argv[2];

    unsigned  int N = 10, delta = 10, i = 0;
    char *buf = (char*)malloc(sizeof(char) * N);
    if (buf == NULL) {
        puts("Memory allocation error for word buffer");
        return 0;
    }

    FILE *file;
    file = fopen(input_filepath, "r");
    if (!file) {
        puts("Error open file");
        return 0;
    }

    Node* newNode = NULL;
    char tempChar;

    while (((tempChar = fgetc(file)) != EOF)) {
        if (isspace(tempChar)) {
            buf[i] = '\0';
            newNode = addNode(buf, newNode);
            buf = (char *)malloc(sizeof(char) * N);
            if (buf == NULL) {
                printf("Memory allocation error for word buffer");
                return NULL;
            }
            i = 0;
        } else {
            buf[i] = tempChar;
            if (++ i >= N) {
                N += delta;
                buf = (char *)realloc(buf, sizeof(char) * N);
                if (buf == NULL) {
                    printf("Memory allocation error for word buffer");
                    return NULL;
                }
            }
        }
    }
    newNode = addNode(buf, newNode);
    fclose(file);

    file = fopen(output_filepath, "w");
    if (!file) {
        puts("Error open file");
        return 0;
    }
    writeTreeInFile(file, newNode);
    fclose(file);
    clearTree(newNode);
    free(buf);
    return 0;
}
