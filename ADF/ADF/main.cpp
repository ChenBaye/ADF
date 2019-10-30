#include <windows.h>  //Windows Header
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define EPSILON 0.000001	//�������Ƚϴ�С�ľ���
#define cube_l	0.1   //cube����С�߳� 
#define Radius	1		//��λ��
#define INOUT(distance) (Radius-distance)>EPSILON?-1:(distance-Radius)>EPSILON?1:0	
//������distance�ĵ��Ƿ������� -1-�� 1-�� 0-����
static float center[3] = { 0,0,0 };//����


typedef struct Node {
	float x[2], y[2], z[2];	//xmin xmax ymin ymax zmin zmax
	Node* node_points[8];//�˸���֧
	float cube_length;//�߳�
}Node;

Node *P_root;

//����������
float Distance(float a[3], float b[3])
{
	return sqrt((a[0] - b[0]) * (a[0] - b[0]) +
		(a[1] - b[1]) * (a[1] - b[1]) +
		(a[2] - b[2]) * (a[2] - b[2]));
}

//��ԭ�����
float Distance_0(float x, float y, float z) {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

//�ڵ��Ƿ��ܼ�����֧
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

	if ((IsIn == 1) && (IsOut == 1)) {	//������һ���������ڣ�һ���������⣬�ż�������
		return true;
	}
	else {
		return false;
	}


}

//����x_min,y_min,z_min,new_cube_length�����µĽڵ㲢����
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


//�Ը��ڵ�p��������Ӧ�˲�����֧
void ADF(Node *p) {
	//��ʼ�����ڵ�
	if ((p->cube_length > cube_l && CanDivide(p))
		|| p==P_root) {
		float x_min = p->x[0];
		float y_min = p->y[0];
		float z_min = p->z[0];
		float cube_length = p->cube_length;	//���ڵ�p�ı߳�
		float new_cube_length = cube_length / 2;	//�½ڵ�ı߳�
		//���ɰ˸��ӽڵ�
		p->node_points[0] = GenerateNode(x_min, y_min, z_min, new_cube_length);
		p->node_points[1] = GenerateNode(x_min, y_min, z_min + new_cube_length, new_cube_length);
		p->node_points[2] = GenerateNode(x_min + new_cube_length, y_min, z_min, new_cube_length);
		p->node_points[3] = GenerateNode(x_min + new_cube_length, y_min, z_min + new_cube_length, new_cube_length);
		p->node_points[4] = GenerateNode(x_min + new_cube_length, y_min + new_cube_length, z_min, new_cube_length);
		p->node_points[5] = GenerateNode(x_min + new_cube_length, y_min + new_cube_length, z_min + new_cube_length, new_cube_length);
		p->node_points[6] = GenerateNode(x_min, y_min + new_cube_length, z_min, new_cube_length);
		p->node_points[7] = GenerateNode(x_min, y_min + new_cube_length, z_min + new_cube_length, new_cube_length);

		for (int i = 0; i < 8; i++) {
			ADF(p->node_points[i]);	//�ݹ������ӽڵ�
		}
	}
	else {

		for (int i = 0; i < 8; i++) {
			p->node_points[i] = NULL;	//�������ٷֵķ�֧�ÿ�
		}
		
		
	}

}

void Search(Node*p) {

	//���λ���cube��12����
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


//ʹ��openglչʾ
void display_voxel() {

	//���ű���
	glScalef(0.7, 0.7, 0.7);
	if (P_root == NULL)
	{
		printf("���ڵ�Ϊ��");
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT);//����ɫ������
	glVertex2f(1, 1);
	glBegin(GL_LINES);
	Search(P_root);
	glEnd();
	glFlush();
}


int main(int argc, char** argv) {
	//��ʼ���˲������ڵ㣬�߳�Ϊ(Radius + cube_l)*2
	float min= -(Radius+ cube_l);
	float max= (Radius + cube_l);
	P_root = GenerateNode(min, min, min, (Radius + cube_l)*2);

	ADF(P_root);

	//printf("%f", P_root->node_points[0]->node_points[0]->node_points[0]->x[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("��λ��");
	//�ӽ���xyz��ת45��
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