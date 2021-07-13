#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	//station is empty
	station->free_seats = 0;
	station->passengers_entered = 0;
	station->waiting_passengers = 0;
	pthread_mutex_init(&(station->t_mutex),NULL);
	pthread_cond_init(&(station->train_arrival),NULL);
	pthread_cond_init(&(station->train_leave),NULL);

}

void
station_load_train(struct station *station, int count)
{
	//enter critical section
	pthread_mutex_lock(&(station->t_mutex));
	//no free seats or no waiting passengers
	if(!count || !station->waiting_passengers){
		pthread_mutex_unlock(&(station->t_mutex));
		return;
	}
	station->free_seats = count;
	//wake up all passengers
	pthread_cond_broadcast(&(station->train_arrival));
	// waiting passengers to get on the train
	pthread_cond_wait(&(station->train_leave),&(station->t_mutex));
	station->free_seats = 0;
	pthread_mutex_unlock(&(station->t_mutex));
}

void
station_wait_for_train(struct station *station)
{
	//enter critical section
	pthread_mutex_lock(&(station->t_mutex));
	station->waiting_passengers++;
	// waiting for free seats
	while(!station->free_seats){
		pthread_cond_wait(&(station->train_arrival),&(station->t_mutex));
	}
	station->free_seats--;
	station->passengers_entered++;
	station->waiting_passengers--;
	pthread_mutex_unlock(&(station->t_mutex));
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&(station->t_mutex));
	station->passengers_entered--;
	//all passegers on the train
	if(!station->passengers_entered && !(station->waiting_passengers && station->free_seats)){
		pthread_cond_signal(&(station->train_leave));
	}
	pthread_mutex_unlock(&(station->t_mutex));
}
