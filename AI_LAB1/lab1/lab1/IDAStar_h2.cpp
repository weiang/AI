#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

int iabs(int x)
{
	return x > 0 ? x : -x;
}

enum Action {
	Up, 
	Down, 
	Left,
	Right
};

// Goal status
int goalStatus[5][3];

class Node 
{
public:
	Node(int s[5][3], enum Action a = Up, int g = 0, int h = 0, Node *p = NULL)
	: action(a), gx(g), hx(h), parent(p)
	{
		for (int i = 0; i != 5; i ++) {
			for (int j = 0; j != 3; j ++)
				status[i][j] = s[i][j];
		}
	}
	Node(const Node& n)
	: action(n.action), gx(n.gx), hx(n.hx), parent(n.parent)
	{
		for (int i = 0; i != 5; i ++)
			for (int j = 0; j != 3; j ++)
				status[i][j] = n.status[i][j];
	}

	void show()
	{
		for (int i = 0; i != 5; i ++) {
			for (int j = 0; j != 3; j ++) 
				cout << status[i][j] << " ";
			cout << endl;
		}
	}
	Node *parent;
	enum Action action;
	int gx;
	int hx;	// h(x) + g(x)
	int status[5][3];
};


int h2(Node* currentStatus)
{
	int result = 0;
	for (int i = 0; i != 5; i ++) {
		for (int j = 0; j != 3; j ++) {
			if (currentStatus -> status[i][j] == goalStatus[i][j])
				continue;
			int k, l;
			int flag = false;
			for (k = 0; k != 5; k ++) {
				for (l = 0; l != 3; l ++) {
					if (currentStatus -> status[i][j] == goalStatus[k][l]) {
						flag = true;
						break;
					}
				}
				if (flag)
					break;
			}
			result += (iabs(i - k) + iabs(j - l));
		}
	}
	return result;
}


vector<Node *> nextStatus(Node *node)
{
	vector<Node *> result;
	int i, j;
	for (i = 0; i != 5; i ++) {	// Find 0
		int flag = false;
		for (j = 0; j != 3; j ++) {
			if (node -> status[i][j] == 0) {
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}
//	cout << "i = " << i << ", j = " << j << endl;
	// Up direction
	if ((i - 1 >= 0) && (j != 1 || (i != 2 && i != 4))) {
		Node *upNode = new Node(*node);
		upNode -> status[i][j] = upNode -> status[i - 1][j];
		upNode -> status[i - 1][j] = 0;
		upNode -> action = Up;
		upNode -> hx = h2(upNode);
		upNode -> gx ++;
		upNode -> parent = node;
		result.push_back(upNode);
	}
	// Down direction
	if ((i + 1 <= 4) && (j != 1 || (i != 0 && i != 2))) {
		Node *downNode = new Node(*node);
		downNode -> status[i][j] = downNode -> status[i + 1][j];
		downNode -> status[i + 1][j] = 0;
		downNode -> action = Down;
		downNode -> hx = h2(downNode);
		downNode -> gx ++;
		downNode -> parent = node;
		result.push_back(downNode);
	}
	// Left direction
	if (j - 1 >= 0 && 
		 !(j - 1 == 1 && (i == 1 || i == 3))) {
		Node *leftNode = new Node(*node);
		leftNode -> status[i][j] = leftNode -> status [i][j - 1];
		leftNode -> status[i][j - 1] = 0;
		leftNode -> action = Left;
		leftNode -> hx = h2(leftNode);
		leftNode -> gx ++;
		leftNode -> parent = node;
		result.push_back(leftNode);
	}
	// Right direction
	if (j + 1 <= 2 &&  
		!(j + 1 == 1 && (i == 1 || i == 3))) {
		Node *rightNode = new Node(*node);
		rightNode -> status[i][j] = rightNode -> status[i][j + 1];
		rightNode -> status[i][j + 1] = 0;
		rightNode -> action = Right;
		rightNode -> hx = h2(rightNode);
		rightNode -> gx ++;
		rightNode -> parent = node;
		result.push_back(rightNode);
	}	
	return result;
}

int minFx(vector<Node *>ls)
{
	if (ls.empty())
		return -1;
	int minNum = 0;
	int minValue = ls[0] -> hx + ls[0] -> gx;
	for (int i = 1; i != ls.size(); i ++) {
		int tmp = ls[i] -> hx + ls[i] -> gx;
		if (tmp < minValue) {
			minValue = tmp;
			minNum = i;
		}
	}
	return minNum;
}

/*
void showResult(Node *status)
{
	if (status -> parent == NULL) 
		return;
	showResult(status -> parent);
	switch (status -> action) {
		case Up:
			cout << "U";
			break;
		case Down:
			cout << "D";
			break;
		case Left:
			cout << "L";
			break;
		case Right:
			cout << "R";
	}
	cout << " ";
}
*/
void showResult(Node *status)
{
	char arr[100];
	int i;
	for (i = 0; status -> parent != NULL;  i ++) {
		switch (status -> action) {
			case Up:
				arr[i] = 'U';
				break;
			case Down:
				arr[i] = 'D';
				break;
			case Left:
				arr[i] = 'L';
				break;
			case Right:
				arr[i] = 'R';
				break;
		}
		status = status -> parent;
	}
	i --;
	for (; i >= 0; i --) {
		cout << arr[i] << " ";
	}
	cout << endl;
}
bool testGoal(Node *s)
{
	return s -> hx == 0;
}
/*
 * Return if get result
 */
const int LIMIT = 1000;
bool IDAStar(Node *root)
{
	vector<Node *>ls;
	ls.push_back(root);
	Node *selectedStatus;
	
	int d_limit = LIMIT;
	int next_d_limit = LIMIT;
	while (!ls.empty()) {
		int minNum = minFx(ls);
		selectedStatus = ls[minNum];
		ls.erase(ls.begin() + minNum);
		int ds = selectedStatus -> hx + selectedStatus -> gx;
		if (ds > d_limit) {
			next_d_limit = min(next_d_limit, ds);
		}
		else {
			if (testGoal(selectedStatus))
				break;
			vector<Node *>nStatus = nextStatus(selectedStatus);
			for (int i = 0; i != nStatus.size(); i ++)
				ls.push_back(nStatus[i]);
		}
		d_limit = next_d_limit;
	}
	if (selectedStatus -> hx != 0)
		return false;
	cout << endl;
	selectedStatus -> show();
	showResult(selectedStatus);
	return true;
}

int main()
{
	int initStatus[5][3];
	for (int i = 0; i != 5; i ++) {	
		cin >> initStatus[i][0] >> initStatus[i][1] >> initStatus[i][2];
		cout << initStatus[i][0] << " ";
		cout << initStatus[i][1] << " ";
		cout << initStatus[i][2] << endl;
	}
	cout << endl;
	for (int i = 0; i != 5; i ++) {
		cin >> goalStatus[i][0] >> goalStatus[i][1] >> goalStatus[i][2];
		cout << goalStatus[i][0] << " ";
		cout << goalStatus[i][1] << " ";
		cout << goalStatus[i][2] << endl;
	}


	Node *root = new Node(initStatus);
	root -> hx = h2(root);
	root -> gx = 0;
	root -> parent = NULL;

	if (!IDAStar(root))
		cout << "No result" << endl;
	return 0;

}
