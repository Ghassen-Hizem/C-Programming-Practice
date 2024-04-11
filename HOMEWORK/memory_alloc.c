#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <setjmp.h>
#include "cmocka.h"
#include "memory_alloc.h"

struct memory_alloc_t m;

/* Initialize the memory allocator */
void memory_init() {
  /* TODO */
  int i;
  for(i=0;i<DEFAULT_SIZE;i++) {
    m.blocks[i] = i+1;
  }
  m.blocks[DEFAULT_SIZE-1] = NULL_BLOCK;
  m.first_block = 0;
  m.available_blocks = 16;
}

/* Return the number of consecutive blocks starting from first */
int nb_consecutive_blocks(int first) {
  /* TODO */
  int i = first;
  int s =1;
  while(1) {
    if(m.blocks[i] == i+1) {
      i = i + 1;
      s = s +1;
    }
    else {
      break;
    }
  }
  return s;
}

/* Reorder memory blocks */
void memory_reorder() {
  /* TODO (exercise 2) */
  int i,j;
 // int premier_min_indice;
  //int deuxieme_min_indice;
 // memory_print();
  //printf("element occupée est : %d", NULL_BLOCK);
  //printf("element occupée est : %ld", m.blocks[1]);

  i = 0;
  while(i<DEFAULT_SIZE-1) {
    if(m.blocks[i]>=0) {
      break;
    }
    i +=1;
  }

  m.first_block = i;


  for(i=0;i<DEFAULT_SIZE-1;i++) {

    if((m.blocks[i]>=0) ) {
    //  printf("its happening for %d\n", i);    
        for(j=i+1;j<DEFAULT_SIZE;j++) {
            if((m.blocks[j] >=0)) {
  //            printf("now its happening for %d\n" , j);
              m.blocks[i] = j;
             // printf("m.blocks[%d] est devenu : %d\n", i , j);
             // printf("juste pour tester après i : %ld\n", m.blocks[i]);
              break;
            }
        }

        if(j==i+1) {
          m.blocks[j] = NULL_BLOCK;
        }

    }
  }

  //memory_print();
  
//printf("juste pour tester après i : %ld\n", m.blocks[1]);

}

/* Allocate size bytes
 * return -1 in case of an error
 */
// size est en octet !
// chaque block peut stocker 8 octets
int memory_allocate(size_t size) {
  /* TODO */
  int i;
  int s;
  int j;
  int a;
  int b;
  int t;
  int k;
  b = size / 8;
  a = -1;
  k = 0;
  if(8 * b == size) {
        t = b;
        }
      else {
        t = b + 1;
          }
  
  if(t > m.available_blocks) {
      m.error_no = E_NOMEM;
      return -1;
  }
  while(1) {
    i = m.first_block;
  //  memory_print();
      while(i!=NULL_BLOCK) {
    s = nb_consecutive_blocks(i);
   
    if(s*8 >= size) {
     
        
        if(i == m.first_block) {
          j = 0;
        while(j!= t) {
          m.first_block = m.blocks[m.first_block];
          j = j+1;
        }
        }
        
       m.available_blocks = m.available_blocks - t;
      if(a != -1) {
        m.blocks[a] = m.blocks[i+t-1];
      }
      initialize_buffer(i,size);
      
      m.error_no = E_SUCCESS;
      return i;
      
    }
    else {
      a = i;
      i = m.blocks[i];
    }
  }
  if(k==0) {
   // printf("reordering memory");
    memory_reorder();
//    memory_print();
    k = 1;
    continue;
  }
  else {
     m.error_no = E_SHOULD_PACK;
     return -1;
  }
 

  }
 
}

/* Free the block of data starting at address */
void memory_free(int address, size_t size) {
  /* TODO */
  int i;
  int t;
  int b;
  int a;
 // memory_print();
  //printf("address est :%d\n", address);
  //printf("size est :%ld\n", size);
  

// t est le nombre de blocs
  b = size / 8;
  if(8 * b == size) {
        t = b;
        }
      else {
        t = b + 1;
          }
    //    printf("blocs est :%d\n", t);
a = address;
for(i=1;i<t;i++) {
    m.blocks[a] = a + 1;
    a = a +1;
}

m.blocks[a] = m.first_block;
m.first_block = address;
m.available_blocks += t;

// memory_print();

}

/* Print information on the available blocks of the memory allocator */
void memory_print() {
  printf("---------------------------------\n");
  printf("\tBlock size: %lu\n", sizeof(m.blocks[0]));
  printf("\tAvailable blocks: %lu\n", m.available_blocks);
  printf("\tFirst free: %d\n", m.first_block);
  printf("\tStatus: "); memory_error_print(m.error_no);
  printf("\tContent:  ");

  /* TODO: browse the available blocks and print their index */

  int i = m.first_block;
  int s = 0;

  while(s<=m.available_blocks) {
    if(i == NULL_BLOCK) {
      printf(" NULL_BLOCK");
      break;
     }

    printf("[%d] ->" , i);
     s = s +1;
     i = m.blocks[i];

     if(i == NULL_BLOCK) {
      printf(" NULL_BLOCK");
      break;
     }

  }

  

  printf("\n");
  printf("---------------------------------\n");
}


