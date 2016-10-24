#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // threads
#include <dlfcn.h> // getLibraryPath
#include <iostream>

#include <duktape.h>
#include <apecore.hpp>

#include <fstream>
#include <string>


using namespace std;

namespace ape
{
    namespace platform
    {
        void sleep(uint32_t ms)
        {
            usleep(ms * 1000);
        }

        bool createThread(ThreadFunction function, void* parameter) //Adapted to linux
        {
            printf("[?] Creando Thread..");
            pthread_t tid;
            pthread_create(&(tid), NULL, function, NULL);

            printf("[?] Thread Creado...?");

            return true;
        }

        size_t getLibraryPath(char* buffer, size_t size)
        {
            Dl_info info;
            dladdr(__builtin_return_address(0), &info);
            printf("%s\n", info.dli_fname);

            int len = strlen(info.dli_fname);
            int mlen = (len > size) ? size : len;
            strncpy(buffer, info.dli_fname, mlen);

            return mlen;
        }

        void* getLibraryOEP() // get the entrypoint of the library
        {
            Dl_info info;
            dladdr(__builtin_return_address(0), &info);
            printf("%X\n", info.dli_fbase);
            return info.dli_fbase;
        }

        typedef struct
        {
            int long start_addres;
            char trash;
            int long end_addres;
            char prev[4];
            int long trash2;

            unsigned number;
            char dubleP;
            unsigned number2;

            unsigned number3;


            string name;
        } proc;



        uint32_t getProcessSize()
        {
            uint32_t resultado;
            ifstream fMaps;
            fMaps.open("/proc/self/maps");

            if(fMaps.is_open())
            {
                cout << "archivo abierto correctamnete" << endl;
                proc proceso[500];
                string line;

                /*
                while(getline(fMaps,line))
                {
                    cout << line << endl;
                }
                */

                int i = 0;
                while (fMaps >> hex >> proceso[i].start_addres)
                {
                    fMaps >> proceso[i].trash;
                    fMaps >> hex >> proceso[i].end_addres;
                    fMaps >> proceso[i].prev;
                    fMaps >> proceso[i].trash2;
                    fMaps >> proceso[i].number;
                    fMaps >> proceso[i].dubleP;
                    fMaps >> proceso[i].number2;
                    fMaps >> proceso[i].number3;

                    getline(fMaps,proceso[i].name);

                    cout << hex << proceso[i].start_addres << endl;
                    cout << hex << proceso[i].end_addres << endl;
                    cout << proceso[i].name << endl;
                    //cout << line << endl;
                    ++i;
                }

                fMaps.close();

                i = 0;
                unsigned long first = proceso[0].start_addres;
                unsigned long second = proceso[0].start_addres;

                while (proceso[i].name == proceso[i+1].name)
                {
                    second = proceso[i+1].end_addres;
                    ++i;
                }

                cout << "[FIRST]" << first << endl;
                cout << "[SECOND]" << second << endl;

                cout << "[Resultado] " << hex << second-first << endl;
                resultado = second-first;

            }
            else
            {
                cout << "fallo al abrir el archivo" << endl;
            }

            return resultado;
        }
    }
}
