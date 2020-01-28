#include "stdio.h"
#include <sys/time.h>
#include <string.h>
#include <math.h>

/*Function that implements In-Memory Sequential Search*/
void mem_lin(const char *keyfile, const char *seekfile){
  struct timeval tvbef, tvaft;    /*timevals to store timeofday before and after
                                    an execution unit*/
  time_t exec_sec;                /*timeval to store execution time in seconds*/
  long int exec_usec;             /*timeval to store execution time in
                                    microseconds*/

  FILE *seeks;                    /*open file containing the seek list*/
  seeks = fopen(seekfile,"rb");

  int num_seeks;                  /*calculate num of seek values*/
  fseek(seeks, 0L, SEEK_END);
  num_seeks = ftell(seeks) / sizeof(int);
  fseek(seeks, 0L, SEEK_SET);

  int S[num_seeks];               /*store seek values in an array*/
  int i;
  for (i = 0; i < num_seeks; i++) {
    fread(&S[i], sizeof(int), 1, seeks);
    fseek(seeks, (i + 1) * sizeof(int), SEEK_SET);
  }
  fclose(seeks);

  FILE  *keys;                    //open file containing the keys and
  gettimeofday (&tvbef, NULL);    //calculate the execution time
  keys = fopen(keyfile,"rb");
  gettimeofday (&tvaft, NULL);
  exec_sec = tvaft.tv_sec - tvbef.tv_sec;
  exec_usec = tvaft.tv_usec - tvbef.tv_usec;

  int num_keys;                   //calculate num of keys
  fseek(keys, 0L, SEEK_END);
  num_keys = ftell(keys) / sizeof(int);
  fseek(keys, 0L, SEEK_SET);

  int K[num_keys];                  //store keys in an array and calculate the
  for (i = 0; i < num_keys; i++) {  //execution time
    gettimeofday (&tvbef, NULL);
    fread(&K[i], sizeof(int), 1, keys);
    fseek(keys, (i + 1) * sizeof(int), SEEK_SET);
    gettimeofday (&tvaft, NULL);
    exec_sec += tvaft.tv_sec - tvbef.tv_sec;
    exec_usec += tvaft.tv_usec - tvbef.tv_usec;
  }
  fclose(keys);

  int hit[num_seeks];               //declare array to store hits

  int j;                            //perform in-memory sequential search and
  int flag;                         //calculate execution time
  gettimeofday (&tvbef, NULL);
  for (i = 0; i < num_seeks; i++) {
    flag = 0;
    for (j = 0; j < num_keys; j++) {
      if (S[i] == K[j]) {
        flag = 1;
        break;
      }
    }
    hit[i] = flag;
  }
  gettimeofday (&tvaft, NULL);
  exec_sec += tvaft.tv_sec - tvbef.tv_sec;
  exec_usec += tvaft.tv_usec - tvbef.tv_usec;

  for (i = 0; i < num_seeks; i++) {   //print results
    if (hit[i] == 1) {
      printf( "%12d: Yes\n", S[i]);
    } else{
      printf( "%12d: No\n", S[i] );
    }
  }

  printf( "Time: %ld.%06ld", exec_sec, exec_usec );   //print execution time
}

