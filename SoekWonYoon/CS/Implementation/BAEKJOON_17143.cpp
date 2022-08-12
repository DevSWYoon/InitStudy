//MEMORY USAGE : 2684KB , TIME REQUIRED : 116ms
#include <iostream>
#include <vector>
#include <list>
using namespace std;

const int Dir[4][2] = {{-1,0}, {1,0}, {0,1}, {0,-1}};

int R, C;

class shark {
private:
    int size;
    int speed;
    int dir_ind;
    int row;
    int col;
public:
    shark() {}
    
    shark(int _row, int _col, int _speed, int _dir_ind, int _size) : row(_row), col(_col), speed(_speed), dir_ind( _dir_ind), size(_size){}
    
    ~shark() {}
    
    int getSize(void) {return size;}
    
    pair<int,int> getLocation(void) {return make_pair(row, col);}
    
    void move(list<shark>::iterator &it,list<shark> &shark_list, vector<vector<list<shark>::iterator>> &grid, const list<shark>::iterator &empty_flag) {
        for(int remain_move = speed % (2 * ((Dir[dir_ind][0] != 0 ? R : C) - 1)); remain_move > 0; --remain_move)
        {
            int next_row = row + Dir[dir_ind][0];
            int next_col = col + Dir[dir_ind][1];
            
            if(next_row == R || next_row == -1 || next_col == C || next_col == -1)
                dir_ind = !(dir_ind - (dir_ind > 1 ? 2 : 0)) + (dir_ind > 1 ? 2 : 0);
            
            row += Dir[dir_ind][0];
            col += Dir[dir_ind][1];
        }
        
        pair<int,int> cur_loc = getLocation();
        
        if(grid[cur_loc.first][cur_loc.second] == empty_flag || it->getSize() > grid[cur_loc.first][cur_loc.second]->getSize())
        {
            if(grid[cur_loc.first][cur_loc.second] != empty_flag)
                shark_list.erase(grid[cur_loc.first][cur_loc.second]);
            grid[cur_loc.first][cur_loc.second] = it;
            it++;
        }
        else
            it = shark_list.erase(it);
    }
};

int main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    
    int M;
    int total_size = 0;
    list<shark> shark_list;
    list<shark> dummy_list;
    vector<vector<list<shark>::iterator>> grid;
    list<shark>::iterator empty_flag = dummy_list.end();
    
    cin>>R>>C>>M;
    grid.assign(R, vector<list<shark>::iterator>(C, empty_flag));
    
    for(int i=0; i<M; ++i)
    {
        int r, c, s, d, z;
        cin>>r>>c>>s>>d>>z;
        
        r--; c--;
        shark_list.push_back(shark(r,c,s,d-1,z));
        grid[r][c] = next(shark_list.end(), -1);
    }
    
    for(int time = 0; time < C; time++)
    {
        int row = 0;
        while(row < R && grid[row][time] == empty_flag) row++;
        
        if(row < R)
        {
            list<shark>::iterator it = grid[row][time];
            grid[row][time] = empty_flag;
            total_size += it->getSize();
            shark_list.erase(it);
        }
        
        pair<int,int> cur_loc;
        for(list<shark>::iterator it = shark_list.begin(); it != shark_list.end(); it++)
        {
            cur_loc = it->getLocation();
            grid[cur_loc.first][cur_loc.second] = empty_flag;
        }
        
        for(list<shark>::iterator it = shark_list.begin(); it != shark_list.end(); )
        {
            it->move(it, shark_list, grid, empty_flag);
        }
    }
    
    cout<<total_size;
    
    return 0;
}
