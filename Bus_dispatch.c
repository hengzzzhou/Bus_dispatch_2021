#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int TOTAL_STATION = 5;
char STRATEGY[5] = "FCFS";
int DISTANCE = 2;

#define clockwise 1
#define static 0
#define counterclockwise -1

struct Bus {
	int pos;
	int state;
} bus;


typedef struct Request { //请求列表
	char type[30];
	int station;
	struct Request *link;
	int t;
	int m;
} Request, *Linklist;


void freelist(Linklist a) { //释放链表
	Linklist b = a, c;

	while (b != NULL) {
		c = b->link;
		free(b);
		b = c;
	}

	a = NULL;
}

int findx1(Linklist *L, int x, char a[]) {
	struct Request *temp = *L;
	struct Request *temp2 = temp->link;
	int kk = 0;

	while (temp2 != NULL) {
		if ((temp2->station - 1)*DISTANCE == x && strcmp(temp2->type, a) == 0) {

			return 1;
		} else {
			temp = temp->link;
			temp2 = temp2->link;
		}
	}


	return 0;

}

void Insert(Linklist *L, char a[], int x) {
	struct Request *temp1 = (struct Request *)malloc(sizeof(struct Request ));
	temp1->link = NULL;
	temp1->station = x;

	if (findx1(L, (x - 1)*DISTANCE, a))
		return;
	strcpy(temp1->type, a);
	temp1->t = 1; //初始化时间
	struct Request *temp2 = *L;

	while (temp2->link != NULL) {
		temp2 = temp2->link;
	}

	temp2->link = temp1;
}

void shijian(Linklist *L) {
	struct Request *temp = (struct Request *)malloc(sizeof(struct Request ));
	temp->link = NULL;
	struct Request *temp2 = *L;

	while (temp2->link != NULL) {

		temp2 = temp2->link;
		temp2->t -= 1;
		//	printf(" %d\n\n",temp2->t);
	}
}

void gengxin(Linklist *L) {
	struct Request *temp = (struct Request *)malloc(sizeof(struct Request ));
	temp->link = NULL;
	struct Request *temp2 = *L;

	while (temp2->link != NULL) {

		temp2 = temp2->link;
		temp2->m = 1;
	}
}

int panm(Linklist *L) {
	struct Request *temp = (struct Request *)malloc(sizeof(struct Request ));
	temp->link = NULL;
	struct Request *temp2 = *L;

	while (temp2->link != NULL) {

		temp2 = temp2->link;

		if	((temp2->station - 1) * DISTANCE == bus.pos  && temp2->m == 1) {
			return 1;
		}
	}

	return 0;
}

void Delete1(Linklist *L) {
	struct Request *temp = *L;
	struct Request *temp2 = temp->link;
	int m = temp2->station;

	while (temp2 != NULL) {
		if (m == temp2->station) {
			temp->link = temp2->link;
			temp2 = temp2->link;
		} else {
			return ;
		}
	}
}

void Delete2(Linklist *L, char a[], int x) {
	struct Request *temp = *L;
	struct Request *temp2 = temp->link;
	char c[62];
	char d[50];
	int k = panm(L);

	while (temp2 != NULL) {
		strcpy(d, temp2->type);

		if (strcmp(temp2->type, "target") == 0 ) {
			if ((temp2->station - 1)*DISTANCE  == bus.pos ) {
				temp->link = temp2->link;
				temp2 = temp2->link;
			} else {
				temp = temp->link;
				temp2 = temp2->link;
			}
		}

		if (strcmp(d, "target") != 0) {
			if (bus.state == 1) {
				strcpy(c, "clockwise");
			} else {
				strcpy(c, "counterclockwise");
			}

			if ((strcmp(c, temp2->type) == 0 && (temp2->station - 1)*DISTANCE  == bus.pos )
			        || ((temp2->station - 1)*DISTANCE  == bus.pos) && k == 1	) {
				temp->link = temp2->link;
				temp2 = temp2->link;
			} else {
				temp = temp->link;
				temp2 = temp2->link;
			}
		}
	}
}

