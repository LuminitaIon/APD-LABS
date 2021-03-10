#include <bits/stdc++.h>
#include "mpi.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define MASTER 0
#define HORROR 1
#define COMEDY 2
#define FANTASY 3
#define SF  4

int P =  sysconf(_SC_NPROCESSORS_CONF);
int counter = 0;
pthread_mutex_t mutex_;
ofstream out_file;

unordered_map<int, pair<string, string>> my_map;
struct master_arg {
    int id;
    string text;
    string file_name;
};

struct worker_arg {
    int start;
    int end;
    int type;
    int id;
    vector<string> *lines;
} ;

void *workes_function(void *arg)
{
    struct worker_arg *params = (struct worker_arg*)arg;
    for (int i = params->start; i < params->end; i += 1) {  
        string result = "";
        if (params->type == HORROR) {
            for (int j = 0; j < (*params->lines)[i].size(); j += 1) {
                if ((*params->lines)[i][j] != 'a' && (*params->lines)[i][j] != 'e' &&
                    (*params->lines)[i][j] != 'i' && (*params->lines)[i][j] != 'o' &&
                    (*params->lines)[i][j] != 'u' && (*params->lines)[i][j] != 'A' &&
                    (*params->lines)[i][j] != 'E' && (*params->lines)[i][j] != 'I' &&
                    (*params->lines)[i][j] != 'O' && (*params->lines)[i][j] != 'U' && 
                    (*params->lines)[i][j] != ' ' && isalpha((*params->lines)[i][j])) {
                    
                    result += (*params->lines)[i][j];
                    result += tolower((*params->lines)[i][j]);
                } else {
                    result += (*params->lines)[i][j];
                }
            }
            (*params->lines)[i] = result; 
        } else if (params->type == COMEDY) {
            int pos = 1;
            for (int j = 0; j < (*params->lines)[i].size(); j += 1) {
                if (pos % 2 == 0) {
                    (*params->lines)[i][j] = toupper((*params->lines)[i][j]);
                }
                pos ++;
                if ((*params->lines)[i][j] == ' ' || (*params->lines)[i][j] == '\n') {
                    pos = 1;
                }
            }        
        } else if (params->type == FANTASY) {
            (*params->lines)[i][0] = toupper((*params->lines)[i][0]);
            for (int j = 1; j < (*params->lines)[i].size(); j += 1) {
                if ((*params->lines)[i][j - 1] == '\n' || (*params->lines)[i][j - 1] == ' ') {
                    (*params->lines)[i][j] = toupper((*params->lines)[i][j]);
                }
            }
        } else if (params->type == SF) {
            int count = 1;
            for (int j = 0; j < (*params->lines)[i].size(); j += 1) {
                if ((*params->lines)[i][j] == ' ')
                    count ++;
                if ((*params->lines)[i][j] == '\n')
                    count = 1;

                if (count % 7 == 0) {
                    string aux;
                    j++;
                    while ((*params->lines)[i][j] != ' ' && (*params->lines)[i][j] != '\n') {
                        aux += (*params->lines)[i][j];
                        j++;
                    }
                    reverse(aux.begin(), aux.end());
                    if ((*params->lines)[i][j] == '\n') {
                        result += " " + aux + '\n';
                    } else {
                        result += " " + aux + " ";
                    }
                    count ++;
                } else {
                    result += (*params->lines)[i][j];
                }
            }
            (*params->lines)[i] = result;
        }
    }

    pthread_exit(NULL);    
}

