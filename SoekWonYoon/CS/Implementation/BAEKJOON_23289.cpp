//Memory Usage : 2696KB, Time Rquired : 40ms
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

int R, C, K;

const int Dir[4][2] = {{0,1},{0,-1},{-1,0},{1,0}};

struct Point {
    int row;
    int col;
    
    bool chkRange(void) {
        return row >= 0 && row < R && col >= 0 && col < C;
    }
};

struct circulator {
    Point point;
    int dir_ind;
    
    int nextRow(void) {return point.row + Dir[dir_ind][0];}
    
    int nextCol(void) {return point.col + Dir[dir_ind][1];}
    
    Point nextPoint(void) {return {point.row + Dir[dir_ind][0], point.col + Dir[dir_ind][1]};}
};

vector<vector<int>> blow(circulator &cir, const vector<vector<vector<vector<bool>>>> &wall_grid)
{
    vector<vector<int>> result_grid(R, vector<int>(C, 0));
    queue<pair<int, Point>> next_q;
    
    Point cur = cir.nextPoint();
    if(cur.chkRange() && !wall_grid[cir.point.row][cir.point.col][cur.row][cur.col])
        next_q.push(make_pair(5,cur));
    
    while(!next_q.empty())
    {
        cur = next_q.front().second;
        int cnt = next_q.front().first;
        next_q.pop();
        
        if(cur.chkRange() && result_grid[cur.row][cur.col] == 0)
        {
            Point next = {cur.row + Dir[cir.dir_ind][0], cur.col + Dir[cir.dir_ind][1]};
            int vertical_dir = (cir.dir_ind >= 2 ? 0 : 2);
            
            result_grid[cur.row][cur.col] = cnt;
            
            if(cnt > 1)
            {
                if(next.chkRange() && !wall_grid[cur.row][cur.col][next.row][next.col])
                    next_q.push(make_pair(cnt - 1, next));
                
                for(int i = 0; i < 2; ++i)
                {
                    Point ver_next = {cur.row + Dir[vertical_dir + i][0], cur.col + Dir[vertical_dir + i][1]};
                    Point t_next = {next.row + Dir[vertical_dir + i][0],next.col + Dir[vertical_dir + i][1]};
                    
                    if(t_next.chkRange() && ver_next.chkRange() && !wall_grid[cur.row][cur.col][ver_next.row][ver_next.col] &&
                       !wall_grid[ver_next.row][ver_next.col][t_next.row][t_next.col])
                        next_q.push(make_pair(cnt-1,t_next));
                }
            }
        }
    }
    
    return result_grid;
}

vector<vector<int>> adjust_temp(const vector<vector<int>> &temp_grid, const vector<vector<vector<vector<bool>>>> &wall_grid)
{
    vector<vector<int>> oper_grid(R, vector<int>(C, 0));
    
    for(int row = 0; row < R; ++row)
    {
        for(int col = 0; col < C; ++col)
        {
            Point next;
            for(int dir_ind = 0; dir_ind < 4; dir_ind += 3)
            {
                next = {row + Dir[dir_ind][0], col + Dir[dir_ind][1]};
                
                if(next.chkRange() && !wall_grid[row][col][next.row][next.col])
                {
                    int cur_temp = temp_grid[row][col], next_temp = temp_grid[next.row][next.col];
                    
                    oper_grid[row][col] += floor((next_temp - cur_temp) / 4);
                    oper_grid[next.row][next.col] += floor((cur_temp - next_temp) / 4);
                }
                
            }
        }
    }
    
    return oper_grid;
}

void reflection_temp (vector<vector<int>> &temp_grid, const vector<vector<int>> &calc_grid)
{
    for(int row = 0; row < R; ++row)
        for(int col = 0; col < C; ++col)
            temp_grid[row][col] += calc_grid[row][col];
}

void decrese_edge_temp(vector<vector<int>> &temp_grid)
{
    for(int row = 0; row < R; ++row)
    {
        temp_grid[row][0] = (temp_grid[row][0] == 0 ? 0 : temp_grid[row][0] - 1);
        temp_grid[row][C-1] = (temp_grid[row][C-1] == 0 ? 0 : temp_grid[row][C-1] - 1);
    }
    for(int col = 1; col < C-1; ++col)
    {
        temp_grid[0][col] = (temp_grid[0][col] == 0 ? 0 : temp_grid[0][col] - 1);
        temp_grid[R-1][col] = (temp_grid[R-1][col] == 0 ? 0 : temp_grid[R-1][col] - 1);
    }
}

bool check_minimum_temp(const vector<vector<int>> &temp_grid, const vector<Point> &toCheck_v)
{
    for(int i=0; i < toCheck_v.size(); ++i)
        if(temp_grid[toCheck_v[i].row][toCheck_v[i].col] < K)
            return 0;
    return 1;
}

int main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    
    int chocolate_cnt = 0;
    
    vector<circulator> cir_v;
    vector<Point> toCheck_v;
    vector<vector<int>> temp_grid;
    vector<vector<vector<vector<bool>>>> wall_grid;
    
    cin>>R>>C>>K;
    temp_grid.assign(R, vector<int>(C, 0));
    wall_grid.assign(R, vector<vector<vector<bool>>>(C, vector<vector<bool>>(R, vector<bool>(C,0))));
    
    
    for(int row = 0; row < R; ++row)
        for(int col = 0; col < C; ++col)
        {
            int temp;
            cin>>temp;
            
            if(temp == 5)
                toCheck_v.push_back({row,col});
            else if(temp != 0)
                cir_v.push_back({{row,col},temp-1});
        }
    
    int K;
    cin>>K;
    for(int i = 0; i < K; ++i)
    {
        int row, col, t;
        cin>>row>>col>>t;
        
        row--; col--;
        Point next = {row - !t, col + t};
        
        if(next.chkRange())
        {
            wall_grid[row][col][next.row][next.col] = 1;
            wall_grid[next.row][next.col][row][col] = 1;
        }
    }
    
    bool flag = 1;
    while(flag && chocolate_cnt <= 100)
    {
        for(int i = 0; i < cir_v.size(); ++i)
        {
            vector<vector<int>> calc_grid = blow(cir_v[i], wall_grid);
            reflection_temp(temp_grid, calc_grid);
        }
        
        vector<vector<int>> oper_grid = adjust_temp(temp_grid, wall_grid);
        
        reflection_temp(temp_grid, oper_grid);
        
        decrese_edge_temp(temp_grid);
        
        chocolate_cnt++;
        
        flag = !check_minimum_temp(temp_grid, toCheck_v);
    }
    
    cout<<chocolate_cnt;
    
    return 0;
}
