/* A-Star
 * ======
 * 
 * Basic implementation of the A-Star algorithm written in dirty C++.
 * 
 * A-Star is a method to find the shortest path between two points.
 * 
 * The calculation is slowed down for you to follow the creation of the path.
 * 
 * Possible output:
 * 
 *     ######################
 *     #                    #
 *     #                  F #
 *     #OOOOOOOOOOOOOOOOOOvO#
 *     #v<<<<<<<<<<<<<<<<<<O#
 *     #v#########OOOOOOOOO #
 *     #>vvvvvvvv#          #
 *     #Ovvvvvvvv#          #
 *     #Ovvvvvvvv#          #
 *     #OS<<<<<<<#          #
 *     # OOOO^^^^#          #
 *     ######################
 * 
 *     S = start
 *     F = finish
 *     # = wall
 *     O = open for checking
 *     <>^v = when going from finish to start, follow the arrows
 *     (you can always swap F and S to find a way from the start to the finish)
 * 
 * Don't expect too much from a one-day project! ;)
 *
 * Author: Gjum <gjum42@gmail.com>
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

const int width = 20;
const int height = 10;
char map[][width+1] = {
    "                    ",
    "                  F ",
    "                    ",
    "                    ",
    " #########          ",
    "         #          ",
    "         #          ",
    "         #          ",
    " S       #          ",
    "         #          "};

class Node {
    public:
        Node(int _x, int _y, char _dir, int _prevDist);
        void tryAddNeighbors();
        int x, y, prevDist, estimDist;
        char dir;
};

Node *finish = new Node(0, 0, 'F', 0);
Node *start = new Node(0, 0, 'S', -1);
std::vector<Node*> open, visited, path;

void debug(char* msg) {
    //printf("%s\n", msg);
}

bool compareNodes(const Node *a, const Node *b) {
    // best node is smallest total length
    if (a->prevDist + a->estimDist == b->prevDist + b->estimDist)
        return a->prevDist < b->prevDist;
    return a->prevDist + a->estimDist > b->prevDist + b->estimDist;
}

bool operator==(const Node& a, const Node& b) {
    return a.x == b.x && a.y == b.y;
}

void init() {
    debug("Initializing");
    // cleanup
    open.clear();
    visited.clear();
    // find start and finish
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] == 'S') {
                start->x = x;
                start->y = y;
            }
            else if (map[y][x] == 'F') {
                finish->x = x;
                finish->y = y;
            }
        }
    }
    open.push_back(start);
}

void show() { // print map
    debug("show");
    char outMap[height][width+2];
    // copy empty map
    for (int h = 0; h < height; h++) {
        sprintf(outMap[h], "%s", map[h]);
    }
    // show open nodes
    for (int i = 0; i < open.size(); i++) {
        Node *n = open[i];
        outMap[n->y][n->x] = 'O';
        //outMap[n->y][n->x] = n->dir;
    }
    // show visited nodes
    for (int i = 0; i < visited.size(); i++) {
        Node *n = visited[i];
        outMap[n->y][n->x] = n->dir;
    }
    // show start and finish
    outMap[start->y][start->x] = 'S';
    outMap[finish->y][finish->x] = 'F';
    // print map
    printf("#");
    for (int x = 0; x < width; x++) printf("#");
    printf("#\n");
    for (int h = 0; h < height; h++) {
        printf("#%s#\n", outMap[h]);
    }
    printf("#");
    for (int x = 0; x < width; x++) printf("#");
    printf("#\n\n");
	usleep(100000);
}

bool invalidNode(Node *node) {
    debug("Checking if invalid node");
    // out of bounds?
    if (node->x < 0) return true;
    if (node->y < 0) return true;
    if (node->x >= width)  return true;
    if (node->y >= height) return true;
    // in a wall?
    if (map[node->y][node->x] == '#') return true;
    // already visited?
    for (int i = 0; i < visited.size(); i++) {
        if (*node == *visited[i]) return true;
    }
    return false;
}

void tryAddOpenNode(Node *node) {
    if (!invalidNode(node)) {
        bool insert = true;
        // already open?
        for (int i = 0; i < open.size(); i++) {
            if (*node == *open[i]) {
                if (node->prevDist < open[i]->prevDist) {
                    open[i]->prevDist = node->prevDist;
                    open[i]->dir = node->dir;
                }
                insert = false;
            }
        }
        if (insert) open.push_back(node);
        //show();
    }
}

Node::Node(int _x, int _y, char _dir, int _prevDist) {
    x = _x;
    y = _y;
    dir = _dir;
    prevDist = _prevDist+1;
    if (_dir == 'F') {
        estimDist = 0;
        return;
    }
    int dx = x-finish->x;
    int dy = y-finish->y;
    estimDist = (dx > 0 ? dx : -dx) + (dy > 0 ? dy : -dy);
}

void Node::tryAddNeighbors() {
    tryAddOpenNode(new Node(x, y+1, '^', prevDist));
    tryAddOpenNode(new Node(x, y-1, 'v', prevDist));
    tryAddOpenNode(new Node(x+1, y, '<', prevDist));
    tryAddOpenNode(new Node(x-1, y, '>', prevDist));
    std::sort(open.begin(), open.end(), compareNodes);
}

bool step() {
    debug("step");
    // pick best unvisited open node
    Node *node;
    do {
        node = open.back();
        open.pop_back();
    } while (invalidNode(node));
    // are we done?
    if (*node == *finish) {
        return true;
    }
    visited.push_back(node);
    node->tryAddNeighbors();
    return false;
}

int main() {
    init();
    while(!step()) {
        show();
    }
    show();
    return 0;
}

