#include <bits/stdc++.h>
using namespace std;

enum{
    maxRows = 30,
    maxCols = 50,
    Wall = 0,
    Open = 1,
    maxRecords = 40
};

struct MazeRecord{
    int id;
    string name;
    int rows;
    int cols;
    int edges;
};

struct Maze{
    int rows;
    int cols;
    int grid[maxRows][maxCols];
    bool visitedGrid[maxRows][maxCols];
    bool path[maxRows][maxCols];
    int parentNode[maxRows * maxCols + 1];
    int startRow;
    int startCol;
    int goalRow;
    int goalCol;

    int toNode(int r, int c){
        return r*cols + c + 1;
    }

    int toRow(int node){
        return (node-1) / cols;
    }

    int toCol(int node){
        return (node-1) % cols;
    }

    void reset(){
        int totalNodes = rows*cols;
        for(int r=0; r<rows; r++){
            for(int c=0; c<cols; c++){
                visitedGrid[r][c] = false;
                path[r][c] = false;
            }
        }
        for(int i=0; i<=totalNodes; i++){
            parentNode[i] = -1;
        }
    }

    void createPaths(int r, int c){
        visitedGrid[r][c] = true;
        grid[r][c] = Open;

        int moveRow[4] = {-2, 2, 0, 0};
        int moveCol[4] = {0, 0, -2, 2};
        int dirs[4] = {0, 1, 2, 3};

        for(int i=3; i>0; i--){
            int j = rand() % (i+1);
            int temp = dirs[i];
            dirs[i] = dirs[j];
            dirs[j] = temp;
        }

        for(int i=0; i<4; i++){
            int d = dirs[i];
            int nr = r+moveRow[d];
            int nc = c+moveCol[d];

            if(nr>0 && nr<rows-1 && nc>0 && nc<cols-1 && !visitedGrid[nr][nc]){
                grid[r + moveRow[d] / 2][c + moveCol[d] / 2] = Open;
                createPaths(nr, nc);
            }
        }
    }

    void start(int r, int c){
        rows = (r%2 == 0) ? r+1 : r;
        cols = (c%2 == 0) ? c+1 : c;
        if(rows > maxRows) rows = maxRows;
        if(cols > maxCols) cols = maxCols;
        if(rows < 7) rows = 7;
        if(cols < 9) cols = 9;

        startRow = 1;
        startCol = 1;
        goalRow = rows - 2;
        goalCol = cols - 2;
        makeMaze();
    }

    void makeMaze(){
        for(int r=0; r<rows; r++){
            for(int c=0; c<cols; c++){
                grid[r][c] = Wall;
                visitedGrid[r][c] = false;
                path[r][c] = false;
            }
        }

        createPaths(startRow, startCol);

        int loops = (rows*cols) / 35;
        for(int i=0; i<loops; i++){
            int randRow = 1 + rand() % (rows-2);
            int randCol = 1 + rand() % (cols-2);
            grid[randRow][randCol] = Open;
        }

        grid[startRow][startCol] = Open;
        grid[goalRow][goalCol] = Open;
        reset();
    }

    char WallSign(int r, int c){
        bool upOpen = (r > 0 && grid[r-1][c] == Open);
        bool downOpen = (r < rows-1 && grid[r+1][c] == Open);
        bool leftOpen = (c > 0 && grid[r][c-1] == Open);
        bool rightOpen = (c < cols-1 && grid[r][c+1] == Open);

        if(upOpen || downOpen) return '-';
        if(leftOpen || rightOpen) return '|';
        if(r == 0 || r == rows-1) return '-';
        return '|';
    }

    void show(){
        cout << "\nS = Start | E = Goal | |/- = Wall | : = Visited | @ = Path\n\n";
        for(int r=0; r<rows; r++){
            cout<<"   ";
            for(int c=0; c<cols; c++){
                if(r==startRow && c==startCol) cout<<"S ";
                else if(r == goalRow && c == goalCol) cout<<"E ";
                else if(path[r][c]) cout << "@ ";
                else if(grid[r][c] == Wall) cout<<WallSign(r, c) <<" ";
                else if(visitedGrid[r][c])cout<<": ";
                else cout<<"  ";
            }
            cout<<"\n";
        }
    }

