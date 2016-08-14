#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // threads
#include <dlfcn.h> // getLibraryPath

//#include "socket.hpp"

#include <iostream>

#include <duktape.h>
#include <apecore.hpp>

#include <fstream>
#include <string>

using namespace std;

int make_terminal() {
    char  pidarg[256]; // the '--pid=' argument of tail
    pid_t child;       // the pid of the child proc
    pid_t parent;      // the pid of the parent proc
    FILE* fp;          // file to which output is redirected
    int   fn;          // file no of fp

    // Open file for redirection
    fp = fopen("/tmp/asdf.log","w");
    fn = fileno(fp);

    // Get pid of current process and create string with argument for tail
    parent = getpid();
    sprintf( pidarg, "--pid=%d", parent );

    // Create child process
    child = fork();
    if( child == 0 ) {
        // CHILD PROCESS

        // Replace child process with a gnome-terminal running:
        //      tail -f /tmp/asdf.log --pid=<parent_pid>
        // This prints the lines outputed in asdf.log and exits when
        // the parent process dies.
        execlp( "mate-terminal", "mate-terminal", "-x", "tail","-f","/tmp/asdf.log", pidarg, NULL );

        // if there's an error, print out the message and exit
        perror("execlp()");
        exit( -1 );
    } else {
        // PARENT PROCESS
        close(1);      // close stdout
        int ok = dup2( fn, 1 ); // replace stdout with the file

        if( ok != 1 ) {
            perror("dup2()");
            return -1;
        }

        // Make stdout flush on newline, doesn't happen by default
        // since stdout is actually a file at this point.
        setvbuf( stdout, NULL, _IONBF, BUFSIZ );
    }

    return 0;
}



void hello(){
	printf("[!] Modulo inyectado\n");
}


bool createThread(ThreadFunction function, void* parameter) //Adapted to linux
{
  printf("[?] Creando Thread..");
	pthread_t tid;
  pthread_create(&(tid), NULL, function, NULL);

  printf("[?] Thread Creado...?");

  return(true);
}



size_t getLibraryPath(char* buffer, size_t size){
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




duk_ret_t WrapCreateConsole(duk_context *ctx)
{
    duk_push_boolean(ctx, make_terminal());
    return 1;  /* one return value */
}




typedef struct{
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
}proc;



uint32_t getProcessSize()
{
  uint32_t resultado;
  ifstream fMaps;
  fMaps.open("/proc/self/maps");

  	if(fMaps.is_open()){

  		cout << "archivo abierto correctamnete" << endl;

  		proc proceso[500];

  		string line;

  /*
  		while(getline(fMaps,line)){
  			cout << line << endl;
  		}

  */

  int i = 0;
  		while(fMaps >> hex >> proceso[i].start_addres){
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
  			i++;
  		}

  		fMaps.close();
  		i = 0;
  		unsigned long first = proceso[0].start_addres;
  		unsigned long second = proceso[0].start_addres;
  		while(proceso[i].name == proceso[i+1].name){

  			second = proceso[i+1].end_addres;
  			i++;
  		};

  		cout << "[FIRST]" << first << endl;
  		cout << "[SECOND]" << second << endl;

  		cout << "[Resultado] " << hex << second-first << endl;
      resultado = second-first;

  	}else{
  		cout << "fallo al abrir el archivo" << endl;
  	}

  return (resultado);
}







//EntryPoint
__attribute__((constructor))
void loadMsg()
{
  hello();


  duk_context* ctx = apecore_initialize([](duk_context* ctx) -> void {

  			// Add CreateConsole
  			duk_push_c_function(ctx, WrapCreateConsole, DUK_VARARGS);
  			duk_put_global_string(ctx, "CreateConsole");

  });




/*
   if( make_terminal() != 0 ) {
        fprintf( stderr, "Could not create terminal!\n" );
    }


  duk_context *ctx = duk_create_heap_default();
  duk_eval_string(ctx, "print('Js is WORKING!');");
  duk_destroy_heap(ctx);

  printf("[?] Obteniendo ruta...");
  identify_function_ptr();
  printf ("[?] Obteniendo EntryPoint de la libreria" );
  getEntryPoint();

  //dl_iterate_phdr SIZE OF DLL
  //getrusage       Size of PROC
  // VIRTUALQUERY http://stackoverflow.com/questions/269314/is-there-a-better-way-than-parsing-proc-self-maps-to-figure-out-memory-protecti
*/
}