void *master_function(void *arg)
{
    struct master_arg params = *(struct master_arg*)arg;
    ifstream input_file(params.file_name);
    string line;
    int id = 0;
    while(getline(input_file, line) && !input_file.eof()) {
        string paragraph;
        string title = line;
        while (!line.empty() && !input_file.eof()) {
            getline(input_file, line);
            paragraph += line + "\n";   
        }

        if (title == "horror" && params.id == HORROR) {
            int length = paragraph.length();
            MPI_Send(&id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&length, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(paragraph.c_str(), length, MPI_CHAR, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&params.id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);

            MPI_Status status;
            int recv_id;
            int recv_len;
            char* recv_paragraph;
            MPI_Recv(&recv_id, 1, MPI_INT, HORROR, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&recv_len, 1, MPI_INT, HORROR, 0, MPI_COMM_WORLD, &status);
            recv_paragraph = new char[recv_len + 1];
            MPI_Recv(recv_paragraph, recv_len, MPI_CHAR, HORROR, 0, MPI_COMM_WORLD, &status);
            recv_paragraph[recv_len] = '\0';
            string final_paragraph(recv_paragraph);
            delete(recv_paragraph);

            int rank;
            string recv_title = "";
            MPI_Recv(&rank, 1, MPI_INT, HORROR, 0, MPI_COMM_WORLD, &status);
            pthread_mutex_lock(&mutex_);

            if (rank == HORROR)
                recv_title = "horror";
            else if (rank == COMEDY)
                recv_title = "comedy";
            else if (rank == FANTASY)
                recv_title = "fantasy";
            else if (rank == SF)
                recv_title = "science-fiction";

            my_map.insert(make_pair(recv_id, make_pair(final_paragraph, recv_title)));
            while (my_map.find(counter) != my_map.end()) {
                out_file << my_map[counter].second << endl;
                out_file << my_map[counter].first;
                my_map.erase(counter);
                counter++;
            }
            pthread_mutex_unlock(&mutex_);
        } else if (title == "comedy" && params.id == COMEDY) {
            int length = paragraph.length();
            MPI_Send(&id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&length, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(paragraph.c_str(), length, MPI_CHAR, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&params.id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);

            MPI_Status status;
            int recv_id;
            int recv_len;
            char* recv_paragraph;
            MPI_Recv(&recv_id, 1, MPI_INT, COMEDY, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&recv_len, 1, MPI_INT, COMEDY, 0, MPI_COMM_WORLD, &status);
            recv_paragraph = new char[recv_len + 1];
            MPI_Recv(recv_paragraph, recv_len, MPI_CHAR, COMEDY, 0, MPI_COMM_WORLD, &status);
            recv_paragraph[recv_len] = '\0';
            string final_paragraph(recv_paragraph);
            delete(recv_paragraph);

            int rank;
            string recv_title = "";
            MPI_Recv(&rank, 1, MPI_INT, COMEDY, 0, MPI_COMM_WORLD, &status);
            pthread_mutex_lock(&mutex_);

            if (rank == HORROR)
                recv_title = "horror";
            else if (rank == COMEDY)
                recv_title = "comedy";
            else if (rank == FANTASY)
                recv_title = "fantasy";
            else if (rank == SF)
                recv_title = "science-fiction";

            my_map.insert(make_pair(recv_id, make_pair(final_paragraph, recv_title)));
            while (my_map.find(counter) != my_map.end()) {
                out_file << my_map[counter].second << endl;
                out_file << my_map[counter].first;
                my_map.erase(counter);
                counter++;
            }
            pthread_mutex_unlock(&mutex_);
        } else if (title == "fantasy" && params.id == FANTASY) {
            int length = paragraph.length();
            MPI_Send(&id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&length, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(paragraph.c_str(), length, MPI_CHAR, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&params.id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);

            MPI_Status status;
            int recv_id;
            int recv_len;
            char* recv_paragraph;
            MPI_Recv(&recv_id, 1, MPI_INT, FANTASY, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&recv_len, 1, MPI_INT, FANTASY, 0, MPI_COMM_WORLD, &status);
            recv_paragraph = new char[recv_len + 1];
            MPI_Recv(recv_paragraph, recv_len, MPI_CHAR, FANTASY, 0, MPI_COMM_WORLD, &status);
            recv_paragraph[recv_len] = '\0';
            string final_paragraph(recv_paragraph);
            delete(recv_paragraph);

            int rank;
            string recv_title = "";
            MPI_Recv(&rank, 1, MPI_INT, FANTASY, 0, MPI_COMM_WORLD, &status);
            pthread_mutex_lock(&mutex_);

            if (rank == HORROR)
                recv_title = "horror";
            else if (rank == COMEDY)
                recv_title = "comedy";
            else if (rank == FANTASY)
                recv_title = "fantasy";
            else if (rank == SF)
                recv_title = "science-fiction";

            my_map.insert(make_pair(recv_id, make_pair(final_paragraph, recv_title)));
            while (my_map.find(counter) != my_map.end()) {
                out_file << my_map[counter].second << endl;
                out_file << my_map[counter].first;
                my_map.erase(counter);
                counter++;
            }
            pthread_mutex_unlock(&mutex_);
        } else if (title == "science-fiction" && params.id == SF) {
            int length = paragraph.length();
            MPI_Send(&id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&length, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(paragraph.c_str(), length, MPI_CHAR, params.id, 0, MPI_COMM_WORLD);
            MPI_Send(&params.id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);

            MPI_Status status;
            int recv_id;
            int recv_len;
            char* recv_paragraph;
            MPI_Recv(&recv_id, 1, MPI_INT, SF, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&recv_len, 1, MPI_INT, SF, 0, MPI_COMM_WORLD, &status);
            recv_paragraph = new char[recv_len + 1];
            MPI_Recv(recv_paragraph, recv_len, MPI_CHAR, SF, 0, MPI_COMM_WORLD, &status);
            recv_paragraph[recv_len] = '\0';
            string final_paragraph(recv_paragraph);
            delete(recv_paragraph);

            int rank;
            string recv_title = "";
            MPI_Recv(&rank, 1, MPI_INT, SF, 0, MPI_COMM_WORLD, &status);
            pthread_mutex_lock(&mutex_);

            if (rank == HORROR)
                recv_title = "horror";
            else if (rank == COMEDY)
                recv_title = "comedy";
            else if (rank == FANTASY)
                recv_title = "fantasy";
            else if (rank == SF)
                recv_title = "science-fiction";

            my_map.insert(make_pair(recv_id, make_pair(final_paragraph, recv_title)));
            while (my_map.find(counter) != my_map.end()) {
                out_file << my_map[counter].second << endl;
                out_file << my_map[counter].first;
                my_map.erase(counter);
                counter++;
            }
            pthread_mutex_unlock(&mutex_);
        }
        id++;
    }

    if (input_file.eof()) {
        id = -1;
        MPI_Send(&id, 1, MPI_INT, params.id, 0, MPI_COMM_WORLD);
    }
    input_file.close();
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int rank, nr_proc, provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &nr_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    pthread_mutex_init(&mutex_, NULL);    

    string file_name(argv[1]);
    string out_ = "./tests/in/" + file_name.substr(11, 6) + ".out";
    out_file.open(out_);
    if (rank == MASTER) {
        pthread_t tid[4];
        vector<struct master_arg> params(4);

        for (int i = 0; i < 4; i += 1) {
            params[i].id = i + 1;
            params[i].file_name = file_name;
            pthread_create(&tid[i], NULL, master_function, &params[i]);
        }

        for (int i = 0; i < 4; i += 1) {
            pthread_join(tid[i], NULL);
        }
    } else {
        int length;
        int id;
        int type;
        MPI_Status status;
        char *paragraph;

        while (true) {
            MPI_Recv(&id, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
            if (id == -1) break;
            MPI_Recv(&length, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
            paragraph = new char[length + 1];
            MPI_Recv(paragraph, length, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, &status);
            paragraph[length] = '\0';
            MPI_Recv(&type, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);

            //break paragraph
            vector<string> lines;
            int count = 0;
            string line, chunk;
            stringstream tokens(paragraph);
            while(true) {
                getline(tokens, line, '\n');
                if(tokens.eof()) {
                    chunk += line;
                    lines.push_back(chunk);
                    break;
                } else {
                    count++;
                    chunk += line + "\n";
                    if (count == 20) {
                        lines.push_back(chunk);
                        chunk = "";
                        count = 0;
                    }
                }
            }
            int number_of_threads = min((int)lines.size(), P - 1);
            pthread_t tid[number_of_threads];
            vector<struct worker_arg> params(number_of_threads);
            for (int i = 0; i < number_of_threads; i +=1 ) {
                int start = i * (double)lines.size() / number_of_threads;
                int end = min((i + 1) * lines.size() / number_of_threads, lines.size());
                params[i].end = end;
                params[i].start = start;
                params[i].type = type;
                params[i].lines = &lines;
                pthread_create(&tid[i], NULL, workes_function, &params[i]);
            }

            for (int i = 0; i < number_of_threads; i +=1 ) {
                pthread_join(tid[i], NULL);
            }
            delete paragraph;

            string final_paragraph = "";
            for (auto x : lines) {
                final_paragraph += x;
            }

            int len = final_paragraph.size();
            MPI_Send(&id, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(&len, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(final_paragraph.c_str(), len, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(&rank, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);

        }
    }
    out_file.close();
    pthread_mutex_destroy(&mutex_);
    MPI_Finalize();
    return 0;
}