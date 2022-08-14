import sys
R, C, M = map(int, input().split())
dx = [-1, 1, 0, 0]
dy = [0, 0, 1, -1]

shark_list = []
ans = 0
fishing_zone = [[[] for _ in range(C)] for _ in range(R)]

for i in range(M):
    r, c, s, d, z = map(int, sys.stdin.readline().rstrip().split())
    fishing_zone[r - 1][c - 1].append([s, d - 1, z])
    # d 0 위, 1 아래, 2 우, 3 좌

def shark_moving():
    temp = [[[] for _ in range(C)] for _ in range(R)]
    for i in range(R):
        for j in range(C):
            if fishing_zone[i][j]:
                x, y = i, j
                s, d, z = fishing_zone[i][j][0]
                temp_s = s

                while temp_s > 0:
                    nx, ny = x + dx[d], y + dy[d]

                    if 0 <= nx < R and 0 <= ny < C:
                        x, y = nx, ny
                        temp_s -= 1

                    else:
                        if d == 0:
                            d = 1
                        elif d == 1:
                            d = 0
                        elif d == 2:
                            d = 3
                        elif d == 3:
                            d = 2
                        continue

                temp[x][y].append([s, d, z])

    for i in range(R):
        for j in range(C):
            fishing_zone[i][j] = temp[i][j]

for i in range(C): # 낚시왕이 오른쪽으로 한칸 이동
    for j in range(R):
        if fishing_zone[j][i]:
            ans += fishing_zone[j][i][0][2]
            fishing_zone[j][i].remove(fishing_zone[j][i][0])
            break

    shark_moving()

    for p in range(R):
        for q in range(C):
            if len(fishing_zone[p][q]) > 1:
                fishing_zone[p][q].sort(key= lambda x: x[2], reverse=True)
                while len(fishing_zone[p][q]) > 1:
                    fishing_zone[p][q].pop()

print(ans)