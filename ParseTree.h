#ifndef PARSETREE_H_23235
#define PARSETREE_H_23235

#include <stdarg.h>
#include <stdlib.h>

typedef struct ParseTreeStruct ParseTree;
ParseTree *createParseTree(ParseTreeContentType content, void (*contentDestructor)(ParseTreeContentType content), int childrenCount, ...);
void destroyParseTree(ParseTree *root);

struct ParseTreeStruct {
    ParseTreeContentType content;
    ParseTree **children;
    int childrenCount;

    void (*contentDestructor)(ParseTreeContentType content);
};

ParseTree *createParseTree(ParseTreeContentType content, void (*contentDestructor)(ParseTreeContentType content), int childrenCount, ...) {
    va_list childrenListArgument;
    ParseTree *parseTree = (ParseTree *)malloc(sizeof(ParseTree));

    parseTree->content = content;
    parseTree->contentDestructor = contentDestructor;
    parseTree->childrenCount = childrenCount;
    parseTree->children = (ParseTree **)malloc(childrenCount * sizeof(ParseTree *));

    va_start(childrenListArgument, childrenCount);
    
    {
        int childIndex;
        for (childIndex = 0; childIndex < childrenCount; ++childIndex) {
            parseTree->children[childIndex] = va_arg(childrenListArgument, ParseTree *);
        }
    }

    va_end(childrenListArgument);

    return parseTree;
}

void destroyParseTree(ParseTree *root) {
    int childIndex;
    for (childIndex = 0; childIndex < root->childrenCount; ++childIndex) destroyParseTree(root->children[childIndex]);
    free(root->children);
    if (root->contentDestructor) (*root->contentDestructor)(root->content);
    free(root);
}

#endif
