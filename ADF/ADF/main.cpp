#include <windows.h>  //Windows Header
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define EPSILON 0.000001	//浮点数比较大小的精度
#define cube_l	0.1   //cube的最小边长 
#define Radius	1		//单位球
#define INOUT(distance) (Radius-distance)>EPSILON?-1:(distance-Radius)>EPSILON?1:0	
//距球心distance的点是否在球内 -1-内 1-外 0-球面
static float center[3] = { 0,0,0 };//球心


typedef struct Node {
	float x[2], y[2], z[2];	//xmin xmax ymin ymax zmin zmax
	Node* node_points[8];//八个分支
	float cube_length;//边长
}Node;

Node *P_root;

//求两点间距离
float Distance(float a[3], float b[3])
{
	return sqrt((a[0] - b[0]) * (a[0] - b[0]) +
		(a[1] - b[1]) * (a[1] - b[1]) +
		(a[2] - b[2]) * (a[2] - b[2]));
}

//距原点距离
float Distance_0(float x, float y, float z) {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

//节点是否能继续分支
bool CanDivide(Node *p) {
	int IsIn = 0;
	int IsOut = 0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				int inout = INOUT(Distance_0(p->x[i], p->y[j], p->z[k]));
				
				if (inout == -1) {
					IsIn = 1;
				}
				else if (inout == 1) {
					IsOut = 1;
				}
			}
		}
	}

	if ((IsIn == 1) && (IsOut == 1)) {	//至少有一个点在球内，一个点在球外，才继续划分
		return true;
	}
	else {
		return false;
	}


}

//根据x_min,y_min,z_min,new_cube_length生成新的节点并返回
Node* GenerateNode(float x_min,float y_min,float z_min,float new_cube_length) {
	Node*p = (Node*)malloc(sizeof(Node));
	p->cube_length = new_cube_length;
	p->x[0] = x_min;
	p->x[1] = x_min + new_cube_length;

	p->y[0] = y_min;
	p->y[1] = y_min + new_cube_length;

	p->z[0] = z_min;
	p->z[1] = z_min + new_cube_length;
	return p;
}


//对根节点p进行自适应八叉树分支
void ADF(Node *p) {
	//初始化根节点
	if ((p->cube_length > cube_l && CanDivide(p))
		|| p==P_root) {
		float x_min = p->x[0];
		float y_min = p->y[0];
		float z_min = p->z[0];
		float cube_length = p->cube_length;	//父节点p的边长
		float new_cube_length = cube_length / 2;	//新节点的边长
		//生成八个子节点
		p->node_points[0] = GenerateNode(x_min, y_min, z_min, new_cube_length);
		p->node_points[1] = GenerateNode(x_min, y_min, z_min + new_cube_length, new_cube_length);
		p->node_points[2] = GenerateNode(x_min + new_cube_length, y_min, z_min, new_cube_length);
		p->node_points[3] = GenerateNode(x_min + new_cube_length, y_min, z_min + new_cube_length, new_cube_length);
		p->node_points[4] = GenerateNode(x_min + new_cube_length, y_min + new_cube_length, z_min, new_cube_length);
		p->node_points[5] = GenerateNode(x_min + new_cube_length, y_min + new_cube_length, z_min + new_cube_length, new_cube_length);
		p->node_points[6] = GenerateNode(x_min, y_min + new_cube_length, z_min, new_cube_length);
		p->node_points[7] = GenerateNode(x_min, y_min + new_cube_length, z_min + new_cube_length, new_cube_length);

		for (int i = 0; i < 8; i++) {
			ADF(p->node_points[i]);	//递归生成子节点
		}
	}
	else {

		for (int i = 0; i < 8; i++) {
			p->node_points[i] = NULL;	//将不需再分的分支置空
		}
		
		
	}

}

void Search(Node*p) {

	//依次画出cube的12条边
	glVertex3f(p->x[1], p->y[1], p->z[1]);
	glVertex3f(p->x[0], p->y[1], p->z[1]);

	glVertex3f(p->x[1], p->y[1], p->z[1]);
	glVertex3f(p->x[1], p->y[0], p->z[1]);

	glVertex3f(p->x[1], p->y[1], p->z[1]);
	glVertex3f(p->x[1], p->y[1], p->z[0]);

	glVertex3f(p->x[0], p->y[0], p->z[1]);
	glVertex3f(p->x[0], p->y[1], p->z[1]);

	glVertex3f(p->x[0], p->y[0], p->z[1]);
	glVertex3f(p->x[1], p->y[0], p->z[1]);

	glVertex3f(p->x[0], p->y[0], p->z[1]);
	glVertex3f(p->x[0], p->y[0], p->z[0]);

	glVertex3f(p->x[1], p->y[0], p->z[0]);
	glVertex3f(p->x[1], p->y[0], p->z[1]);

	glVertex3f(p->x[1], p->y[0], p->z[0]);
	glVertex3f(p->x[1], p->y[1], p->z[0]);

	glVertex3f(p->x[1], p->y[0], p->z[0]);
	glVertex3f(p->x[0], p->y[0], p->z[0]);

	glVertex3f(p->x[0], p->y[1], p->z[0]);
	glVertex3f(p->x[0], p->y[0], p->z[0]);

	glVertex3f(p->x[0], p->y[1], p->z[0]);
	glVertex3f(p->x[0], p->y[1], p->z[1]);

	glVertex3f(p->x[0], p->y[1], p->z[0]);
	glVertex3f(p->x[1], p->y[1], p->z[0]);

	
	for (int i = 0; i < 8; i++) {
		if (p->node_points[i] != NULL) {
			Search(p->node_points[i]);
		}
	}



	

	return;
}


//使用opengl展示
void display_voxel() {

	//缩放比例
	glScalef(0.7, 0.7, 0.7);
	if (P_root == NULL)
	{
		printf("根节点为空");
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT);//清颜色缓冲区
	glVertex2f(1, 1);
	glBegin(GL_LINES);
	Search(P_root);
	glEnd();
	glFlush();
}


int main(int argc, char** argv) {
	//初始化八叉树根节点，边长为(Radius + cube_l)*2
	float min= -(Radius+ cube_l);
	float max= (Radius + cube_l);
	P_root = GenerateNode(min, min, min, (Radius + cube_l)*2);

	ADF(P_root);

	//printf("%f", P_root->node_points[0]->node_points[0]->node_points[0]->x[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("单位球");
	//视角绕xyz各转45度
	glRotatef(45.0, 0.0, -1, 0.0);
	glRotatef(45.0, 0.0, 0, -1.0);
	glRotatef(75.0, -1.0, 0, 0.0);
	glutDisplayFunc(display_voxel);
	glutMainLoop();
	int i = 0;
	scanf("%d", &i);
	scanf("%d", &i);

	return 0;
}