/* print the message corresponding to error_number */
void memory_error_print(enum memory_errno error_number) {
  switch(error_number) {
  case E_SUCCESS:
    printf("Success\n");
    break;
  case E_NOMEM:
    printf("Not enough memory\n");
    break;
  case  E_SHOULD_PACK:
    printf("Not enough contiguous blocks\n");
    break;
  default:
    break;
  }
}

/* Initialize an allocated buffer with zeros */
void initialize_buffer(int start_index, size_t size) {
  char* ptr = (char*)&m.blocks[start_index];
  for(int i=0; i<size; i++) {
    ptr[i]=0;
  }
}






/*************************************************/
/*             Test functions                    */
/*************************************************/

// We define a constant to be stored in a block which is supposed to be allocated:
// The value of this constant is such as it is different form NULL_BLOCK *and*
// it guarantees a segmentation vioaltion in case the code does something like
// m.blocks[A_B]
#define A_B INT32_MIN

/* Initialize m with all allocated blocks. So there is no available block */
void init_m_with_all_allocated_blocks() {
  struct memory_alloc_t m_init = {
    // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
    {A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B, A_B},
    0,
    NULL_BLOCK,
    INT32_MIN // We initialize error_no with a value which we are sure that it cannot be set by the different memory_...() functions
  };
  m = m_init;
}

/* Test memory_init() */
void test_exo1_memory_init(){
  init_m_with_all_allocated_blocks();

  memory_init();

  // Check that m contains [0]->[1]->[2]->[3]->[4]->[5]->[6]->[7]->[8]->[9]->[10]->[11]->[12]->[13]->[14]->[15]->NULL_BLOCK
  assert_int_equal(m.first_block, 0);

  assert_int_equal(m.blocks[0], 1);
  assert_int_equal(m.blocks[1], 2);
  assert_int_equal(m.blocks[2], 3);
  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 6);
  assert_int_equal(m.blocks[6], 7);
  assert_int_equal(m.blocks[7], 8);
  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], 10);
  assert_int_equal(m.blocks[10], 11);
  assert_int_equal(m.blocks[11], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], 15);
  assert_int_equal(m.blocks[15], NULL_BLOCK);
  
  assert_int_equal(m.available_blocks, DEFAULT_SIZE);

  // We do not care about value of m.error_no
}

/* Initialize m with some allocated blocks. The 10 available blocks are: [8]->[9]->[3]->[4]->[5]->[12]->[13]->[14]->[11]->[1]->NULL_BLOCK */
void init_m_with_some_allocated_blocks() {
  struct memory_alloc_t m_init = {
    // 0           1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
    {A_B, NULL_BLOCK, A_B,   4,   5,  12, A_B, A_B,   9,   3, A_B,   1,  13,  14,  11, A_B},
    10,
    8,
    INT32_MIN // We initialize error_no with a value which we are sure that it cannot be set by the different memory_...() functions
  };
  m = m_init;
}

/* Test nb_consecutive_block() at the beginning of the available blocks list */
void test_exo1_nb_consecutive_blocks_at_beginning_linked_list(){
  init_m_with_some_allocated_blocks();
  memory_print();
  assert_int_equal(nb_consecutive_blocks(8), 2);
}

/* Test nb_consecutive_block() at the middle of the available blocks list */
void test_exo1_nb_consecutive_blocks_at_middle_linked_list(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(nb_consecutive_blocks(3), 3);
}

/* Test nb_consecutive_block() at the end of the available blocks list */
void test_exo1_nb_consecutive_blocks_at_end_linked_list(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(nb_consecutive_blocks(1), 1);
}

/* Test memory_allocate() when the blocks allocated are at the beginning of the linked list */
void test_exo1_memory_allocate_beginning_linked_list(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(memory_allocate(16), 8);
  
  // We check that m contains [3]->[4]->[5]->[12]->[13]->[14]->[11]->[1]->NULL_BLOCK 
  assert_int_equal(m.first_block, 3);

  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], 11);
  assert_int_equal(m.blocks[11], 1);
  assert_int_equal(m.blocks[1], NULL_BLOCK);
  
  assert_int_equal(m.available_blocks, 8);

  assert_int_equal(m.error_no, E_SUCCESS);
}

/* Test memory_allocate() when the blocks allocated are at in the middle of the linked list */
void test_exo1_memory_allocate_middle_linked_list(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(memory_allocate(17), 3);
  
  // We check that m contains [8]->[9]->[12]->[13]->[14]->[11]->[1]->NULL_BLOCK 
  assert_int_equal(m.first_block, 8);

  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], 11);
  assert_int_equal(m.blocks[11], 1);
  assert_int_equal(m.blocks[1], NULL_BLOCK);
  
  assert_int_equal(m.available_blocks, 7);

  assert_int_equal(m.error_no, E_SUCCESS);
}