int Delete3(Linklist *L, int x) {
	struct Request *temp = *L;
	struct Request *temp2 = temp->link;
	int kk = 0;

	while (temp2 != NULL) {
		if ((temp2->station - 1)*DISTANCE == x ) {
			temp->link = temp2->link;
			temp2 = temp2->link;
			kk = 1;
		} else {
			temp = temp->link;
			temp2 = temp2->link;
		}
	}

	if (kk == 1) {
		return 1;
	}

	return 0;
}

//文件读取函数

void FileUpdate(FILE *fp) {
	char buf[40];
	char a[20];
	char b[5];

	while (fgets(buf, 100, fp) != NULL) {
		if (buf[0] == '#') {
			continue;
		}

		sscanf( buf, "%s = %s", a, b);
		int b1;
		b1 = atoi(b);

		if (strcmp(a, "DISTANCE") == 0) {
			DISTANCE = b1;
		}

		if (strcmp(a, "TOTAL_STATION") == 0) {
			TOTAL_STATION = b1;
		}

		if (strcmp(a, "STRATEGY") == 0)
			strcpy(STRATEGY, b);
	}
}

//函数
int Scan(struct Bus bus, Linklist L) { //检测当前站台有无请求
	Linklist a = L;
	a = a->link;
	int sta = bus.pos / DISTANCE + 1;

	while (a != NULL) {
		if (sta == a->station)
			return 1;
		a = a->link;
	}

	return 0;
}

void move(struct Bus *bus) {
//	printf("move %d\n", bus->state);

	if (bus->state == 0) {
		return;
	}

	if (bus->state == 1) {
		if (bus->pos + 1 <= TOTAL_STATION * DISTANCE - 1)
			bus->pos += 1;
		else
			bus->pos = 0;
	}

	if (bus->state == -1) {
		if (bus->pos - 1 >= 0)
			bus->pos -= 1;
		else
			bus->pos = TOTAL_STATION * DISTANCE - 1;
	}

}

int  Daozhan(struct Bus bus) { //检测公交是否在站台
	if (bus.pos % DISTANCE == 0) {
		return 1;
	} else
		return 0;
}

//（该函数必须在车行驶的时候使用）
int Changedir_SSTF(Linklist *L) {
	struct Request *temp = *L;
	struct Request *temp2 = temp->link;
	char c[62];
	char d[52];

	while (temp2 != NULL) {
		strcpy(d, temp2->type);

//	printf("%d    %d\n",temp2->station,bus.pos);
		if (strcmp(temp2->type, "target") == 0 ) {

			if ((temp2->station - 1)*DISTANCE  == bus.pos && temp2->t < 0) {
				return 0;
			} else {

				temp = temp->link;
				temp2 = temp2->link;
			}
		}

//	printf("daisdjoaidsjiosajdi");
		if (strcmp(d, "target") != 0) {
			if (bus.state == 1) {
				strcpy(c, "clockwise");
			} else {
				strcpy(c, "counterclockwise");
			}

			if (strcmp(c, temp2->type) == 0 && (temp2->station - 1)*DISTANCE == bus.pos && temp2->t < 0) {

				return 0;
			} else {
				temp = temp->link;
				temp2 = temp2->link;
			}
		}

	}

	return -1;
}

int z = 10;
void choosedir_SSTF(Linklist *L) { //必须	Bus.state == 0时用
	struct Request *temp = *L;
	temp = temp->link;

	if (temp == NULL) {
		bus.state = 0;
		return;//无需求
	}

	int zheng = 0, fan = 0;
	int min = 100;
	int juu;
	int juli;
	int a[100];

	while (temp != NULL) {

		juu = (temp->station - 1) * DISTANCE - bus.pos;//正的加到正向  负的加到反向

		if (juu > 0) {
			juli = juu;

		} else {
			juli = TOTAL_STATION * DISTANCE - abs(juu);
		}

		//正向走的距离
		int juli2 = TOTAL_STATION * DISTANCE - juli; //反向走的距离

		for (int mip = 1; mip <= TOTAL_STATION; mip++) {

			if (a[mip] == juli) {
				juli = 0;
				juli2 = 0;
			}
		}

		for (int mip = 1; mip <= TOTAL_STATION; mip++) {

			if (a[mip] == 0) {
				a[mip] = juli;
			}
		}

		zheng += juli;
		fan += juli2;

		z = (temp->station - 1) * DISTANCE - bus.pos; //需求车站坐标-公交车位置坐标

		if (z == 0) {
			bus.state = 0; //有需求在该站，继续停。
			return ;
		}


		temp = temp->link;

	}

	if (fan == zheng) {
		bus.state = 1;//当正反一样默认正向
	}

	if (zheng < fan) {
		bus.state = 1;
	}

	if (zheng > fan) {
		bus.state = -1;
	}

	if (zheng == 0 && fan == 0) {
		bus.state = 0;
	}
}

