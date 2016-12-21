#include <dlfcn.h>
#include "randcpuid.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


/* Main program, which outputs N bytes of random data.  */
int main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      exit(1);
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  void* dl_handle;
  char* error;
  if (rdrand_supported ())
  {
    dl_handle = dlopen("randlibhw.so", RTLD_LAZY);
    if (!dl_handle){
        fprintf(stderr, "dlopen() Error with randlibhw.so %s\n", dlerror());
        exit(1);
    }

    initialize = dlsym (dl_handle, "rand64_init");
    error = dlerror();
    if (error!= NULL)
    {
      fprintf(stderr, "Error with opening rand64_init for hardware %s\n", error);
      exit(1);
    }
    //initialize = hardware_rand64_init;

    rand64 = dlsym (dl_handle, "rand64");
    error = dlerror();
    if (error!= NULL)
    {
      fprintf(stderr, "Error with opening rand64 for hardware %s\n", error);
      exit(1);
    }
    //rand64 = hardware_rand64;

    finalize = dlsym (dl_handle, "rand64_fini");
    error = dlerror();
    if (error!= NULL)
    {
      fprintf(stderr, "Error with opening rand64_fini for hardware %s\n", error);
      exit(1);
    }
    //finalize = hardware_rand64_fini;
    fprintf(stdout, "HW\n");
  }
  else
  {
    dl_handle = dlopen("randlibsw.so", RTLD_LAZY);
    if (!dl_handle){
        fprintf(stderr, "dlopen() Error with randlibhw.so %s\n", dlerror());
        exit(1);
    }

    // initialize = dlsym (dl_handle, "rand64_init");
    // error = dlerror();
    // if (error!= NULL)
    // {
    //   fprintf(stderr, "Error with opening rand64_init for software %s\n", error);
    //   exit(1);
    // }

    rand64 = dlsym (dl_handle, "rand64");
    error = dlerror();
    if (error!= NULL)
    {
      fprintf(stderr, "Error with opening rand64 for software %s\n", error);
      exit(1);
    }

    // finalize = dlsym (dl_handle, "rand64_fini");
    // error = dlerror();
    // if (error!= NULL)
    // {
    //   fprintf(stderr, "Error with opening rand64_fini for software %s\n", error);
    //   exit(1);
    // }
    fprintf(stdout, "SW\n");
  }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  int ceiling = nbytes%wordsize;
  if (ceiling != 0)
    nbytes = nbytes + (wordsize - ceiling);

  //nbytes = nbytes + (wordsize - (nbytes%wordsize));
  do
  {
    unsigned long long x = rand64 ();
    fprintf(stdout, "%llu\n", x);
    nbytes -= wordsize;
   //  size_t outbytes = nbytes < wordsize ? nbytes : wordsize;

   //  if (fwrite (&x, 1, outbytes, stdout) != outbytes)
  	// {
  	//   output_errno = errno;
  	//   break;
  	// }
   //  nbytes -= outbytes;
  }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
  {
    errno = output_errno;
    perror ("output");
    finalize ();
    exit(1);
  }

  dlclose(dl_handle);
  error = dlerror();
  if (error != NULL)
  {
    fprintf(stderr, "Error closing the dl handle%s\n", error);
    exit(1);
  }
  exit(0);
}