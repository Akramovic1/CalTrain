#include <pthread.h>

struct station {
	pthread_mutex_t t_mutex;
	pthread_cond_t train_arrival , train_leave ;
	int free_seats;
	int waiting_passengers;
	int passengers_entered;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);