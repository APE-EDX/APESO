#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
//#include "socket.hpp"

#include <duktape.h>
#include <apecore.hpp>



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



void hello()
{
	printf("I just got loaded\n");

  duk_context *ctx = duk_create_heap_default();
  duk_eval_string(ctx, "print('Hello world!');");
  duk_destroy_heap(ctx);
    
}


//EntryPoint
__attribute__((constructor))
void loadMsg()
{


   if( make_terminal() != 0 ) {
        fprintf( stderr, "Could not create terminal!\n" );
    } 

	hello();

}