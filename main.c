#include <pthread.h>
#include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>

// Zmienne, które później chcemy sprawdzić czy są zdefiniowane
// SLEEP_TIME musi przyjmować wartość
// #define SLEEP_TIME
// #define VERBOSE
#define THREAD_NUM 10

// Zmienne modyfikowane przez poszczególne wątki
// Oznaczają aktualną liczbę reniferów i skrzatów
int current_reindeer_count = 0;
int current_gnome_count = 0;

// Na podobnej zasadzie zmienne oznaczające
// liczbę wszystkich obsłużonych reniferów i skrzatów
int total_reindeers = 0;
int total_gnomes = 0;

// Muteksy reniferów i skrzatów
pthread_mutex_t r_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

// Zmienne decyzyjne
pthread_cond_t r_cv = PTHREAD_COND_INITIALIZER;
pthread_cond_t g_cv = PTHREAD_COND_INITIALIZER;

// Funkcja oznaczająca przybycie renifera
void *reindeer_visit(void *arg)
{
	int r_num = *(int *)arg;
	/*
		1. Przybycie
		2. oczekiwanie na pozostałe renifery
		3. jeśli jest >= 9 - wyruszamy w drogę z mikołajem
			TODO - 3.1. Droga może trwać x ms. Opcjonalnie sleep()
		4. Powrót z podróży odprawienie reniferów
	*/
	// printf("\t\t[R%d] Locking mutex\n", r_num);
	pthread_mutex_lock(&r_mutex);
	// printf("\t\t[R%d] Mutex locked\n", r_num);

#ifdef SLEEP_TIME
	printf("[R%d] Going to sleep\n", r_num);
	sleep(SLEEP_TIME);
#endif

	// printf("\t\t[R%d] Signalling\n", r_num);
	// pthread_cond_signal(&r_cv);
	pthread_cond_broadcast(&r_cv);

	// printf("\t\t[R%d] increasing rcount\n", r_num);
	current_reindeer_count++;
	total_reindeers++;

#ifdef VERBOSE
	printf("-> Reindeer nr %d arrived.\n", r_num + 1);
#endif
	// printf("\t\tCurrent nrof reindeers: %d\n", current_reindeer_count);
	pthread_mutex_unlock(&r_mutex);

	// printf("\t\t[R%d] mutex unlocked\n", r_num);
	// printf("\t\t[R%d] Exiting...\n", r_num);
	pthread_exit(NULL);
}

// Funkcja oznaczająca przybycie skrzata
void *gnome_visit(void *arg)
{
	int g_num = *(int *)arg;
	/*
		1. Przybycie
		2. oczekiwanie na pozostałe skrzaty
		3. jeśli jest >= 3 - konsultacje
		4. pożegnanie
	*/
	// printf("\t\t\t\t[G%d] Locking mutex\n", g_num);
	pthread_mutex_lock(&g_mutex);
	// printf("\t\t\t\t[G%d] Mutex locked\n", g_num);

#ifdef SLEEP_TIME
	printf("[G%d] Sleeping\n", g_num + 1);
	sleep(SLEEP_TIME);
#endif

	// printf("\t\t\t\t[G%d] signaling\n", g_num);
	// pthread_cond_signal(&g_cv);
	pthread_cond_broadcast(&g_cv);

	// printf("\t\t\t\t[G%d] incrementing gcount\n", g_num);
	current_gnome_count++;
	total_gnomes++;

#ifdef VERBOSE
	printf("-> Gnome nr %d arrived.\n", g_num + 1);
#endif
	// printf("\t\t\t\tCurrent nrof gnomes: %d\n", current_gnome_count);
	pthread_mutex_unlock(&g_mutex);

	// printf("\t\t\t\t[G%d] mutex unlocked\n", g_num);
	// printf("\t\t\t\t[G%d] exiting...\n", g_num);
	pthread_exit(NULL);
}

// Obsługa reniferów odbywa się w poniższym wątku
void *serve_reindeers(void *arg)
{
	// printf("[Sr] Started serving reindeers.\n");
	// Jeżeli jest już 9 reniferów, zaprzęgamy je i jedziemy rozdać prezenty.
	// Opcjonalnie - podróż może trwać X ms
	int reindeers_served = 0;
	while (reindeers_served < THREAD_NUM)
	{
		// printf("[Sr] Locking reindeer mutex\n");
		pthread_mutex_lock(&r_mutex);
		// printf("[Sr] current reindeer count: %d\n", current_reindeer_count);

		while (current_reindeer_count < 9 && reindeers_served < THREAD_NUM)
		{
			// printf("[Sr] waiting for 9 reindeers\n");
			pthread_cond_wait(&r_cv, &r_mutex);
			// printf("[Sr] received signal/broadcast.\n");
			// printf("[Sr] Current reindeer count: %d\n", current_reindeer_count);

			// reindeers_served++;
			reindeers_served = total_reindeers;
			// printf("[Sr] Total reindeers served = %d\n", reindeers_served);
		}

#ifdef VERBOSE
		printf("[S] 9 Reindeers arrived. Serving...\n");
#endif
		current_reindeer_count = current_reindeer_count - 9;
		// printf("[Sr] unlocking reindeer mutex\n");
		pthread_mutex_unlock(&r_mutex);
	}
	// printf("[Sr] Exiting...\n");
	pthread_exit(NULL);
}

