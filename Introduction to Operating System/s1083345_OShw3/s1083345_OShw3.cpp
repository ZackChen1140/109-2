#include<iostream>
#include<ctime>
#include<utility>
#include<queue>
#include<pthread.h>
#include<unistd.h>

using namespace std;

struct pork
{
	int id;
	bool cut;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//mutex

int btime = 0;//background time
int porkN = 10;//the total number of pieces of pork;
int preQN = 5;//prepare queue capacity, size of preQ + size of preQ2 <= preQN
int porkid = 1;//the identifier of pork, count from 1
int cutcount = 10;//how many pieces of prok have not been cut
int packcount = 10;//how many pieces of prok have not been packed
bool cutWork = false, packWork = false;//Judging the status of the factories
queue<pork> preQ, preQ2;//prepare queue, divided into uncut and cut;
priority_queue<pair<pork, int>> frezQ;//freezer space

//thread function
void *cutter(void *noth);
void *packer(void *noth);
void *freezer(void *noth);
void *timer(void *noth);

bool operator<(pair<pork, int> a, pair<pork, int> b)//sorting rule of the priority_queue
{
	return a.second > b.second;
}

void wait(int ms)//
{
	usleep(ms*1000);
}

void putinQ(pork p)//put the pork in prepare queue
{
	pthread_mutex_lock(&mutex);
	p.cut == true ? preQ2.push(p) : preQ.push(p);
	pthread_mutex_unlock(&mutex);
}

void freeze(pork p)//estimated freezing time and put the pork in the freezer space
{
	int freztime = (rand() % 2 + 3) * 100 + rand() % 11 * 10;
	frezQ.push(make_pair(p, btime + freztime));
	cout << btime << "ms -- Pork#" << p.id << " has been sent to the Freezer - "<< freztime << "ms\n";
}

int main(int argc, char *argv[])
{
	//input
	if(argc>1)
	{
		porkN = atoi(argv[1]);
		cutcount = porkN;
		packcount = porkN;
		if(argc == 3)
			preQN = atoi(argv[2]);
	}	
	
	//initial threads
	pthread_t cutid, packid, frezid, timeid;
	pthread_attr_t cutattr, packattr, frezattr, timeattr;
	
	pthread_attr_init(&cutattr);
	pthread_attr_init(&packattr);
	pthread_attr_init(&frezattr);
	pthread_attr_init(&timeattr);
	
	//create threads
	pthread_create(&cutid, NULL, cutter, NULL);
	pthread_create(&packid, NULL, packer, NULL);
	pthread_create(&frezid, NULL, freezer, NULL);
	pthread_create(&timeid, NULL, timer, NULL);
	
	srand(3);
	while(porkN)
	{
		wait((rand() % 6 + 5) * 10);
		if((preQ.size() + preQ2.size()) >= preQN)//if the prepare queue is full, sent the pork to the freezer
			freeze({porkid, false});
		else//put the pork in the prepare queue
		{
			putinQ({porkid, false});
			cout << btime << "ms -- Pork#" << porkid << ": waiting in the slot\n";
		}
		--porkN, ++porkid;
	}
	
	//wait for the threads to exit
	pthread_join(cutid, NULL);
	pthread_join(packid, NULL);
	pthread_join(frezid, NULL);
	pthread_join(timeid, NULL);
	
	return 0;
}

void *cutter(void *noth)
{
	while(cutcount)
	{
		while(preQ.empty());
		
		//take out from the prepare queue
		pthread_mutex_lock(&mutex);
		pork p = preQ.front();
		preQ.pop();
		pthread_mutex_unlock(&mutex);
		
		cout << btime << "ms -- Pork#" << p.id << ": enters the CUTTER\n";
		
		
		//start to cut
		cutWork = true;
		//cutting
		int cuttime = (rand() % 2 + 1) * 100 + rand() % 11 * 10;
		cout << btime << "ms -- CUTTER: cutting... cutting... Pork#" << p.id << " -- " << cuttime << "ms\n";
		wait(cuttime);
		p.cut = true;
		--cutcount;
		cout << btime << "ms -- Pork#" << p.id << ": Leaves CUTTER(Complete 1st stage)\n";
		//finish cutting
		cutWork = false;
		
		
		//prepare queue is full and the next prok is cut -> place here and wait
		while((preQ.size() + preQ2.size()) == preQN  && preQ.front().cut == true);
	
		putinQ(p);
		cout << btime << "ms -- Pork#" << p.id << ": waiting in the slot(cutted)\n";
	}
	pthread_exit(0);
}

void *packer(void *noth)
{
	while(packcount)
	{
		while(preQ2.empty());
		
		//take out from the prepare queue
		pthread_mutex_lock(&mutex);
		pork p = preQ2.front();
		preQ2.pop();
		pthread_mutex_unlock(&mutex);
		
		cout << btime << "ms -- Pork#" << p.id << ": enters to the factory(PACKER)\n";
		
		
		//start to pack
		packWork = true;
		//packing
		int packtime = (rand() % 5 + 5) * 100 + rand() % 11 * 10;
		cout << btime << "ms -- PACKER: processing & Paking the Pork#" << p.id << " -- " << packtime << "ms\n";
		wait(packtime);
		--packcount;
		cout << btime << "ms -- Pork#" << p.id << ": Leaves PACKER(Complete)\n";
		//finish packing
		packWork = false;
	}
	pthread_exit(0);
}

void *freezer(void *noth)
{
	while(packcount)
	{
		if(!frezQ.empty() && frezQ.top().second <= btime)//its time to defrost
		{
			pork p = frezQ.top().first;
			if((preQ.size() + preQ2.size()) >= preQN)//the prepare queue is still full -> keep freezing
				freeze(p);
			else//take the prok out from the freezer and put it in the prepare queue 
			{
				putinQ(p);
				cout << btime << "ms -- Pork#" << p.id << ": waiting in the slot\n";
			}
			frezQ.pop();
		}
	}
	pthread_exit(0);
}

void *timer(void *noth)
{
	while(packcount)
	{
		//Transfer the massages of the factory status regularly
		if(!cutWork && !packWork)
			cout << btime << "ms -- CUTTER & PACKER: under reviewing together...\n";
		else if(!cutWork && cutcount > 0)
			cout << btime << "ms -- CUTTER: under maintenance.\n";
		else if(!packWork)
			cout << btime << "ms -- PACKER: under maintenance.\n";
		
		//clock
		wait(10);
		pthread_mutex_lock(&mutex);
		btime += 10;
		pthread_mutex_unlock(&mutex);
	}
	cout << btime << "ms -- CUTTER & PACKER: under reviewing together...\n";
	pthread_exit(0);
}
