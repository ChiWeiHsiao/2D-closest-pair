#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<ctime>
#include<utility>
using namespace std;
typedef pair<double,double> Point;
typedef vector<Point> Points;
#define x first
#define y second
bool cmpx(const Point& p1, const Point& p2) {return (p1.x < p2.x);}
bool cmpy(const Point& p1, const Point& p2){return (p1.y < p2.y);}

double Distance(const Point &p1, const Point &p2) {
    double dx = p1.x - p2.x, dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

double enumerate(Points P){//O(n^2) method, enumerate all possible pairs
	int n=P.size();
	double dmin = 1e9;
	for(int i=0; i<n; i++)
		for (int j = i+1; j<n; j++){
            dmin = min(dmin, Distance(P[i], P[j]));
		}
	return dmin;
}

double closest_strip(Points strip){ //MERGE: O(n)
	double dmin = 1e9;
    int s = strip.size();
    for(int i=0;i<s;i++)//O(n)
        //no more than three iterations->constant
        for(int j=1; (strip[i+j].y-strip[i].y<dmin)&& i+j<s; j++)
            dmin = min(dmin, Distance(strip[i], strip[i+j]));
	return dmin;
}

double DnC(Points px, Points py) {
	int n=px.size();
/*BOUNDARY CONDITION: O(1)*/
	if(n <= 3) return enumerate(px);
/*DIVIDE: Divide the problem to left/right subset of points. */
	int mid = n / 2;

	Points pyl, pyr;
	for(int i=0;i<n;i++){
		if(px[i].x <= px[mid].x) pyl.push_back(px[i]);
		else pyr.push_back(px[i]);
	}

	Points pxl = Points(px.begin(), px.begin()+mid);
	Points pxr = Points(px.begin()+mid + 1, px.end());

	double dl = DnC(pxl, pyl);
	double dr = DnC(pxr, pyr);
	double dmin = min(dl, dr);

/*MERGE: O(n), Find if there is any pair across left/right subset is closer. */
    //strip around right bound of left subset (width=2*dmin)
    Points stripl;
    for(int i=0;i<n;i++)
        if( (py[i].x>=px[mid].x&&py[i].x-px[mid].x < dmin) || (py[i].x<px[mid].x && px[mid].x- py[i].x < dmin))
            stripl.push_back(py[i]);
    //strip around right bound of left subset (width=2*dmin)
	Points stripr;
    for(int i=0;i<n;i++)
        if( (py[i].x<=px[mid+1].x && px[mid+1].x- py[i].x < dmin) || (py[i].x>px[mid+1].x&&py[i].x-px[mid+1].x < dmin))
            stripr.push_back(py[i]);

    dmin = min(dmin, closest_strip(stripl));
    dmin = min(dmin, closest_strip(stripr));

	return dmin;
}

/*O(nlogn)*/
double closest(Points p) {
	Points px = p, py = p;

	sort(px.begin(), px.end(), cmpx);
	sort(py.begin(), py.end(), cmpy);

	return DnC(px, py);
}

int main(int argc, char* argv[]){
    int N;
    ifstream fin(argv[1]);
    fin >> N;//number of points
    Points points;
    double tmpx, tmpy;
    for(int i=0; i<N; i++){
        fin >> tmpx >> tmpy;
        points.push_back( make_pair(tmpx, tmpy) );
    }
    fin.close();
    //int t_start=clock();
    printf("Shortest distance: %f\n", closest(points));
   // printf("Time: %d ms\n\n", clock()-t_start);
    return 0;
}
