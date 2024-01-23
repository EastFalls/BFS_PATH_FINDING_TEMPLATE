#include "ofApp.h"
#include <vector>
#include <algorithm>
#include <random>
#include <queue>

enum Side {UP, DOWN, LEFT, RIGHT};

double cell_width = 50;
int num_x=0;
int num_y=0;
int margin = 5;

std::mt19937 random_engine(std::random_device{}());
std::vector<std::pair<int,int>> bfs();
std::vector<std::pair<int,int>> solution;
bool mouse_locked = false;
std::pair<int,int> start;
std::pair<int,int> destination;

struct Cell {
    double x1,y1,x2,y2;
    bool visited = false;
    
    Cell(double _x1, double _y1, double _x2, double _y2)
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
            walls.resize(4);
            previous_node = {-1, -1};
        }
    std::vector<int> walls;
    std::pair<int,int> previous_node;
    
    void draw() {
        ofSetColor(255);
        ofNoFill();
        if (walls[UP]) ofDrawLine(x1, y1, x2, y1);
        if (walls[DOWN]) ofDrawLine(x1, y2, x2, y2);
        if (walls[LEFT]) ofDrawLine(x1, y1, x1, y2);
        if (walls[RIGHT]) ofDrawLine(x2, y1, x2, y2);
    }
    
    void drawFill() {
        ofSetHexColor(0xff0000);
        ofFill();
        ofDrawRectangle(x1+1, y1+1, cell_width-2, cell_width-2);
    }
};

std::vector<std::vector<Cell>> maze;

void add_wall(int x, int y, Side s)
{
    if (x==0 && s==LEFT) return;
    if (x==num_x-1 && s==RIGHT) return;
    if (y==0 && s==UP) return;
    if (y==num_y-1 && s==DOWN) return;
    maze[y][x].walls[s]=1;
    if (s==UP) maze[y-1][x].walls[DOWN]=1;
    if (s==DOWN) maze[y+1][x].walls[UP]=1;
    if (s==RIGHT) maze[y][x+1].walls[LEFT]=1;
    if (s==LEFT) maze[y][x-1].walls[RIGHT]=1;
}

void init_maze()
{
    num_x = (ofGetWidth() - 2*margin)/cell_width;
    num_y = (ofGetHeight()-2*margin)/cell_width;
    
    maze.clear();
    maze.resize(num_y);
    for (int j=0;j<num_y;j++) {
        maze[j].reserve(num_x);
        for (int i=0;i<num_x;i++) {
            maze[j].emplace_back(margin+i*cell_width, margin+j*cell_width,
                    margin+(i+1)*cell_width, margin+(j+1)*cell_width);
        }
    }
    for (int i=0;i<num_x*num_y;i++) {
        int x = std::uniform_int_distribution<int>(0, num_x-1)(random_engine);
        int y = std::uniform_int_distribution<int>(0, num_y-1)(random_engine);
        auto &cell = maze[y][x];
        std::vector<Side> directions{UP, DOWN, LEFT, RIGHT};
        std::shuffle(directions.begin(), directions.end(), random_engine);
        add_wall(x,y,directions[0]);
    }
    
    start = {0,0};
    destination = {num_x-1, num_y-1};
    solution = bfs();
}

std::vector<std::pair<int,int>> bfs()
{
    std::vector<std::pair<int,int>> solution;
    std::vector<std::pair<int,int>> adj;
    for (auto &r : maze) for (auto &c : r) c.visited = false;
    
    std::queue<std::pair<int,int>> q;
    q.push(start);
    bool solution_exist = false;
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        auto &current_cell = maze[current.second][current.first];
        current_cell.visited=true;
        if (current == destination) { solution_exist=true; break; }
        
        // write you BFS Code here
    }
    auto current = destination;
    while (current!=start && solution_exist) {
        solution.push_back(current);
        current=maze[current.second][current.first].previous_node;
    }
    return solution;
}

//--------------------------------------------------------------
void ofApp::setup(){
    init_maze();
    std::cout << solution.size() << std::endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}
int show_solution = 1;
//--------------------------------------------------------------
void ofApp::draw(){
    for (auto &row : maze) for (auto &c : row) c.draw();
    maze[start.second][start.first].drawFill();
    maze[destination.second][destination.first].drawFill();
    if (show_solution)
        for (auto &c : solution) maze[c.second][c.first].drawFill();
    
    if (mouse_locked) {
        ofSetHexColor(0x00ffff);
        auto &cell = maze[start.second][start.first];
        ofDrawLine(cell.x1+cell_width/2, cell.y1+cell_width/2,
                   ofGetMouseX(), ofGetMouseY());
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==OF_KEY_UP) {
        cell_width++;
        init_maze();
    }
    if (key==OF_KEY_DOWN) {
        cell_width--;
        if (cell_width<5) cell_width=5;
        init_maze();
    }
    if (key=='r') init_maze();
    if (key=='s') show_solution=1-show_solution;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    int i = (x-margin)/cell_width;
    int j = (y-margin)/cell_width;
    std::cout << i << ", " << j << std::endl;
    if (i<0||i>num_x-1||j<0||j>num_y-1) return;
    if (mouse_locked) {
        mouse_locked=false;
        destination={i,j};
        solution = bfs();
    } else {
        mouse_locked=true;
        start={i,j};
    }
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    init_maze();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
