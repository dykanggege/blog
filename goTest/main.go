package main

import (
	"strconv"
)

func main(){

}

func numIslands(grid [][]byte) int {
	height := len(grid)
	if height == 0{
		return 0
	}
	width := len(grid[0])
	if width == 0{
		return 0
	}


	visited := [][]byte{}
	for i := 0; i < height; i++{
		arr := make([]byte, width)
		visited = append(visited,arr)
	}
	
	flags := [][]int{{1,0},{0,1},{-1,0},{0,-1}}
	sum := 0
	for i := 0; i < height; i++{
		for j := 0; j < width; j++{
			if grid[i][j] == '1' && visited[i][j] != 1{
				sum++
				dfs(i,j,height,width,grid,visited,flags)
			}
		}
	}

	s := "kangege"
	for _, c := range s{
	}

    return 0
}

func dfs(i,j,h,w int,grid [][]byte,visited [][]byte,flag [][]int){
	grid[i][j] = '0'
	for _, offset := range flag{
		x,y := i+offset[0],j+offset[1]
		if safe(x,y,h,w) && visited[x][y] != 1{
			visited[x][y] = 1
			dfs(x,y,h,w,grid,visited,flag)
		}
	}
}

func safe(i,j,h,w int) bool {
	if i < 0 || j < 0 || i >= h || j >= w{
		return false
	}
	return true
}