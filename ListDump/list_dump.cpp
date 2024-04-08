#include <stdio.h>
#include <stdlib.h>

#include "../FastList/list.h"
#include "../debug/debug.h"
#include "list_dump.h"

static size_t call_count = 0;

static FILE *log_file = nullptr;

static const char * const log_file_name = "list.dmp.html";

//================================================================================================

void BeginListGraphDump()
{
    system("rm -f *.png");

    log_file = fopen(log_file_name, "w");

    if (log_file == nullptr)
    {
        perror("BeginListGraphDump() failed to open file");

        return;
    }

    fprintf(log_file, "<pre>\n");
}

//================================================================================================

void EndListGraphDump()
{
    if (log_file == nullptr)
    {
        perror("EndLostGrapDump()");

        return;
    }

    fclose(log_file);
}

//================================================================================================

ListErrs_t GraphDumpList(List *list,
                         const char *file,
                         const char *func,
                         const int line)
{
    if (log_file == nullptr)
    {
        ColorPrintf(kRed, ">> GraphDumpList(): log file was not opened.\n"
                          "   Use function BeginListGraphDump()\n");

        return kLogFileWasNotOpened;
    }

    FILE *dot_file = fopen("dump_src/list.dmp.dot", "w");

    if (dot_file == nullptr)
    {
        perror("GraphDumpList() failed to open dump file");

        return kFailedToOpenFile;
    }

    static char cmd_command[200] = {0};

    #define LOG_PRINT(...) fprintf(dot_file, __VA_ARGS__)

    LOG_PRINT("digraph List\n{\n"
              "\trankdir = TB;\n"
              "\tgraph [bgcolor = \"black\", splines = \"ortho\"]\n"
              "\tnode[color =\"black\", fontsize=14];\n"
              "\tedge[color = \"red\", fontcolor = \"blue\",fontsize = 12];\n\n\n");

    LOG_PRINT("{rank = max;\n"
              "TAIL [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"TAIL : %d\" ]\n"
              "ELEM_COUNT [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"ELEM COUNT : %d\" ]\n"
              "FREE [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"FREE : %d\" ]\n"
              "HEAD [style = filled, fillcolor = \"lightgreen\", shape = \"rect\", label =  \"HEAD : %d\" ]}\n"
              "ELEM_COUNT->TAIL->FREE->HEAD [weight = 100, color = \"invis\"]",
              list->tail,
              list->elem_count,
              list->free,
              list->head);

    LOG_PRINT("{rank = max; node0 [style = filled, fillcolor = \"lightblue\", shape=Mrecord, label = "
              "\"<name> NODE_0 | {data : POISON | <next> next : %d | <prev> prev : %d}\"]}",
              list->next[0],
              list->prev[0]);

    LOG_PRINT("{rank = same;\n");


    for (size_t i = 0; i < list->capacity; i++)
    {
        if (list->data[i] != kPoisonVal)
        {
            LOG_PRINT("node%d [style = filled, fillcolor = \"%s\", shape=Mrecord, label = "
                    "\"<name> NODE_%d | {data : %s | <next> next : %d | <prev> prev : %d}\"]\n",
                    i,
                    (list->prev[i] == -1) ? "pink" : "lightblue",
                    i,
                    list->data[i],
                    list->next[i],
                    list->prev[i]);
        }
        else
        {
            LOG_PRINT("node%d [style = filled, fillcolor = \"%s\", shape=Mrecord, label = "
                    "\"<name> NODE_%d | {data : POISON | <next> next : %d | <prev> prev : %d}\"]\n",
                    i,
                    (list->prev[i] == -1) ? "pink" : "lightblue",
                    i,
                    list->next[i],
                    list->prev[i]);
        }
    }

    LOG_PRINT( "}\n");

    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        LOG_PRINT("node%d->node%d [weight = 100, color = \"invis\"]\n",
                  i,
                  i + 1);
    }

    for (size_t i = 0; i < list->capacity; i++)
    {
        LOG_PRINT("node%d->node%d [weight = 0, color = \"%s\"]\n",
                  i,
                  list->next[i],
                  (list->prev[i] == -1) ? "orange" : "red");
    }

    #ifdef PRINT_PREV

    for (size_t i = 0; i < list->capacity; i++)
    {
        if (list->prev[i] != -1)
        {
            LOG_PRINT("node%d->node%d [weight = 0, color = \"yellow\"]",
                      i,
                      list->prev[i]);
        }
    }

    #endif

    LOG_PRINT("TAIL->node%d [weight = 0, color = \"green\"]\n"
              "FREE->node%d [weight = 0, color = \"green\"]\n"
              "HEAD->node%d [weight = 0, color = \"green\"]\n",
              list->tail,
              list->free,
              list->head);

    LOG_PRINT("\n\n}");

    fclose(dot_file);

    sprintf(cmd_command, "dot -Tpng dump_src/list.dmp.dot -o dump_src/graphdump%d.png", call_count);

    system(cmd_command);

    fprintf(log_file, "Called from file: %s\n"
                      "Called from function: %s\n"
                      "Line: %d\n"
                      "<img height=\"150px\" src=\"dump_src/graphdump%d.png\">\n"
                      "-----------------------------------------------------------------\n",
                      file,
                      func,
                      line,
                      call_count);


    ++call_count;
}

#undef LOG_PRINT