void Printf_SSTF(struct Bus bus, Linklist L) { //输出公交当前状态
	Linklist now = L;
	now = now->link;
	printf("BUS:\n");
	printf("position:%d\n", bus.pos);
	printf("target:");

	for (int i = 1; i <= TOTAL_STATION; i++) {

		int p = 0;
		now = L;
		now = now->link;

		while (now != NULL) {
			if (now->station == i && strcmp(now->type, "target") == 0 && now->t <= 0)
				p = 1;
			now = now->link;
		}

		if (p == 0)
			printf("0");
		else
			printf("1");
	}

	now = L;
	now = now->link;
	printf("\n");
	printf("STATION:\n");
	printf("clockwise:");

	for (int i = 1; i <= TOTAL_STATION; i++) {

		int p = 0;
		now = L;
		now = now->link;

		while (now != NULL) {
			if (now->station == i && strcmp(now->type, "clockwise") == 0 && now->t <= 0)
				p = 1;
			now = now->link;
		}

		if (p == 0)
			printf("0");
		else
			printf("1");
	}

	now = L;
	now = now->link;
	printf("\n");
	printf("counterclockwise:");

	for (int i = 1; i <= TOTAL_STATION; i++) {

		int p = 0;
		now = L;
		now = now->link;

		while (now != NULL) {
			if (now->station == i && strcmp(now->type, "counterclockwise") == 0 && now->t <= 0)
				p = 1;
			now = now->link;
		}

		if (p == 0)
			printf("0");
		else
			printf("1");
	}

	printf("\n");
}

int findx(Linklist *L, int x) {
	struct Request *temp = *L;
	struct Request *temp2 = temp->link;
	int kk = 0;

	while (temp2 != NULL) {
		if ((temp2->station - 1)*DISTANCE == x ) {

			return 1;
		} else {
			temp = temp->link;
			temp2 = temp2->link;
		}
	}


	return 0;

}


int Changedir(Linklist *L, struct Bus
              bus) { //返回-1表示换方向 1表示不换方向，0表示停下。（该函数必须在车行驶的时候使用）
	int t = 0;
//	printf("bus.pos  %d\n", bus.pos);
	int half = TOTAL_STATION * DISTANCE / 2;//轨道一半距离
	struct Request *temp = *L;
	temp = temp->link;
	int ab = TOTAL_STATION * DISTANCE;//轨道全距离

	if (temp == NULL) {
		return 0;
	}//无需求了

	while (temp != NULL) {
		int dis1 = (temp->station - 1) * DISTANCE - bus.pos;
		int dis2 = ab  + (temp->station - 1) * DISTANCE - bus.pos ;
		int dis3 = ab - (temp->station - 1) * DISTANCE + bus.pos;

		if (bus.state == 1 && (temp->station - 1) * DISTANCE - bus.pos >= 0 && dis1 <= half) {
			t = 1;
			break;
		}

		if (bus.state == 1 && (temp->station - 1) * DISTANCE - bus.pos  < 0 && dis2 <= half) {
			t = 1;
			break;
		}

		if (bus.state == -1 && (temp->station - 1) * DISTANCE - bus.pos  >= 0 && dis3 <= half) {
			t = 1;
			break;
		}

		if (bus.state == -1 && (temp->station - 1) * DISTANCE - bus.pos < 0 && -1 * dis1 <= half) {
			t = 1;
			break;
		}

		temp = temp->link;
	}

	if (t == 1) {
		return 1;
	}

	if (t == 0) {
		return -1;
	}

}

