#define MAX(x,y) x > y ? x : y
#define MIN(x,y) x < y ? x : y
#define GEN_TASK_COST(c,v) v ? rand() % c + 1 : c
#define FOR_EACH(it, container) for(it=container.begin();it!=container.end();it++)
#define FOR_EVER for(;;)