/*Function that implements In-Memory Sequential Search*/
void mem_bin(const char *keyfile, const char *seekfile){
  struct timeval tvbef, tvaft;    /*timevals to store timeofday before and after
                                    an execution unit*/
  time_t exec_sec;                /*timeval to store execution time in seconds*/
  long int exec_usec;             /*timeval to store execution time in
                                    microseconds*/

  FILE *seeks;                    /*open file containing the seek list*/
  seeks = fopen(seekfile,"rb");

  int num_seeks;                  /*calculate num of seek values*/
  fseek(seeks, 0L, SEEK_END);
  num_seeks = ftell(seeks) / sizeof(int);
  fseek(seeks, 0L, SEEK_SET);

  int S[num_seeks];               /*store seek values in an array*/
  int i;
  for (i = 0; i < num_seeks; i++) {
    fread(&S[i], sizeof(int), 1, seeks);
    fseek(seeks, (i + 1) * sizeof(int), SEEK_SET);
  }
  fclose(seeks);

  FILE  *keys;                    //open file containing the keys and
  gettimeofday (&tvbef, NULL);    //calculate the execution time
  keys = fopen(keyfile,"rb");
  gettimeofday (&tvaft, NULL);
  exec_sec = tvaft.tv_sec - tvbef.tv_sec;
  exec_usec = tvaft.tv_usec - tvbef.tv_usec;

  int num_keys;                   //calculate num of keys
  fseek(keys, 0L, SEEK_END);
  num_keys = ftell(keys) / sizeof(int);
  fseek(keys, 0L, SEEK_SET);

  int K[num_keys];                  //store keys in an array and calculate the
  for (i = 0; i < num_keys; i++) {  //execution time
    gettimeofday (&tvbef, NULL);
    fread(&K[i], sizeof(int), 1, keys);
    fseek(keys, (i + 1) * sizeof(int), SEEK_SET);
    gettimeofday (&tvaft, NULL);
    exec_sec += tvaft.tv_sec - tvbef.tv_sec;
    exec_usec += tvaft.tv_usec - tvbef.tv_usec;
  }
  fclose(keys);

  int hit[num_seeks];               //declare array to store hits

  int flag;                         //Perform in-memory binary search and
  int l;                            //calculate execution time
  int r;
  int m;
  gettimeofday (&tvbef, NULL);
  for (i = 0; i < num_seeks; i++) {
    flag = 0;
    l = 0;
    r = num_keys;
    while (l <= r) {
      m = floor((l+r)/2);
      if (K[m] < S[i]) {
        l = m + 1;
      } else if (K[m] > S[i]){
        r = m - 1;
      } else{
        flag = 1;
        break;
      }
    }
    hit[i] = flag;
  }
  gettimeofday (&tvaft, NULL);
  exec_sec += tvaft.tv_sec - tvbef.tv_sec;
  exec_usec += tvaft.tv_usec - tvbef.tv_usec;

  for (i = 0; i < num_seeks; i++) {     //print results
    if (hit[i] == 1) {
      printf( "%12d: Yes\n", S[i]);
    } else{
      printf( "%12d: No\n", S[i] );
    }
  }

  printf( "Time: %ld.%06ld", exec_sec, exec_usec );   //print execution time
}

/*Function that implements On-Disk Sequential Search*/
void disk_lin(const char *keyfile, const char *seekfile){
  struct timeval tvbef, tvaft;    /*timevals to store timeofday before and after
                                    an execution unit*/
  time_t exec_sec;                /*timeval to store execution time in seconds*/
  long int exec_usec;             /*timeval to store execution time in
                                    microseconds*/

  FILE *seeks;                    /*open file containing the seek list*/
  seeks = fopen(seekfile,"rb");

  int num_seeks;                  /*calculate num of seek values*/
  fseek(seeks, 0L, SEEK_END);
  num_seeks = ftell(seeks) / sizeof(int);
  fseek(seeks, 0L, SEEK_SET);

  int S[num_seeks];               /*store seek values in an array*/
  int i;
  for (i = 0; i < num_seeks; i++) {
    fread(&S[i], sizeof(int), 1, seeks);
    fseek(seeks, (i + 1) * sizeof(int), SEEK_SET);
  }
  fclose(seeks);

  FILE  *keys;                    //open file containing the keys and
  gettimeofday (&tvbef, NULL);    //calculate the execution time
  keys = fopen(keyfile,"rb");
  gettimeofday (&tvaft, NULL);
  exec_sec = tvaft.tv_sec - tvbef.tv_sec;
  exec_usec = tvaft.tv_usec - tvbef.tv_usec;

  int num_keys;                   //calculate num of keys
  fseek(keys, 0L, SEEK_END);
  num_keys = ftell(keys) / sizeof(int);
  fseek(keys, 0L, SEEK_SET);

  int hit[num_seeks];             //Declare array to store hits

  int j;                          //Perform on-disk sequential search and
  int flag;                       //calculate the execution time
  int k;
  gettimeofday (&tvbef, NULL);
  for (i = 0; i < num_seeks; i++) {
    flag = 0;
    for (j = 0; j < num_keys; j++) {
      fread(&k, sizeof(int), 1, keys);
      fseek(keys, (j + 1) * sizeof(int), SEEK_SET);
      if (S[i] == k) {
        flag = 1;
        break;
      }
    }
    hit[i] = flag;
  }
  gettimeofday (&tvaft, NULL);
  exec_sec += tvaft.tv_sec - tvbef.tv_sec;
  exec_usec += tvaft.tv_usec - tvbef.tv_usec;

  fclose(keys);

  for (i = 0; i < num_seeks; i++) {   //print results
    if (hit[i] == 1) {
      printf( "%12d: Yes\n", S[i]);
    } else{
      printf( "%12d: No\n", S[i] );
    }
  }

  printf( "Time: %ld.%06ld", exec_sec, exec_usec );   //print execution time
}