    void buildGraph(int totalNode, vector<int> adj[]){
        int moveRow[4] = {-1, 1, 0, 0};
        int moveCol[4] = {0, 0, -1, 1};

        for(int i=1; i<=totalNode; i++) adj[i].clear();

        for(int r=0; r<rows; r++){
            for(int c=0; c<cols; c++){
                if(grid[r][c] == Open){
                    int u = toNode(r, c);
                    for(int k=0; k<4; k++){
                        int nr = r+moveRow[k];
                        int nc = c+moveCol[k];
                        if(nr>=0 && nr<rows && nc>=0 && nc<cols && grid[nr][nc] == Open){
                            adj[u].push_back(toNode(nr, nc));
                        }
                    }
                }
            }
        }
    }

    int markPath(int src, int dest){
        int curr = dest;
        int edges = 0;

        while(curr != src){
            int prev = parentNode[curr];
            if(prev == -1) return -1;
            int r = toRow(curr);
            int c = toCol(curr);
            if(!(r==goalRow && c==goalCol)){
                path[r][c] = true;
            }
            edges++;
            curr = prev;
        }
        return edges;
    }

    int bfs(int totalNode, int src, int dest, vector<int> adj[], int& visitedOut){
        reset();
        vector<bool> vis(totalNode+1, false);
        queue<int> q;

        q.push(src);
        vis[src] = true;
        visitedOut = 0;

        while(!q.empty()){
            int n = q.front();
            q.pop();
            visitedOut++;
            visitedGrid[toRow(n)][toCol(n)] = true;

            if(n == dest){
                return markPath(src, dest);
            }

            for(int it : adj[n]){
                if(!vis[it]){
                    q.push(it);
                    vis[it] = true;
                    parentNode[it] = n;
                }
            }
        }
        return -1;
    }

    int dfs(int totalNode, int src, int dest, vector<int> adj[], int& visitedOut){
        reset();
        vector<bool> vis(totalNode + 1, false);
        stack<int> st;

        st.push(src);
        visitedOut = 0;

        while(!st.empty()){
            int n = st.top();
            st.pop();

            if(vis[n]) continue;
            vis[n] = true;
            visitedOut++;
            visitedGrid[toRow(n)][toCol(n)] = true;

            if(n == dest){
                return markPath(src, dest);
            }

            int sz = adj[n].size();
            for(int i = sz-1; i>=0; i--){
                int it = adj[n][i];
                if(!vis[it]){
                    if(parentNode[it] == -1) parentNode[it] = n;
                    st.push(it);
                }
            }
        }
        return -1;
    }
};

void loadSample(MazeRecord rec[], int& n){
    string names[15] = {
        "Maze1", "Maze2", "Maze3", "Maze4", "Maze5",
        "Maze6", "Maze7", "Maze8", "Maze9", "Maze10",
        "Maze11", "Maze12", "Maze13", "Maze14", "Maze15"
    };
    int rArr[15] = {9, 11, 13, 15, 17, 11, 13, 15, 19, 9, 21, 13, 17, 15, 23};
    int cArr[15] = {11, 15, 19, 21, 25, 17, 23, 27, 29, 13, 31, 25, 29, 19, 35};

    n = 15;
    for(int i = 0; i < 15; i++){
        rec[i].id = i + 1;
        rec[i].name = names[i];
        rec[i].rows = rArr[i];
        rec[i].cols = cArr[i];
        rec[i].edges = -1;
    }
}

void printRecords(MazeRecord rec[], int n){
    for(int i = 0; i < n; i++){
        cout <<rec[i].id << " | " << rec[i].name << " | " << rec[i].rows << "x" << rec[i].cols << " | Edges: " << rec[i].edges << "\n";
    }
}

