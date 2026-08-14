#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 400
#define MAX_CODE 16
#define MAX_SUBJECT 128
#define MAX_CHILDREN 20
//Constraints for nodes

typedef struct Node {
    char code[MAX_CODE];
    char subject[MAX_SUBJECT];

    struct Node *parent;
    struct Node *children[MAX_CHILDREN];
    int childCount;

} Node;


Node *findNode(Node nodes[], int count, const char *code)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(nodes[i].code, code) == 0)
            return &nodes[i];
    }

    return NULL;
}
//Searches for matching node and returns pointer to match


Node *findSubject(Node nodes[], int count, const char *subject)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(nodes[i].subject, subject) == 0)
            return &nodes[i];
    }

    return NULL;
}
//Searches for matching node in the array compared to user input


void writePaths(Node *node, FILE *out, char path[][128], int depth, int *first)
{
    strcpy(path[depth], node->subject);


    if (node->childCount == 0)
    {
        if (!(*first))
            fprintf(out, ",\n");

        *first = 0;


        fprintf(out, "        [");

        for (int i = 0; i <= depth; i++)
        {
            fprintf(out, "\"%s\"", path[i]);

            if (i != depth)
                fprintf(out, ", ");
        }

        fprintf(out, "]");

        return;
    }


    for (int i = 0; i < node->childCount; i++)
    {
        writePaths(node->children[i], out, path, depth + 1, first);
    }
}
//Creates all possible pathes from matching node


int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: %s <subject>\n", argv[0]);
        return 1;
    }


    char *targetSubject = argv[1];


    FILE *fp = fopen("/home/pathwayto/subject_tree.txt", "r");
  //***NEEDS CHANGED***


    if (fp == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    Node nodes[MAX_NODES];

    int nodeCount = 0;

    char line[1024];
    char token[256];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int t = 0;

        for (int i = 0;; i++)
        {

            char c = line[i];


            if (c == ',' || c == ';' || c == '\n' || c == '\0')
            {

                if (t > 0)
                {

                    token[t] = '\0';


                    char *dot = strchr(token, '.');


                    if (dot != NULL)
                    {
                        *dot = '\0';

                        strcpy(nodes[nodeCount].code, token);
                        strcpy(nodes[nodeCount].subject,
                               dot + 1);


                        nodes[nodeCount].parent = NULL;
                        nodes[nodeCount].childCount = 0;

                        nodeCount++;
                    }
                    t = 0;
                }
                if (c == '\0')
                    break;
            }

            else if (c != ' ' && c != '\r' && c != '\n')
            {
                token[t++] = c;
            }
        }
    }
    fclose(fp);


    for (int i = 0; i < nodeCount; i++)
    {

        if (strlen(nodes[i].code) == 1)
            continue;

        char parentCode[MAX_CODE];

        strcpy(parentCode, nodes[i].code);

        parentCode[strlen(parentCode)-1] = '\0';

        Node *parent = findNode(nodes, nodeCount, parentCode);

        if (parent != NULL)
        {

            nodes[i].parent = parent;

            parent->children[parent->childCount] = &nodes[i];
            parent->childCount++;

        }

    }

    Node *root = findSubject(nodes, nodeCount, targetSubject);

    if (root == NULL)
    {
        printf("Subject '%s' not found.\n",
               targetSubject);

        return 1;
    }

    char filename[128];

    snprintf(filename, sizeof(filename), "%s_paths.json", targetSubject);

    FILE *out = fopen(filename, "w");
    printf("Saving to: %s\n", filename);

    if (out == NULL)
    {
        printf("Could not create JSON file.\n");
        return 1;
    }

    fprintf(out, "{\n");

    fprintf(out, "    \"subject\": \"%s\",\n", targetSubject);

    fprintf(out, "    \"paths\": [\n");


    char path[100][128];

    int first = 1;



    writePaths(root, out, path, 0, &first);

    fprintf(out, "\n    ]\n");
    fprintf(out, "}\n");

    fclose(out);

    printf("Created %s\n", filename);

    return 0;
}
//Takes the user input subject then reads the the tree for each item in it, it then creates the necessary array
//then takes each item from the tree file to be used in a path. Also creates and writes to the json tree file made.