// Wątek do obsługi skrzatów
void *serve_gnomes(void *arg)
{

	// printf("[Sg] Started serving gnomes\n");
	// Jeżeli jest już 3 skrzatów, udzielamy konsultacji i żegnamy

	int gnomes_served = 0;
	// int old_gnome_count = 0;
	while (gnomes_served < THREAD_NUM)
	{
		// printf("[Sg] Locking gnome mutex\n");
		pthread_mutex_lock(&g_mutex);
		// printf("[Sg] current gnome count: %d\n", current_gnome_count);

		while (current_gnome_count < 3 && gnomes_served < THREAD_NUM)
		{
			// printf("[Sg] waiting for 3 gnomes\n");
			pthread_cond_wait(&g_cv, &g_mutex);
			// printf("[Sg] Received signal/broadcast.\n");
			// printf("[Sg] Current gnome count: %d\n", current_gnome_count);

			// gnomes_served++;
			gnomes_served = total_gnomes;
			// printf("[Sg] Total gnomes_served = %d\n", gnomes_served);
		}

#ifdef VERBOSE
		printf("[S] 3 gnomes arrived. Serving...\n");
#endif
		current_gnome_count = current_gnome_count - 3;
		// printf("[Sg] Unlocking gnome mutex\n");
		pthread_mutex_unlock(&g_mutex);
	}
	// printf("[Sg] exiting...\n");
	pthread_exit(NULL);
}

/* Wątek reprezentujący mikołaja,
po uruchomieniu jest od razu wstrzymywany,
mikołaj budzi się gdy liczba reniferów lub skrzatów
przkroczy zadane limity. */
void *santa(void *arg)
{
	pthread_t gt;
	pthread_t rt;

	// Wątek obsługujący renifery
	int err2 = pthread_create(&rt, NULL, serve_reindeers, NULL);
#ifdef SLEEP_TIME
	sleep(SLEEP_TIME);
#endif

	// Wątek obsługujący skrzaty
	int err = pthread_create(&gt, NULL, serve_gnomes, NULL);
#ifdef SLEEP_TIME
	sleep(SLEEP_TIME);
#endif

	// printf("Waiting for servers to finish...\n");
	pthread_join(rt, NULL);
	pthread_join(gt, NULL);
#ifdef VERBOSE
	printf("[*] Servers finished!\n");
#endif
	pthread_exit(NULL);
}

int projekt_zso()
{

	// Renifery mają priorytet przez skrzatami
	// Pojemnik na atrybuty wątku - renifera
	pthread_attr_t r_attr;
	struct sched_param r_sched_params;

	// Priorytet renifera = 50 (im więcej tym lepiej)
	r_sched_params.sched_priority = 50;

	// Inicjalizacja atrybutów renifera
	pthread_attr_init(&r_attr);

	// Ustawienie atrybutów wątku
	pthread_attr_setschedpolicy(&r_attr, SCHED_RR);
	pthread_attr_setschedparam(&r_attr, &r_sched_params);

	// Analogicznie dla skrzatów
	pthread_attr_t g_attr;
	struct sched_param g_sched_param;
	g_sched_param.sched_priority = 40;

	pthread_attr_init(&g_attr);

	pthread_attr_setschedpolicy(&g_attr, SCHED_RR);
	pthread_attr_setschedparam(&g_attr, &g_sched_param);

	// ... i dla mikołaja (tu priorytet najwyższy)
	pthread_attr_t santa_attr;
	struct sched_param santa_param;
	santa_param.sched_priority = 100;

	pthread_attr_init(&santa_attr);
	pthread_attr_setschedparam(&santa_attr, &santa_param);

	pthread_t t_santa;
	pthread_t reindeers[THREAD_NUM];
	pthread_t gnomes[THREAD_NUM];

#ifdef VERBOSE
	printf("* Santa's here with us.\n");
#endif
	pthread_create(&t_santa, &santa_attr, santa, NULL);

#ifdef SLEEP_TIME
	sleep(SLEEP_TIME);
#endif
#ifdef VERBOSE
	printf("* Deploying reindeers and gnomes.\n");
#endif
	for (int i = 0; i < THREAD_NUM; i++)
	{
		pthread_create(&reindeers[i], &r_attr, reindeer_visit, &i);

#ifdef SLEEP_TIME
		sleep(SLEEP_TIME);
#endif

		pthread_create(&gnomes[i], &g_attr, gnome_visit, &i);

#ifdef SLEEP_TIME
		sleep(SLEEP_TIME);
#endif

		pthread_join(reindeers[i], NULL);
		pthread_join(gnomes[i], NULL);
	}

	// printf("* waiting for santa to finish...");
	pthread_join(t_santa, NULL);
	// printf(" done!\n");
}

int main()
{

	for (int i = 0; i < 10; i++)
	{
		projekt_zso();
	}

	// Wyczyszczenie pamięci
	pthread_mutex_destroy(&g_mutex);
	pthread_mutex_destroy(&r_mutex);

	pthread_cond_destroy(&r_cv);
	pthread_cond_destroy(&g_cv);
}