void ChooseFirstDir(Linklist *L) { //按最新请求变向
	struct Request *temp = *L;
	temp = temp->link;

	if (temp == NULL) {
		bus.state = 0;
		return;
	}

	int juli = abs((temp->station - 1) * DISTANCE - bus.pos);

	if (juli >= TOTAL_STATION * DISTANCE / 2) {
		juli = TOTAL_STATION * DISTANCE - juli;
	}

	int z = (temp->station - 1) * DISTANCE - bus.pos;

	if ((temp->station - 1) * DISTANCE > bus.pos) {
		if ((temp->station - 1) * DISTANCE - bus.pos <= TOTAL_STATION * DISTANCE  / 2) {
			bus.state = 1;
		} else {
			bus.state = -1;
		}
	}

	if ((temp->station - 1) * DISTANCE < bus.pos) {
		if (bus.pos - (temp->station - 1) * DISTANCE >= TOTAL_STATION * DISTANCE / 2) {
			bus.state = 1;
		} else {
			bus.state = -1;
		}
	}

	return;
}

void choosedir(Linklist *L) { //必须	Bus.state == 0时用
	struct Request *temp = *L;
	temp = temp->link;

	if (temp == NULL) {
		bus.state = 0;
		return;//无需求
	}

	int min = 100;

	while (temp != NULL) {
		int juli = abs((temp->station - 1) * DISTANCE - bus.pos);

		if (juli >= TOTAL_STATION * DISTANCE / 2) {
			juli = TOTAL_STATION * DISTANCE - juli;
		}

		int z = (temp->station - 1) * DISTANCE - bus.pos; //需求车站坐标-公交车位置坐标

		if (juli <= min) {
			min = juli;

			if ((temp->station - 1) * DISTANCE > bus.pos) {
				if ((temp->station - 1) * DISTANCE - bus.pos <= TOTAL_STATION * DISTANCE  / 2) {
					bus.state = 1;
				} else {
					bus.state = -1;
				}
			}

			if ((temp->station - 1) * DISTANCE < bus.pos) {
				if (bus.pos - (temp->station - 1) * DISTANCE >= TOTAL_STATION * DISTANCE / 2) {
					bus.state = 1;
				} else {
					bus.state = -1;
				}
			}

			if (z == 0) {
				bus.state = 0; //有需求在该站，继续停。
			}
		}

		temp = temp->link;
	}
}

void Printf(struct Bus bus, Linklist L) { //输出公交当前状态
	Linklist now = L;
	now = now->link;
	printf("BUS:\n");
	printf("position:%d\n", bus.pos);
	printf("target:");

	for (int i = 1; i <= TOTAL_STATION; i++) {

		int p = 0;
		now = L;
		now = now->link;

		while (now != NULL) {
			if (now->station == i && strcmp(now->type, "target") == 0)
				p = 1;
			now = now->link;
		}

		if (p == 0)
			printf("0");
		else
			printf("1");
	}

	now = L;
	now = now->link;
	printf("\n");
	printf("STATION:\n");
	printf("clockwise:");

	for (int i = 1; i <= TOTAL_STATION; i++) {

		int p = 0;
		now = L;
		now = now->link;

		while (now != NULL) {
			if (now->station == i && strcmp(now->type, "clockwise") == 0)
				p = 1;
			now = now->link;
		}

		if (p == 0)
			printf("0");
		else
			printf("1");
	}

	now = L;
	now = now->link;
	printf("\n");
	printf("counterclockwise:");

	for (int i = 1; i <= TOTAL_STATION; i++) {

		int p = 0;
		now = L;
		now = now->link;

		while (now != NULL) {
			if (now->station == i && strcmp(now->type, "counterclockwise") == 0)
				p = 1;
			now = now->link;
		}

		if (p == 0)
			printf("0");
		else
			printf("1");
	}

	printf("\n");
}

void qukongge(char *p) {
	int i;
	i = strlen(p) - 1;

	while (p[i] == ' ' && i > 0)
		i--;
	p[i + 1] = '\0';

	return;

}


