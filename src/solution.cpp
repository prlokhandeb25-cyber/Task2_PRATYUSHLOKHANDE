#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

class Task 
{
public:
    string id;
    int burst;
    int remaining;
    vector<string> mem_requests;
    int current_mem_index;

    Task() 
    {
        current_mem_index = 0;
    }

    string getMemoryRequest() 
    {
        if (mem_requests.empty())
            return "";

        return mem_requests[current_mem_index % mem_requests.size()];
    }
};

class Cache 
{
private:
    string name;
    int capacity;
    int latency;
    vector<string> blocks;

public:
    Cache(string n, int c, int l)
        : name(n), capacity(c), latency(l) {}

    bool contains(string block) 
    {
        return find(blocks.begin(), blocks.end(), block)
               != blocks.end();
    }

    int getLatency() 
    {
        return latency;
    }

    void remove(string block) 
    {
        auto it = find(blocks.begin(), blocks.end(), block);
        if (it != blocks.end())
            blocks.erase(it);
    }

    string insert(string block) 
    {
        if (contains(block))
            return "";

        string evicted = "";

        if (blocks.size() >= capacity) 
        {

            evicted = blocks.front();
            blocks.erase(blocks.begin());
        }

        blocks.push_back(block);
        return evicted;
    }

    void print() 
    {
        cout << name << ": [";
        for (int i = 0; i < blocks.size(); i++) 
        {
            cout << blocks[i];
            if (i != blocks.size() - 1)
                cout << ", ";
        }
        cout << "]";
    }
};

class MemorySystem 
{
private:
    Cache L1;
    Cache L2;
    Cache L3;

    void promoteToL1(string block) 
    {
        string e1 = L1.insert(block);

        if (!e1.empty()) 
        {
            string e2 = L2.insert(e1);

            if (!e2.empty()) {

                L3.insert(e2);
            }
        }
    }

public:
    int ram_access;
 
    MemorySystem()
        : L1("L1",32,4),
          L2("L2",128,12),
          L3("L3",512,40)
    {
        ram_access = 0;
    }

    int access(string block) 
    {
        if (L1.contains(block)) 
        {
            cout << " -> HIT L1";
            return L1.getLatency();
        }
        cout << " -> MISS L1";

        if (L2.contains(block)) 
        {
            cout << " -> HIT L2";
            L2.remove(block);
            promoteToL1(block);
            return L2.getLatency();
        }

        cout << " -> MISS L2";

        if (L3.contains(block)) 
        {
            cout << " -> HIT L3";
            L3.remove(block);
            promoteToL1(block);
            return L3.getLatency();
        }

        cout << " -> MISS L3";
        cout << " -> Fetching RAM";

        ram_access++;
        promoteToL1(block);
        return 200;
    }

    void printState() 
    {
        cout << "\n";
        L1.print();
        cout << "\n";
        L2.print();
        cout << "\n";
        L3.print();
        cout << "\n";
    }

};

class Simulator 
{
private:
    vector<Task> tasks;
    MemorySystem memory;
    int quantum;
    long long totalCycles;
public:
    Simulator(int q) 
    {
        quantum = q;
        totalCycles = 0;
    }

    void loadTask(string filename) 
    {

        ifstream file(filename);

        if (!file.is_open()) 
        {
            cout << "File not found\n";
            return;
        }
        string line;

        while (getline(file,line)) 
        {
            if (line.empty())
                continue;

            stringstream ss(line);

            string token;
            Task t;

            ss >> token;
            ss >> t.id;

            ss >> token;
            ss >> t.burst;

            ss >> token;
            t.remaining = t.burst;

            string block;

            while (ss >> block) 
            {
                t.mem_requests.push_back(block);
            }

            tasks.push_back(t);
        }
    }

    void roundRobin() 
    {
        queue<int> ready;
        for (int i = 0; i < tasks.size(); i++)
            ready.push(i);

        int completed = 0;

        int cycle = 1;
        while (!ready.empty()) 
        {
            int index = ready.front();
            ready.pop();
            Task &current = tasks[index];

            int run = min(quantum,
                          current.remaining);

            for (int i = 0; i < run; i++) 
            {
                string request =
                current.getMemoryRequest();

                cout << "\nCycle "<<cycle<<" Running: "<<current.id<<" Requesting: "<<request;
                int latency =
                memory.access(request);

                cout << "\nLatency: "
                     << latency
                     << " cycles\n";

                totalCycles += latency;
                memory.printState();
                current.current_mem_index++;
                current.remaining--;
                cycle++;
            }
            if (current.remaining > 0) 
            {
                ready.push(index);
            }
            else 
            {
                completed++;
                cout << "\nTask "
                     << current.id
                     << " completed\n";
            }
        }
        cout << "\n=== Final Results ===" << "Total Cycles: " << totalCycles<< " Tasks Completed: " << completed<< " Scheduler: Round Robin (quantum =" <<quantum<<")"<< " RAM Accesses: " << memory.ram_access << endl;
    }
};

int main() 
{
    Simulator sim(3);
    sim.loadTask("input_task2.txt");
    sim.roundRobin();
    return 0;
}