/*Function that implements On-Disk Binary Search*/
void disk_bin(const char *keyfile, const char *seekfile){
  struct timeval tvbef, tvaft;    /*timevals to store timeofday before and after
                                    an execution unit*/
  time_t exec_sec;                /*timeval to store execution time in seconds*/
  long int exec_usec;             /*timeval to store execution time in
                                    microseconds*/

  FILE *seeks;                    /*open file containing the seek list*/
  seeks = fopen(seekfile,"rb");

  int num_seeks;                  /*calculate num of seek values*/
  fseek(seeks, 0L, SEEK_END);
  num_seeks = ftell(seeks) / sizeof(int);
  fseek(seeks, 0L, SEEK_SET);

  int S[num_seeks];               /*store seek values in an array*/
  int i;
  for (i = 0; i < num_seeks; i++) {
    fread(&S[i], sizeof(int), 1, seeks);
    fseek(seeks, (i + 1) * sizeof(int), SEEK_SET);
  }
  fclose(seeks);

  FILE  *keys;                    //open file containing the keys and
  gettimeofday (&tvbef, NULL);    //calculate the execution time
  keys = fopen(keyfile,"rb");
  gettimeofday (&tvaft, NULL);
  exec_sec = tvaft.tv_sec - tvbef.tv_sec;
  exec_usec = tvaft.tv_usec - tvbef.tv_usec;

  int num_keys;                   //calculate num of keys
  fseek(keys, 0L, SEEK_END);
  num_keys = ftell(keys) / sizeof(int);
  fseek(keys, 0L, SEEK_SET);

  int hit[num_seeks];             //Declare array to store hits

  int flag;                       //Perform on-disk binary search and
  int l;                          //calculate execution time
  int r;
  int m;
  int k;
  gettimeofday (&tvbef, NULL);
  for (i = 0; i < num_seeks; i++) {
    flag = 0;
    l = 0;
    r = num_keys;
    while (l <= r) {
      m = floor((l+r)/2);
      fseek(keys, m * sizeof(int), SEEK_SET);
      fread(&k, sizeof(int), 1, keys);
      if (k < S[i]) {
        l = m + 1;
      } else if (k > S[i]){
        r = m - 1;
      } else{
        flag = 1;
        break;
      }
    }
    hit[i] = flag;
  }
  gettimeofday (&tvaft, NULL);
  exec_sec += tvaft.tv_sec - tvbef.tv_sec;
  exec_usec += tvaft.tv_usec - tvbef.tv_usec;

  fclose(keys);

  for (i = 0; i < num_seeks; i++) {   //print results
    if (hit[i] == 1) {
      printf( "%12d: Yes\n", S[i]);
    } else{
      printf( "%12d: No\n", S[i] );
    }
  }

  printf( "Time: %ld.%06ld", exec_sec, exec_usec );   //print execution time
}

int main(int argc, char const *argv[]) {
  if (strcmp(argv[1], "--mem-lin") == 0) {
      mem_lin(argv[2], argv[3]);
  } else if (strcmp(argv[1], "--mem-bin") == 0) {
    mem_bin(argv[2], argv[3]);
  } else if (strcmp(argv[1], "--disk-lin") == 0) {
    disk_lin(argv[2], argv[3]);
  } else if (strcmp(argv[1], "--disk-bin") == 0) {
    disk_bin(argv[2], argv[3]);
  } else {
    printf("Invalid Arguments\n");
  }
  return 0;
}