int main() {
	FILE *fp = fopen("D:\\xx.txt", "r");
//	FILE *fp = fopen("dict.dic", "r");
	FileUpdate(fp);
	Linklist L;
	L = (struct Request *)malloc(sizeof(struct Request ));
	L->link = NULL;
	char line[100];
	int time = 0;
	bus.pos = 0;
	bus.state = 0;

	if (strcmp("FCFS", STRATEGY) == 0) {
		printf("TIME:%d\n", time);
		Printf(bus, L);
		gets(line);
		int k = 0;

		while (strcmp(line, "end") != 0) {
			if (strcmp(line, "clock") == 0) {
				k = 0;

				if (bus.state == 0)
					ChooseFirstDir(&L);

				if (L->link == NULL)
					bus.state = 0;

				if (Daozhan(bus) && L->link != NULL && L->link->station == bus.pos / DISTANCE + 1) {
					Delete1(&L);

					if (bus.state != 0)
						k = 1;
					ChooseFirstDir(&L);
				}

				if (k == 0)
					move(&bus);
				time++;
				printf("TIME:%d\n", time);
				Printf(bus, L);
			} else {
				char line2[30];
				int s;
				sscanf(line, "%s %d", line2, &s);
				Insert(&L, line2, s);
			}

			gets(line);
		}
	}

	if (strcmp("SCAN", STRATEGY) == 0) {
		printf("TIME:%d\n", time);
		Printf(bus, L);
		gets(line);
		//	qukongge(line);
		int zz = 0;

		while (strcmp(line, "end") != 0) {

			if (strcmp(line, "clock") == 0) {

				time++;

				if (zz == 1) {

					if (Delete3(&L, bus.pos) ) {
						goto out;
					}
				}

				if (bus.state == 0) {
					choosedir(&L);
				} else if (bus.state != 0 && Changedir(&L, bus) == 0) {
					bus.state = 0;
				}

				else	if (bus.state != 0 && Changedir(&L, bus) == -1) {
					if (bus.state == 1) {
						bus.state = -1;
					}

					else	if (bus.state == -1) {
						bus.state = 1;
					}
				}

				//	printf("%d\n", bus.state);
				move(&bus);

out:
				printf("TIME:%d\n", time);
				Printf(bus, L);
				zz = findx(&L, bus.pos);


			} else {
				char line2[50];
				int s;
				sscanf(line, "%s %d", line2, &s);

				if (bus.state == 0 && (s - 1)*DISTANCE == bus.pos) {
					gets(line);
					qukongge(line);
					continue;
				}

				Insert(&L, line2, s);
			}

			gets(line);
			qukongge(line);
		}
	}

	if (strcmp("SSTF", STRATEGY) == 0) {
		gets(line);
		qukongge(line);
		char line2[60];
		int s;

		while (strcmp(line, "end") != 0) {
			z = 1;

			if (strcmp(line, "clock") == 0) {
				printf("TIME:%d\n", time);
				Printf_SSTF(bus, L);
				shijian(&L);

				if (bus.state == 0) {
					choosedir_SSTF(&L);

					if (z == 0) {
						Delete2(&L, line, bus.pos);
					}

					gengxin(&L); //给请求上标记
				} else //判m点
					if ((bus.state != 0 && 	Changedir_SSTF(&L) == 0) || (panm(&L))) {
						// printf("MMMMMMMMMMMMMMMMMMMMMMM\n");
						Delete2(&L, line, bus.pos);
						bus.state = 0;
						//	printf("                                  \n    %d                 \n                  ",bus.state);
					}

				move(&bus);
				time++;

			} else {
				sscanf(line, "%s %d", line2, &s);

				if (bus.state == 0 && (s - 1)*DISTANCE == bus.pos) {
					gets(line);
					qukongge(line);
					continue;
				}

				Insert(&L, line2, s);
			}

			gets(line);
			qukongge(line);
		}

		printf("TIME:%d\n", time);
		Printf_SSTF(bus, L);
	}

	printf("end\n");
	freelist(L);
	return 0;
}