int main(){
    srand(time(0));

    Maze m;
    m.start(10, 15);

    int totalNode = m.rows*m.cols;
    vector<int> adj[maxRows*maxCols+1];
    m.buildGraph(totalNode, adj);

    int src = m.toNode(m.startRow, m.startCol);
    int dest = m.toNode(m.goalRow, m.goalCol);

    MazeRecord rec[maxRecords];
    int recCount = 0;
    loadSample(rec, recCount);
    int nextId = 16;

    int ch = 0;

    while(true){
        cout<<"\n1. New Maze\n";
        cout<<"2. Show Maze\n";
        cout<<"3. DFS (Stack)\n";
        cout<<"4. BFS (Queue)\n";
        cout<<"5. Compare BFS vs DFS\n";
        cout<<"6. Reset & Load Sample\n";
        cout<<"7. Records (View/Add/Edit/Delete)\n";
        cout<<"8. Search / Filter / Sort Records\n";
        cout<<"9. Exit\n";
        cout<<"Choice: ";

        if(!(cin>>ch)){
            cout<<"Invalid choice.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if(ch == 1){
            m.makeMaze();

            totalNode = m.rows * m.cols;
            m.buildGraph(totalNode, adj);

            src = m.toNode(m.startRow, m.startCol);
            dest = m.toNode(m.goalRow, m.goalCol);
            m.show();
        }

        else if(ch == 2){
            m.show();
        }

        else if(ch == 3){
            int vis = 0;
            int edges = m.dfs(totalNode, src, dest, adj, vis);
            m.show();

            cout<< "\nDFS -> Visited: " << vis << " | Edges: " << edges << "\n";
        }

        else if(ch == 4){
            int vis = 0;
            int edges = m.bfs(totalNode, src, dest, adj, vis);
            m.show();

            cout<< "\nBFS -> Visited: " << vis << " | Edges: " << edges << "\n";
        }

        else if(ch == 5){
            int bVis = 0, dVis = 0;
            int bEdges = m.bfs(totalNode, src, dest, adj, bVis);
            int dEdges = m.dfs(totalNode, src, dest, adj, dVis);
            m.show();

            cout<< "\nBFS -> Visited: " << bVis << " | Edges: " << bEdges << "\n";
            cout<< "DFS -> Visited: " << dVis << " | Edges: " << dEdges << "\n";
        }

        else if(ch == 6){
            loadSample(rec, recCount);

            nextId = 16;
            m.start(10, 15);
            totalNode = m.rows * m.cols;
            m.buildGraph(totalNode, adj);

            src = m.toNode(m.startRow, m.startCol);
            dest = m.toNode(m.goalRow, m.goalCol);
            cout<< "Reset done.\n";
        }

        else if(ch == 7){
            cout<< "1.View      2.Add      3.Edit      4.Delete: ";
            int op;
            cin>>op;
            if(op == 1){
                printRecords(rec, recCount);
            }
            else if(op == 2 && recCount < maxRecords){
                MazeRecord item;
                item.id = nextId++;
                cout<<"Name: "; 
                cin>>item.name;
                cout<<"Rows: "; 
                cin>>item.rows;
                cout<<"Cols: "; 
                cin>>item.cols;
                item.edges = -1;
                rec[recCount++] = item;
            }
            else if(op == 3){
                int id;
                cout<<"ID to edit: "; 
                cin>>id;
                for(int i=0; i<recCount; i++){
                    if(rec[i].id == id){
                        cout<<"New name: "; 
                        cin>>rec[i].name;
                        cout<<"New rows: "; 
                        cin>>rec[i].rows;
                        cout<<"New cols: "; 
                        cin>>rec[i].cols;
                    }
                }
            }
            else if(op == 4){
                int id;
                cout<<"ID to delete: ";
                cin>>id;
                for(int i=0; i<recCount; i++){
                    if(rec[i].id == id){
                        for(int j=i; j<recCount-1; j++){
                            rec[j] = rec[j + 1];
                        } 
                        recCount--;
                        break;
                    }
                }
            }
            else{
                cout<<"Invalid choice.\n";
            }
        }

        else if(ch == 8){
            cout<<"1.Search Name      2.Show Rows >=      3.Sort by Rows: ";
            int op;
            cin >> op;
            if(op == 1){
                string key;
                cout<<"Name: "; 
                cin>>key;
                for(int i=0; i<recCount; i++){
                    if(rec[i].name == key){
                        cout<<rec[i].id << " " << rec[i].name << " (" << rec[i].rows << "x" << rec[i].cols << ")\n";
                    }
                }
            }
            else if(op == 2){
                int minR;
                cout<<"Min rows: "; 
                cin>>minR;
                for(int i=0; i<recCount; i++){
                    if(rec[i].rows >= minR){
                        cout<< rec[i].id << " " << rec[i].name << " (" << rec[i].rows << "x" << rec[i].cols << ")\n";
                    }
                }
            }
            else if(op == 3){
                for(int i=0; i < recCount-1; i++){
                    for(int j=0; j < recCount-i-1; j++){
                        if(rec[j].rows > rec[j+1].rows){
                            MazeRecord t = rec[j];
                            rec[j] = rec[j+1];
                            rec[j+1] = t;
                        }
                    }
                }
                printRecords(rec, recCount);
            }
            else{
                cout<<"Invalid choice.\n";
            }
        }
        else if(ch == 9){
            break;
        }
        else{
            cout<<"Invalid choice.\n";
        }
    }

    return 0;
}
