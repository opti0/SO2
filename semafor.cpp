#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
using namespace std;

class Semafor 
{
    private:
        //integer z dostępem atomowym
        //dostęp atomowy = operacje synchronizowane przez system operacyjny
        atomic<bool> token;

    public:
        //semafor z zadaną liczbą żetonów
        //jeden żeton = jeden proces z dostępem do sekcji krytycznej
        Semafor(bool token = false) : token(token)
        {}

        //get żeton
        void get() {
            //uzyskaj czy mamy teraz żeton
            bool old_token = token.load();
            //jeśli jest false (brak żetonu)
            //lub nie jesteś w stanie zmienić go na false
            //uwaga: ta funkcja zmieni liczbę żetonów tylko jeśli jest ich wciąż "stare_zetony"
            //czyli inny proces nie pracuje na nich w tej chwili
            while (old_token == false ||
                   !token.compare_exchange_strong(
                           old_token,
                           false))
            {
                //odświeżamy stan żetonu
                old_token = token.load();
            }
        }

        //release żeton
        void release() {
            token.store(true);
        }
};

//liczba wątków
const int k = 10;
//liczba do której wątki liczą
const int n = 10000;

//semafor do synchronizacji
Semafor sem(0);
//zmienna do której wątki będą sumowały wartość.
volatile int suma;

//watek sumujący bez zabezpieczen
void niebezpieczny_sumator()
{
    for (int i = 0; i < n; i++)
        suma++;
}

//watek sumujący za pomocą semafora
void bezpieczny_sumator()
{
    for (int i = 0; i < n; i++)
    {
        sem.get();
        suma++;
        sem.release();
    }
}

volatile int magazyn = 0;
void producent()
{
    int i = 100;
    while (i > 0)
    {
        sem.get();
        magazyn++;
        i--;
        stringstream s;
        s << "Watek " << this_thread::get_id() << " produkuje (" << magazyn << ")\n";
        cout << s.str();
        sem.release();
    }
}
void konsument()
{
    int i = 100;
    while (i > 0)
    {
        sem.get();
        if (magazyn == 0)
        {
            stringstream s;
            s << "Watek " << this_thread::get_id() << " nie ma co konsumowac (" << magazyn << ")\n";
            cout << s.str();
        }
        else
        {
            magazyn--;
            i--;
            stringstream s;
            s << "Watek " << this_thread::get_id() << " konsumuje (" << magazyn << ")\n";
            cout << s.str();
        }
        sem.release();
    }
}
/*
int main()
{
    {
        cout << "Test bez zabezpieczenia, sumowanie " << k << " x " << n << " = " << k * n << endl;
        suma = 0;
        std::thread watki[k];
        for (int i = 0; i < k; i++) {
            watki[i] = thread(niebezpieczny_sumator);
        }
        sem.zwolnij();
        for (int i = 0; i < k; i++) {
            watki[i].join();
        }
        cout << "Uzyskana suma: " << suma << endl;
        system("pause");
    }
    {
        cout << "Test z semaforem, sumowanie " << k << " x " << n << " = " << k * n << endl;
        suma = 0;
        std::thread watki[k];
        for (int i = 0; i < k; i++) {
            watki[i] = thread(bezpieczny_sumator);
        }
        sem.zwolnij();
        for (int i = 0; i < k; i++) {
            watki[i].join();
        }
        cout << "Uzyskana suma: " << suma << endl;
        system("pause");
    }
    {
        cout << "Test producent/konsument" << endl;
        std::thread producenci[k];
        std::thread konsumenci[k];
        for (int i = 0; i < k; i++) {
            producenci[i] = thread(producent);
        }
        sem.release();
        for (int i = 0; i < k; i++) {
            konsumenci[i] = thread(konsument);
        }
        for (int i = 0; i < k; i++) {
            producenci[i].join();
            konsumenci[i].join();
        }
        cout << "Magazyn: " << magazyn << endl;
        system("pause");
    }
    return 0;
}
 */