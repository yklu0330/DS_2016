#include <iostream>
#include <fstream>

using namespace std;

struct _edge{
    int v1, v2;
    int length;
}edge[100000];

int main(int argc, const char * argv[])
{
    ifstream ifs(argv[1], ios::in);
    
    int vertexNum, edgeNum;
    while (ifs >> vertexNum >> edgeNum)
    {
	    int dis[vertexNum][vertexNum];
	    int i, j;
	    for (i = 0; i < vertexNum; i++)
	        for (j = 0; j < vertexNum; j++)
	            dis[i][j] = -1;
	    
	    for (i = 0; i < edgeNum; i++)
	    {
	        int vertex1, vertex2, len;
	        ifs >> vertex1 >> vertex2 >> len;
	        dis[vertex1][vertex2] = len;
	        dis[vertex2][vertex1] = len;
	        edge[i].v1 = vertex1;
	        edge[i].v2 = vertex2;
	        edge[i].length = len;
	    }
	    
	    for (i = 0; i < edgeNum; i++)
	    {
	        for (j = i + 1; j < edgeNum; j++)
	        {
	            if (edge[i].length > edge[j].length)
	            {
	                struct _edge temp = edge[i];
	                edge[i] = edge[j];
	                edge[j] = temp;
	            }
	        }
	    }
	    
	    int graph[vertexNum];
	    for (i = 0; i < vertexNum; i++)
	        graph[i] = -1;
	    int line = 0;
	    int mstEdgeCount = 0;
	    int pairA[vertexNum], pairB[vertexNum];
	    for (i = 0; i < edgeNum; i++)
	    {
	        if (graph[edge[i].v1] == -1 && graph[edge[i].v2] == -1)
	        {
	            line++;
	            graph[edge[i].v1] = line;
	            graph[edge[i].v2] = line;
	            pairA[mstEdgeCount] = edge[i].v1;
	            pairB[mstEdgeCount] = edge[i].v2;
	            mstEdgeCount++;
	        }
	        else if (graph[edge[i].v1] != -1 && graph[edge[i].v2] == -1)
	        {
	            graph[edge[i].v2] = graph[edge[i].v1];
	            pairA[mstEdgeCount] = edge[i].v1;
	            pairB[mstEdgeCount] = edge[i].v2;
	            mstEdgeCount++;
	        }
	        else if (graph[edge[i].v1] == -1 && graph[edge[i].v2] != -1)
	        {
	            graph[edge[i].v1] = graph[edge[i].v2];
	            pairA[mstEdgeCount] = edge[i].v1;
	            pairB[mstEdgeCount] = edge[i].v2;
	            mstEdgeCount++;
	        }
	        else if (graph[edge[i].v1] == graph[edge[i].v2])
	            continue;
	        else if (graph[edge[i].v1] != graph[edge[i].v2])
	        {
	            if (graph[edge[i].v1] > graph[edge[i].v2])
	                graph[edge[i].v2] = graph[edge[i].v1];
	            else
	                graph[edge[i].v1] = graph[edge[i].v2];
	            pairA[mstEdgeCount] = edge[i].v1;
	            pairB[mstEdgeCount] = edge[i].v2;
	            mstEdgeCount++;
	        }
	        
	        if (mstEdgeCount == vertexNum - 1)
	            break;
	    }
	    
	    if (mstEdgeCount < vertexNum - 1)
	    {
	    	cout << "No spanning tree";
		}
	    else
	    { 
		    for (i = 0; i < mstEdgeCount; i++)
		    {
		        cout << "(" << pairA[i] << ", " << pairB[i] << "); ";
		    }
	    } 
	    
	    
	    int stPoint, endPoint;
	    while (ifs >> stPoint >> endPoint)
	    {
	    	if (stPoint == endPoint)
			{
				cout << "\n(" << stPoint << ", " << endPoint << "); ";
				continue;
			}
		    int walk[vertexNum], visit[vertexNum];
		    for (i = 0; i < vertexNum; i++)
		    {
		    	walk[i] = 2100000000;
		    	visit[i] = 0;
			}
		    int point = stPoint;
		    int path[vertexNum];
		    walk[stPoint] = 0;
		    for (i = 0; i < vertexNum; i++)
		        path[i] = -1;
		    while (point != endPoint)
		    {
		        for (j = 0; j < vertexNum; j++)
		        {
		            if (dis[point][j] != -1 && walk[point] + dis[point][j] < walk[j])
		            {
		                walk[j] = dis[point][j];
		                path[j] = point;
		                
		            }
		        }
		        visit[point]++;
		        int nextExistFlag = false;
		        int minWalk = 2000000000;
		        for (j = 0; j < vertexNum; j++)
		        {
		            if (walk[j] < minWalk && visit[j] == 0)
		            {
		                point = j;
		                nextExistFlag = true;
		            }
		        }
		        if (nextExistFlag == false)
		        {
		        	point = -1;
		        	break;
				}
		    }
		    int point1[vertexNum], point2[vertexNum], pointIdx = 0;
		    for (i = 0; i < vertexNum; i++)
		    {
		    	point1[i] = 0;
		    	point2[i] = 0;
			}
		    if (point != -1)
		    {
			    for (i = endPoint; i != -1; i=path[i])
			    {
			    	point1[pointIdx] = path[i];
			    	point2[pointIdx] = i;
			    	pointIdx++;
				}
				cout << endl;
				for (i = pointIdx - 2; i >= 0; i--)
				{
					cout << "(" << point1[i] << ", " << point2[i] << "); ";
				}
			}
			else
				cout << "\nNot exist";
		}
		cout << endl << endl;
		ifs.clear();
		char c;
		ifs >> c;
		if (c == 'x')
			break;
	}
    return 0;
}