/* Test memory_allocate() when we ask for too many blocks ==> We get -1 return code and m.error_no is M_NOMEM */
void test_exo1_memory_allocate_too_many_blocks(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(memory_allocate(256), -1);
  
  // We check that m does not change and still contains: [8]->[9]->[3]->[4]->[5]->[12]->[13]->[14]->[11]->[1]->NULL_BLOCK
  assert_int_equal(m.first_block, 8);

  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], 3);
  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], 11);
  assert_int_equal(m.blocks[11], 1);
  assert_int_equal(m.blocks[1], NULL_BLOCK);
  
  assert_int_equal(m.available_blocks, 10);

  assert_int_equal(m.error_no, E_NOMEM);
}

/* Test memory_free() */
void test_exo1_memory_free(){
  init_m_with_some_allocated_blocks();

  memory_free(6, 9);
  
  // We check that m contains: [6]->[7]->[8]->[9]->[3]->[4]->[5]->[12]->[13]->[14]->[11]->[1]->NULL_BLOCK
  assert_int_equal(m.first_block, 6);

  assert_int_equal(m.blocks[6], 7);
  assert_int_equal(m.blocks[7], 8);
  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], 3);
  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], 11);
  assert_int_equal(m.blocks[11], 1);
  assert_int_equal(m.blocks[1], NULL_BLOCK);
  
  assert_int_equal(m.available_blocks, 12);

  // We do not care about value of m.error_no
}

/* Test memory_reorder() */
void test_exo2_memory_reorder(){
  init_m_with_some_allocated_blocks();

  memory_reorder();
  
  // We check that m contains: [1]->[3]->[4]->[5]->[8]->[9]->[11]->[12]->[13]->[14]->NULL_BLOCK
  assert_int_equal(m.first_block, 1);

  assert_int_equal(m.blocks[1], 3);
  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 8);
  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], 11);
  assert_int_equal(m.blocks[11], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], NULL_BLOCK);  
  assert_int_equal(m.available_blocks, 10);

  // We do not care about value of m.error_no
}

/* Test memory_reorder() leading to successful memory_allocate() because we find enough consecutive bytes */
void test_exo2_memory_reorder_leading_to_successful_memory_allocate(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(memory_allocate(32), 11);
  // We check that m contains: [1]->[3]->[4]->[5]->[8]->[9]->NULL_BLOCK
  assert_int_equal(m.first_block, 1);

  assert_int_equal(m.blocks[1], 3);
  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 8);
  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], NULL_BLOCK);
  
  assert_int_equal(m.available_blocks, 6);

  assert_int_equal(m.error_no, E_SUCCESS);
}

/* Test memory_reorder() leading to failed memory_allocate() because not enough consecutive bytes */
void test_exo2_memory_reorder_leading_to_failed_memory_allocate(){
  init_m_with_some_allocated_blocks();

  assert_int_equal(memory_allocate(56), -1);

  // We check that m contains: [1]->[3]->[4]->[5]->[8]->[9]->[11]->[12]->[13]->[14]->NULL_BLOCK
  assert_int_equal(m.first_block, 1);

  assert_int_equal(m.blocks[1], 3);
  assert_int_equal(m.blocks[3], 4);
  assert_int_equal(m.blocks[4], 5);
  assert_int_equal(m.blocks[5], 8);
  assert_int_equal(m.blocks[8], 9);
  assert_int_equal(m.blocks[9], 11);
  assert_int_equal(m.blocks[11], 12);
  assert_int_equal(m.blocks[12], 13);
  assert_int_equal(m.blocks[13], 14);
  assert_int_equal(m.blocks[14], NULL_BLOCK);  
  assert_int_equal(m.available_blocks, 10);

  assert_int_equal(m.available_blocks, 10);

  assert_int_equal(m.error_no, E_SHOULD_PACK);
}

int main(int argc, char**argv) {
  const struct CMUnitTest tests[] = {
    /* a few tests for exercise 1.
     *
     * If you implemented correctly the functions, all these tests should be successfull
     * Of course this test suite may not cover all the tricky cases, and you are free to add
     * your own tests.
     */
    cmocka_unit_test(test_exo1_memory_init),
    cmocka_unit_test(test_exo1_nb_consecutive_blocks_at_beginning_linked_list),
    cmocka_unit_test(test_exo1_nb_consecutive_blocks_at_middle_linked_list),
    cmocka_unit_test(test_exo1_nb_consecutive_blocks_at_end_linked_list),
    cmocka_unit_test(test_exo1_memory_allocate_beginning_linked_list),
    cmocka_unit_test(test_exo1_memory_allocate_middle_linked_list),
    cmocka_unit_test(test_exo1_memory_allocate_too_many_blocks),
    cmocka_unit_test(test_exo1_memory_free),

    /* Run a few tests for exercise 2.
     *
     * If you implemented correctly the functions, all these tests should be successfull
     * Of course this test suite may not cover all the tricky cases, and you are free to add
     * your own tests.
     */

    cmocka_unit_test(test_exo2_memory_reorder),
    cmocka_unit_test(test_exo2_memory_reorder_leading_to_successful_memory_allocate),
    cmocka_unit_test(test_exo2_memory_reorder_leading_to_failed_memory_allocate)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
