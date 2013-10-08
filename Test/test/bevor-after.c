#include "../src/test.h"

BEVOR_ALL() {
  
  char *my_string = malloc(64);
  strcpy(my_string, "my test string");

  return (void *) my_string;
}

BEVOR() {
  
  char *my_string = (char *) args;

  char *new_string = malloc(128);

  sprintf(new_string, "%s new string", my_string);

  return (void *) new_string;  
}

TEST(bevor_after_test) {
  
  ASSERT_FALSE(strcmp((char *) args, "my test string new string"),
               "test args from bevor");

  return args;
}

TEST(bevor_after_test2) {
  
  ASSERT_FALSE(strcmp((char *) args, "my test string new string"),
               "test args from bevor");

  return args;
}

AFTER() {
  
  free((char *) args);
}

AFTER_ALL() {

  free((char *) args);
}
